# pushk2scn1svg

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.16+-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/Version-0.1.0-orange.svg)](https://github.com/vigatron/pushk2scn1svg/releases)

**Tool name** `pushk2scn1svg`

Author : V01G04A81 Viktor Glebov  
Source code : https://github.com/vigatron/pushk2scn1svg  
License : MIT

**Binary structure scanner that renders SVG "DNA" fingerprints and windowed byte-statistics reports.**

`pushk2scn1svg` reads a raw binary file (sprite sheets, tile cells, ROM dumps,
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
rendered as one horizontal "chromosome" row per byte value.

In addition a *window sheet* is produced: the data is split into blocks of
64, 128, 256 and 512 bytes, and every block is reduced to a few statistics
(min, max, range, number of distinct values, histogram).

Typical uses: inspecting tile/sprite data (`data/*.cells`) before writing a
decoder; comparing two versions of the same asset (SHA-256 is printed on the
DNA sheets); finding padding, alignment and repetition in packed formats.

## 2. Build

Requirements: a C++17 compiler, CMake >= 3.16, `git`.

```sh
git clone --recurse-submodules <repo-url> pushk2scn1svg
cd pushk2scn1svg
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

The executable is `build/pushk2scn1svg`. If submodules were not cloned:

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
pushk2scn1svg -fname <source-file> [-wndsz <2..256>] [-outfolder <folder>]
```

| Option | Default | Meaning |
|---|---|---|
| `-fname` | — (required) | Path to the binary file to analyse. |
| `-wndsz` | `128` | Window size in bytes, range `2..256`. |
| `-outfolder` | current directory | Folder for the three output SVG files. Created automatically (including nested paths) if missing. |

`-fname` must point to an existing regular file no larger than 32 768 bytes
(32 KiB); larger files are rejected before the output folder is created.
`-wndsz` must be a plain decimal integer: `abc`, `2x` and `int` overflow are
rejected, and values outside `2..256` are rejected by the argument parser.
`-outfolder` is created automatically, including missing parent directories;
if creation fails, the tool exits with code `1`. Full CLI rules are in
[`rules/CMDLine.md`](rules/CMDLine.md).

```sh
./build/pushk2scn1svg -fname data/backobjs_recombo.cells -wndsz 128
./build/pushk2scn1svg -fname data/backobjs_recombo.cells -outfolder out
```

Output (written to the current working directory by default, or to
`-outfolder`):

```text
Data successfully saved to backobjs_recombo.cells_wnd.svg
Data successfully saved to backobjs_recombo.cells_dna_abs.svg
Data successfully saved to backobjs_recombo.cells_dna_rel.svg
```

Exit code `0` means all three documents were produced. Phase failures return
distinct non-zero codes: `1` command line, `3` source load, `4` ABS scan, `5`
REL scan, `6` WND render, `7` DNA ABS render, `8` DNA REL render.

`-wndsz` doubles as a minimum accepted source size and has no effect on the
window sheets: block sizes are hard-coded to `64, 128, 256, 512`.

## 4. Output documents

All three files are plain SVG with absolute coordinates, no external assets
and no embedded fonts. User-visible strings (source file name and captions)
are XML-escaped before insertion, so names containing `&`, `<`, `>` or quotes
produce valid XML.

- `<base>_wnd.svg` — window sheet: canvas 3200 x 2400 px, 100 px border, 2 x 4
  grid of block-size rows (left column: Section view, right column: Density
  view), data-type legend in the footer (7 columns x 4 rows, 28 type names).
- `<base>_dna_abs.svg` — absolute DNA map: canvas 2800 x 2600 px, 256 rows (one
  per byte value), each recorded position drawn as a 1 px vertical line at
  `pos * innerWidth / fileSize`.
- `<base>_dna_rel.svg` — relative DNA map: same layout, positions are offsets
  from the first occurrence of the same value (first occurrence at offset 0).

DNA sheet footers carry the source file name, size in bytes, SHA-256 (lower-case
hex, 64 characters) and local date/time in `%d-%b-%Y %H:%M` format.

## 5. How it works

The pipeline is short and single-threaded:

1. **Parse CLI.** `VHAppCoreCfg::ParseArgs()` strictly validates `-wndsz`,
   checks that `-fname` is a regular file of at most 32 768 bytes, validates
   `-outfolder`, creates the output folder if needed, and derives the three
   output names.
2. **Load.** `VHAppCore::LoadSource()` delegates to
   `VHSrcDataArray::LoadSource()`, which reads the whole file in binary mode
   into `std::vector<uint8_t>`, records the size and computes SHA-256. The
   same `[wndsz, 32 768]` size window is enforced here again.
3. **Validate.** `VHAppCore::LoadSource()` checks that the buffer is at least
   `-wndsz` bytes; the upper bound of 32 768 bytes (32 KiB) was enforced during
   loading.
4. **Scan ABS.** `VHAppCore::ScanAbs()` runs the absolute scan: for each index
   `i`, `wscanabs[src[i]].putabs(i)`.
5. **Scan REL.** `VHAppCore::ScanRel()` runs the relative scan:
   `wscanrel[src[i]].putrel(i)` marks `i - firstPositionOf(src[i])`, including
   offset 0 for the first occurrence. `ScanAll()` is a convenience wrapper for
   both scans.
6. **Render window sheet.** For each block size the buffer is cut into blocks,
   each block is reduced to `VHBlockStat`, and both columns are drawn.
7. **Render DNA ABS / DNA REL.** One row per byte value; every set bit becomes
   a vertical line.
8. **Save.** Each document is accumulated in memory and written in one pass
   through `VHSvgTools::Save()`, which checks that the output file exists and
   ends with `</svg>`. Failed writes now produce exit codes `6`/`7`/`8` instead
   of a silent success.

**Fingerprint storage.** `VHKeyFingerPrint` owns a 4096-byte bit set
(`32 768 / 8`) plus first and last position. One `VHWideRangeScan` holds 256 of
them, about 1 MB of heap per scan; the application keeps two scans alive, so
roughly 2 MB of scan state in total.

**Block statistics.** `VHBlockStat::scan()` walks a block once and computes
`_min`, `_max`, a histogram `spc[256]` saturated at 255 and `wght` — the number
of distinct values (capped at 255). Cost is linear in block size, so scanning
the file at four block sizes costs about four linear passes.

## 6. Architecture

The project is split into small modules, one folder per component, under
`src/mods/`. The entry point `src/main.cpp` only wires `VHAppCore` calls.

```text
main.cpp
 └─ VHAppCore                       src/mods/vhappcore
     ├─ VHAppCoreCfg                src/mods/vhappcorecfg
     │   ├─ VHArgsParser            extmods/vhlibargs
     │   ├─ VHFSYS                  extmods/vhlibfsys
     │   └─ MAX_SOURCE_SIZE         src/mods/datasource
     ├─ VHSrcDataArray              src/mods/datasource
     │   ├─ std::filesystem, std::ifstream
     │   └─ VHSHA256                src/mods/vhsha256
     ├─ VHWideRangeScan x2          src/mods/widerange
     │   └─ VHKeyFingerPrint x256
     ├─ VHDocWnd                    src/mods/generatewnd
     │   ├─ VHBlockStat              src/mods/vhblockstat
     │   ├─ VHSVGDraw               src/mods/vhsvgdraw
     │   ├─ Colors                  src/mods/colors
     │   └─ VHSvgTools              src/mods/svgtools
     └─ VHDocDNA x2                 src/mods/generatedna
         ├─ VHWideRangeScan, VHKeyFingerPrint
         ├─ Colors                  src/mods/colors
         └─ VHSvgTools              src/mods/svgtools
```

`VHSVG` and `VHSVGRect` come from `extmods/vhlibsvg` and are used by both
generators. `VHLogger` (`dbg`) is a global diagnostic sink declared in
`src/mods/global`.

Layering rules: `datasource` and `widerange` know nothing about SVG;
`generatewnd` knows `VHSVGDraw` and `Colors`, `generatedna` knows `Colors`,
and the two generators do not know each other. Both use `VHSvgTools` for
XML-safe text and verified saving; `vhappcore` knows all of them.

## 7. Documentation

- [Class reference](docs/class_reference.md) — per-class responsibilities and
  API.
- [Performance](docs/performance.md) — measured wall time, memory and output
  sizes.
- [Data limits and invariants](docs/data_limits.md) — processing limits and
  invariants.
- [Known limitations](docs/known_limitations.md) — current known issues.
- [Troubleshooting](docs/troubleshooting.md) — message / symptom table.

## 8. Repository layout

```text
CMakeLists.txt              build definition
README.md                   this file
AGENTS.md                   rules for contributors / AI agents
data/                       sample binary assets
docs/                       deep-dive documentation
rules/                      per-class requirement documents
src/main.cpp                entry point
src/mods/                   first-party modules
  vhappcore/                application state
  vhappcorecfg/             CLI configuration
  datasource/               source buffer, SHA-256, size
  widerange/                fingerprints and scans
  vhblockstat/              VHBlockStat
  generatewnd/              window sheet generator
  generatedna/              DNA sheet generator
  vhsvgdraw/                drawing helpers for window sheet
  colors/                   SVG color palette
  vhlogger/                 logger
  vhsha256/                 SHA-256 implementation
  global/                   global `dbg` and debug helpers
src/extmods/                read-only git submodules
  vhlibplatform/            verr/vok, VHBits, assertions
  vhlibargs/                command-line parser
  vhlibfsys/                VHFile, VHFSYS
  vhlibsvg/                 VHSVG, VHSVGRect, string tools
```

## 9. Performance

See [Performance](docs/performance.md) for details.

## 10. Conventions

- C++17, `#pragma once`, no exceptions thrown explicitly in project code;
  errors use `verr` / `vok` / `verror()` / `verrmsg()` from `vhlibplatform`.
- Class `.hpp` and `.cpp` files live in `src/mods/<classname>/` where
  `<classname>` is the lower-case class name; `rules/` documents deviations.
- Every class method should carry a Doxygen comment (`@brief`, `@param`,
  `@return`).
- `src/extmods/` is read-only: it contains git submodules and must never be
  edited, only read for API reference. `rules/` is the requirement source of
  truth; code changes must keep the corresponding document in sync.

## License

This project is distributed under the [MIT License](https://opensource.org/licenses/MIT).

(С) Viktor Glebov 2026 V01G04A81
