## 12. Known limitations

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

- `VHSVG::savetosvg()` returns `void` and prints its own success message; the
  application can only verify the result afterwards via `VHSvgTools::Save()`
  (file exists, is non-empty and ends with `</svg>`), not detect the exact
  write error or suppress the library message.
- `VHSVG::begin()` does not clear appended content. Re-using a `VHSVG`
  instance (for example `VHDocDNA::svg`) would nest two documents.
- `VHWideRangeScan::operator[]` performs no bounds check.
- `main()` maps failures to phase-level exit codes (3..8), but the detailed
  code produced by `verrmsg()` is still discarded. Error text goes to
  `stdout`, not `stderr`.

**Design / product**

- `-wndsz` doubles as a minimum file size and has no effect on the window
  sheets.
- Reports are written to the current working directory by default, never next
  to the source; use `-outfolder` to select another folder (created if missing).
- Every mark in the DNA map produces its own `<line>` element: a 16 384-byte
  input already produces a 1.3 MB document; identical x positions are not
  merged.
- The window sheet header is empty; source name, size and hash are not printed.
- The types legend requests the `noto` font family, which may be unavailable
  and fall back to a default font. The only localised string is
  `"Размер окна: N байт"` in `src/mods/vhsvgdraw/vhsvgdraw.cpp`.

**Quality gates**

- No unit tests, no golden-file/SVG regression tests, no CI configuration.
- No compiler warning flags in `CMakeLists.txt`; a strict build with
  `-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wunused -Wnon-virtual-dtor`
  emits 76 warnings in first-party code, mostly
  `int`/`float` conversions in `vhsvgdraw.cpp`.
- Doxygen coverage is partial: `VHBlockStat`, `Colors`, `VHWideRangeScan` and
  `VHKeyFingerPrint` methods have few or empty comments.
- The per-class folder rule from `AGENTS.md` is not followed by `datasource`,
  `widerange`, `generatewnd` and `generatedna`; `colors`, `vhsvgdraw` and `svgtools` already follow it.
- `tmp/` still contains early drafts that duplicate submodule headers.
