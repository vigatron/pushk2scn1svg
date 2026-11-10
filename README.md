# vhpackv81scan

**Binary structure scanner that renders SVG "DNA" fingerprints and windowed byte-statistics reports.**

`vhpackv81scan` reads a raw binary file (sprite sheets, tile cells, ROM dumps,
packed blobs — anything) and produces three self-contained SVG documents that
make the internal structure of the bytes visible to the human eye.

- Version: `0.1.0` (prototype, pre-1.0)
- Standard: C++17, CMake >= 3.16, no runtime dependencies
- Third-party code: four read-only git submodules under `src/extmods/`
- Tested on Linux x86-64; Windows code paths exist but are not verified

## 1. What the tool does

For every byte value `0..255` the tool records where that value appears in the
source buffer, in two coordinate spaces:

- **ABS** — absolute offsets from the start of the file;
- **REL** — offsets counted from the first occurrence of the same value.

Positions are stored as one bit set per value (32 768 positions max) and
rendered as one horizontal "chromosome" row per byte value. Repetition,
periodic tables, padding, compressed versus uncompressed regions and small
numeric ranges become visible as texture.

In addition a *window sheet* is produced: the data is split into blocks of
64, 128, 256 and 512 bytes, and every block is reduced to a few statistics
(min, max, range, number of distinct values, histogram). The sheet has one row
per block size with a section view and a density view side by side.

Typical uses: inspecting tile/sprite data (`data/*.cells`) before writing a
decoder; comparing two versions of the same asset (SHA-256 is printed on the
DNA sheets); finding padding, alignment and repetition in packed formats; a
quick visual check of a packing or compression step.

## 2. Build

Requirements: a C++17 compiler, CMake >= 3.16, `git`.

```sh
git clone --recurse-submodules <repo-url> vhpack81scansvg
cd vhpack81scansvg
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

The executable is `build/vhpackv81scan`. If submodules were not cloned:

```sh
git submodule update --init --recursive
```

Notes:

- `CMAKE_BUILD_TYPE` is not forced by `CMakeLists.txt`; without an explicit
  `Release` the build is unoptimised. `CMAKE_EXPORT_COMPILE_COMMANDS` is on, so
  `build/compile_commands.json` is available for clangd and IDE integration.
- The four submodules (`vhlibplatform`, `vhlibargs`, `vhlibfsys`, `vhlibsvg`)
  are read-only dependencies and are never modified by this project.

## 3. Usage

```text
vhpackv81scan -fname <source-file> [-wndsz <2..256>]
```

| Option | Default | Meaning |
|---|---|---|
| `-fname` | — (required) | Path to the binary file to analyse. |
| `-wndsz` | `128` | Window size in bytes, range `2..256`. |

`-fname` must point to an existing regular file. Out-of-range `-wndsz` values
are rejected by the argument parser.

```sh
./build/vhpackv81scan -fname data/backobjs.recombo.cells -wndsz 128
```

Output (written to the current working directory, not next to the source):

```text
Data successfully saved to backobjs.recombo.cells_wnd.svg
Data successfully saved to backobjs.recombo.cells_dna_abs.svg
Data successfully saved to backobjs.recombo.cells_dna_rel.svg
```

Exit code `0` means all three documents were produced; any failure returns `1`.

### What `-wndsz` controls today

- It is the **minimum accepted source size**: `size < wndsz` is an error.
- It is **not** used by the window sheet: block sizes are hard-coded to
  `64, 128, 256, 512`.
- It is bounded to `2..256`, so files smaller than `-wndsz` cannot be analysed
  with default settings. Use `-wndsz 2` for tiny samples.

## 4. Output documents

All three files are plain SVG with absolute coordinates, no external assets and
no embedded fonts.

### 4.1 `<base>_wnd.svg` — window sheet

Canvas 3200 x 2400 px, 100 px border. Between a 200 px empty header and a
200 px footer sits a 2 x 4 grid; each cell has 64 px inner padding and a shadow
box. Rows use block sizes 64, 128, 256 and 512 bytes; the left column is a
Section view, the right column a Density view. A cell is split vertically into
graphics (80 %), delta strip (10 %) and scale strip (10 %). One horizontal band
exists per block of the given size, so a 16 KB file yields 256 bands on row 0
and 32 bands on row 3.

The footer contains the data-type legend: 7 columns x 4 rows, 28 type names
(8-bit, 16-bit BE/LE, 32-bit, float16/32/64, packed 5:6:5 and 4:4:4:4 layouts),
with the selected type highlighted as `[ X ]`. The header area is reserved but
empty: the sheet carries no title, file name or hash.

### 4.2 `<base>_dna_abs.svg` — absolute DNA map

Canvas 2800 x 2600 px, 240 x 200 px margins. The body holds 256 rows, one per
byte value, each 6 px high with a 1 px gap, labelled `#0` .. `#255`.

