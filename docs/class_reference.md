## 7. Class reference

| Class | Folder | Role |
|---|---|---|
| `VHAppCore` | `vhappcore` | Owns configuration, source data and both scans; exposes `LoadSource`, `ScanAbs`, `ScanRel`, `ScanAll`, `GenerateWnd`, `GenerateDNAAbs`, `GenerateDNARel`. |
| `VHAppCoreCfg` | `vhappcorecfg` | Command-line parsing, output-name derivation, `subsize`. |
| `VHSrcDataArray` | `datasource` | Whole-file load, SHA-256, size. |
| `VHWideRangeScan` | `widerange` | 256 fingerprints plus `scanabs` / `scanrel`. |
| `VHKeyFingerPrint` | `widerange` | Bit set of positions for one byte value. |
| `VHBlockStat` | `vhblockstat` | Per-block min/max/histogram/width. |
| `VHDocWnd` | `generatewnd` | Builds the window sheet. |
| `VHDocDNA` | `generatedna` | Builds the DNA sheet. |
| `VHSVGDraw` | `vhsvgdraw` | Stateless drawing helpers for window sheet elements (sections, density, scale, types). |
| `Colors` | `colors` | Named palette used by the drawing helpers. |
| `VHLogger` | `vhlogger` | Diagnostic output (stdout or file). |
| `VHSHA256` | `vhsha256` | SHA-256 of a byte vector. |

Each class has a requirement document in `rules/` (index: `rules/README.md`). The documents are the source of truth for responsibilities and must be updated together with the code.
