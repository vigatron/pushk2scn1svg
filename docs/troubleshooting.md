## 13. Troubleshooting

| Message / symptom | Cause and fix |
|---|---|
| `Source file too large <path> (max 32768 bytes)` | Input exceeds the 32 KiB limit of the `uint16_t` bit-index representation. Slice the file or analyse a smaller region. |
| `File too large <path> (max 32768 bytes)` | The file grew after `ParseArgs` and was rejected again by `VHSrcDataArray::LoadSource` (exit code `3`). |
| `Invalid value for -wndsz: <value>` | `-wndsz` is not a plain decimal integer (`abc`, `2x`) or overflows `int`. |
| `Minimal size less than N` | Source is smaller than `-wndsz` (default 128). Lower `-wndsz`, minimum 2. |
| `Wrong input file <path>` | `-fname` does not exist or is not a regular file (directory, device, FIFO, ...). |
| `Can't create output folder <path>` | `-outfolder` could not be created — check permissions or whether the path is occupied by a file. |
| `key passed without param` | An option was supplied without a value, e.g. `-fname` as the last argument. |
| No SVG although the source exists | Output goes to the CWD (or to `-outfolder`) — check the exit code and that the output folder is writable. |
| `Error generating docwnd` (exit `6`) | The output folder is not writable or the SVG file could not be written. Check permissions and free space. |
| `Error generating docdna abs` / `rel` (exit `7` / `8`) | Same as above for the DNA sheets; make sure the output folder did not become read-only between phases. |
| `Data successfully saved ...` but the file is empty | `VHSVG::savetosvg()` prints its message before `VHSvgTools::Save()` verifies the file; the following `Error generating ...` line and the non-zero exit code indicate the real result. |
| Everything renders in a generic font | Fonts are not embedded; the types legend requests `noto`, which may fall back. Install Noto or change the family in `src/mods/vhsvgdraw/vhsvgdraw.cpp`. |
| The REL sheet looks almost empty | REL uses the same x scale as ABS: rows with a late first occurrence occupy only the left part of the row. |