Every recorded position is drawn as a 1 px vertical line at
`pos * innerWidth / fileSize`, so each row spans the whole file. Line colour
alternates every 2 px to make density visible.

Footer fields: source file name, size in bytes, SHA-256 (lower-case hex, 64
characters) and local date/time in `%d-%b-%Y %H:%M` format.

### 4.3 `<base>_dna_rel.svg` — relative DNA map

Same layout, but positions are offsets from the first occurrence of the same
byte value (the first occurrence itself is at offset 0). Both maps share the
same horizontal scale (fraction of the file size), so a REL row with a late
first occurrence occupies only the left part of the row.

## 5. How it works

The pipeline is short and single-threaded:

1. **Parse CLI.** `VHAppCoreCfg::ParseArgs()` validates `-fname` and `-wndsz`,
   checks that the file exists, derives the three output names.
2. **Load.** `VHSrcDataArray::LoadSource()` reads the whole file into
   `std::vector<uint8_t>`, records the size and computes SHA-256.
3. **Validate.** The buffer must be at least `-wndsz` bytes and at most 32 768.
4. **Scan ABS.** For each index `i`, `wscanabs[src[i]].putabs(src[i], i)`.
5. **Scan REL.** `wscanrel[src[i]].putrel(i)` marks
   `i - firstPositionOf(src[i])`, including offset 0 for the first occurrence.
6. **Render window sheet.** For each block size the buffer is cut into blocks,
   each block is reduced to `BlockProps`, and both columns are drawn.
7. **Render DNA ABS / DNA REL.** One row per byte value; every set bit becomes
   a vertical line.
8. **Save.** Each document is accumulated in memory and written in one pass.

**Fingerprint storage.** `VHKeyFingerPrint` owns a 4096-byte bit set
(`32 768 / 8`) plus first and last position. One `VHWideRangeScan` holds 256 of
them, about 1 MB of heap per scan; the application keeps two scans alive, so
roughly 2 MB of scan state in total.

**Block statistics.** `BlockProps::scan()` walks a block once and computes
`_min`, `_max`, a histogram `spc[256]` saturated at 255 and `wght` — the
number of distinct values (capped at 255). Cost is linear in block size, so
scanning the file at four block sizes costs about four linear passes.

## 6. Architecture

The project is split into small modules, one folder per component, under
`src/mods/`. The entry point `src/main.cpp` only wires `VHAppCore` calls.

```text
main.cpp
 └─ VHAppCore                       src/mods/vhappcore
     ├─ VHAppCoreCfg                src/mods/vhappcorecfg
     │   ├─ VHArgsParser            extmods/vhlibargs
     │   └─ VHFSYS                  extmods/vhlibfsys
     ├─ VHSrcDataArray              src/mods/datasource
     │   ├─ VHFile                  extmods/vhlibfsys
     │   └─ VHSHA256                src/mods/vhsha256
     ├─ VHWideRangeScan x2          src/mods/widerange
     │   └─ VHKeyFingerPrint x256
     ├─ VHDocWnd                    src/mods/generatewnd
     │   ├─ BlockProps              src/mods/structs
     │   └─ draw (free functions)   src/mods/draw
     └─ VHDocDNA x2                 src/mods/generatedna
         └─ VHWideRangeScan, VHKeyFingerPrint
```

`VHSVG` and `VHSVGRect` come from `extmods/vhlibsvg` and are used by both
generators. `VHLogger` (`dbg`) is a global diagnostic sink declared in
`src/mods/global`.

Layering rules: `datasource` and `widerange` know nothing about SVG;
`generatewnd` / `generatedna` know `widerange` and `draw`, not each other;
`vhappcore` knows all of them.

## 7. Class reference

| Class | Folder | Role |
|---|---|---|
| `VHAppCore` | `vhappcore` | Owns configuration, source data and both scans; exposes `LoadAndScan`, `GenerateWnd`, `GenerateDNAAbs`, `GenerateDNARel`. |
| `VHAppCoreCfg` | `vhappcorecfg` | Command-line parsing, output-name derivation, `subsize`. |
| `VHSrcDataArray` | `datasource` | Whole-file load, SHA-256, size. |
| `VHWideRangeScan` | `widerange` | 256 fingerprints plus `scanabs` / `scanrel`. |
| `VHKeyFingerPrint` | `widerange` | Bit set of positions for one byte value. |
| `BlockProps` | `structs` | Per-block min/max/histogram/width. |
| `VHDocWnd` | `generatewnd` | Builds the window sheet. |
| `VHDocDNA` | `generatedna` | Builds the DNA sheet. |
| `Colors` | `draw` | Named palette used by the drawing helpers. |
| `VHLogger` | `vhlogger` | Diagnostic output (stdout or file). |
| `VHSHA256` | `vhsha256` | SHA-256 of a byte vector. |

Each class has a requirement document in `rules/` (index:
`rules/README.md`). The documents are the source of truth for
responsibilities and must be updated together with the code.

## 8. Repository layout

```text
CMakeLists.txt              build definition
README.md                   this file
AGENTS.md                   rules for contributors / AI agents
data/                       sample binary assets
rules/                      per-class requirement documents
src/main.cpp                entry point
src/mods/                   first-party modules
  vhappcore/                application state
  vhappcorecfg/             CLI configuration
  datasource/               source buffer, SHA-256, size
  widerange/                fingerprints and scans
  structs/                  BlockProps
  generatewnd/              window sheet generator
  generatedna/              DNA sheet generator
  draw/                     drawing helpers and palette
  vhlogger/                 logger
  vhsha256/                 SHA-256 implementation
  global/                   global `dbg` and debug helpers
src/extmods/                read-only git submodules
  vhlibplatform/            verr/vok, VHBits, assertions
  vhlibargs/                command-line parser
  vhlibfsys/                VHFile, VHFSYS
  vhlibsvg/                 VHSVG, VHSVGRect, string tools
```

## 9. Data limits and invariants

| Limit | Value | Where |
|---|---|---|
| Minimum source size | `-wndsz` bytes (default 128) | `VHSrcDataArray::LoadSource` |
| Minimum scan size | 2 bytes | `VHWideRangeScan::scanabs/scanrel` |
| Maximum scan size | 32 768 bytes | `VHKeyFingerPrint::MAX_RANGE` |
| Position type | `uint16_t` | `VHKeyFingerPrint` |
| Values tracked | 256 | `VHWideRangeScan::MAX_ELMS` |
| Fingerprint memory | 4096 bytes each, ~1 MB per scan | `VHKeyFingerPrint` |
| Window block sizes | 64, 128, 256, 512 bytes | `VHDocWnd::kBaseChunkSize` |
| Window sheet canvas | 3200 x 2400 px | `VHDocWnd` |
| DNA sheet canvas | 2800 x 2600 px | `VHDocDNA` |
| DNA rows | 256 | `VHDocDNA::Gen256Stat` |

The 32 KB cap follows directly from storing positions as `uint16_t` bit
indices. Larger files are rejected with
`Wide Range (abs) analisys failed`.

## 10. Known limitations

The code is a working prototype; the following items are known.

**Correctness**

- `DrawSection` / `DrawDensity` receive the total file size as the scale value
  while the caption prints the block size, so the scale labels (16384, 8192,
  ...) do not match the "window size" text.
- The density view draws a full-width line for every value present in a block
  and ignores the histogram count, so it does not display density.
- The section bar height uses `wght * height / 128` while `wght` can reach 255,
  so the bar can exceed the cell.
- `DrawTypes` indexes the type list with `i * 4 + j` over a 7 x 4 grid, which
  transposes the intended row-major order.

**Robustness**

- `VHSVG::savetosvg()` returns `void`; write errors cannot be detected or
  reported by the application.
- `VHSVG::begin()` does not clear appended content. Re-using a `VHSVG` instance
  (for example `VHDocDNA::svg`) would nest two documents.
- `VHAppCoreCfg::subsize` and all `BlockProps` fields are uninitialised until
  their first use.
- `VHWideRangeScan::operator[]` performs no bounds check.
- `main()` collapses every error to exit code `1`; the detailed code produced
  by `verrmsg()` is lost. Error text goes to `stdout`, not `stderr`.

**Design / product**

- `-wndsz` doubles as a minimum file size and has no effect on the window
  sheets.
- Input is limited to 32 KB, which is small for a "wide range" scanner.
- Reports are written to the current working directory, never next to the
  source; there is no `-outdir` option.
- Every mark in the DNA map produces its own `<line>` element: a 16 KB input
  already produces a 1.3 MB document; identical x positions are not merged.
- `generatedna.hpp` includes `vhappcore.hpp`, a reverse dependency that is not
  needed.
- The window sheet header is empty; source name, size and hash are not printed.
- Font families are misspelled (`verdna`, `noto`), so viewers fall back to a
  default font. The only localised string is `"Размер окна: N байт"` in
  `draw_section.cpp`.

**Quality gates**

- No unit tests, no golden-file/SVG regression tests, no CI configuration.
- No compiler warning flags in `CMakeLists.txt`; a `-Wall -Wextra` build emits
  10 warnings, `-Wall -Wextra -Wconversion` about 86 (mostly `int` to
  `uint16_t`/`int16_t`).
- Doxygen coverage is partial: `BlockProps`, `Colors`, `VHWideRangeScan` and
  `VHKeyFingerPrint` methods have few or empty comments.
- The per-class folder rule from `AGENTS.md` is not followed by `datasource`,
  `structs`, `widerange`, `generatewnd`, `generatedna` and `draw`; the mapping
  is documented in `rules/README.md` instead.
- `tmp/` still contains early drafts that duplicate submodule headers.

## 11. Performance

Measured on a desktop x86-64 machine, release build, 16 KB sample
(`data/backobjs.recombo.cells`):

| Metric | Value |
|---|---|
| Wall time, all three documents | ~0.05 s |
| Peak RSS | ~8 MB |
| Window sheet | ~319 KB |
| DNA ABS sheet | ~1.38 MB |
| DNA REL sheet | ~1.36 MB |
| DNA ABS line elements | 16 384 |

The algorithm is linear in input size. Because the input is capped at 32 KB,
performance is not a practical concern today. If the cap is lifted, the first
bottlenecks will be the `std::vector<std::string>` accumulation inside `VHSVG`
(one heap string per SVG element), one `<line>` per marked position without
coordinate deduplication, and two independent full passes for ABS and REL.

## 12. Conventions

- C++17, `#pragma once`, no exceptions thrown explicitly in project code;
  errors use `verr` / `vok` / `verror()` / `verrmsg()` from `vhlibplatform`.
- Class `.hpp` and `.cpp` files live in `src/mods/<classname>/` where
  `<classname>` is the lower-case class name; `rules/` documents deviations.
- Every class method should carry a Doxygen comment (`@brief`, `@param`,
  `@return`).
- `src/extmods/` is read-only: it contains git submodules and must never be
  edited, only read for API reference. `rules/` is the requirement source of
  truth; code changes must keep the corresponding document in sync.

## 13. Troubleshooting

| Message / symptom | Cause and fix |
|---|---|
| `Wide Range (abs) analisys failed` | Input is larger than 32 768 bytes — a hard limit of the bit-index representation. |
| `Minimal size less than N` | Source is smaller than `-wndsz` (default 128). Lower `-wndsz`, minimum 2. |
| `Wrong input file <path>` | `-fname` does not exist or points to a directory. |
| `key passed without param` | An option was supplied without a value, e.g. `-fname` as the last argument. |
| No SVG although the source exists | Output goes to the CWD — check the exit code and that the CWD is writable. |
| Everything renders in a generic font | Fonts are not embedded and family names are misspelled; fix `draw_*.cpp`. |
| The REL sheet looks almost empty | REL uses the same x scale as ABS: rows with a late first occurrence occupy only the left part of the row. |

## 14. Roadmap

1. Replace the 32 KB `uint16_t` bit index with a scalable representation
   (64-bit index or per-block summaries) to support megabyte files.
2. Pass the block size, not the file size, to `DrawScale`; use the histogram in
   the density view.
3. Make `VHSVG::savetosvg` report failures and stop printing to `stdout` from
   the library layer.
4. Stream SVG to the output file instead of accumulating strings in memory.
5. Add `-outdir`, real `-wndsz` semantics and `--version` driven by the CMake
   `PROJECT_VERSION`.
6. Add unit tests for SHA-256, fingerprints and both coordinate modes, plus
   golden-file tests for the three SVG documents.
7. Enable `-Wall -Wextra -Wconversion` for project sources and a default
   `Release` build type.

**Glossary.** *Wide range scan* — a pass recording, for all 256 byte values,
the positions where they occur. *Fingerprint* — the bit set plus first/last
position of one byte value. *DNA map* — the 256-row visualisation of all
fingerprints. *ABS / REL* — offsets from the file start / from the first
occurrence. *Window* — a 64..512 byte slice reduced to `BlockProps`.

---

*This README documents version 0.1.0. Keep the version in `CMakeLists.txt`, the
`setAppDesc()` string in `vhappcorecfg.cpp` and this file in sync.*
