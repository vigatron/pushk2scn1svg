## 11. Data limits and invariants

| Limit | Value | Where |
|---|---|---|
| Minimum source size | `-wndsz` bytes (default 128) | `VHSrcDataArray::LoadSource` |
| Maximum source size | 32 768 bytes (32 KiB) | `VHSrcDataArray::MAX_SOURCE_SIZE` |
| Minimum scan size | 2 bytes | `VHWideRangeScan::scanabs/scanrel` |
| Maximum scan size | 32 768 bytes | `VHKeyFingerPrint::MAX_RANGE` |
| Position type | `uint16_t` | `VHKeyFingerPrint` |
| Values tracked | 256 | `VHWideRangeScan::MAX_ELMS` |
| Fingerprint memory | 4096 bytes each, ~1 MB per scan | `VHKeyFingerPrint` |
| Window block sizes | 64, 128, 256, 512 bytes | `VHDocWnd::kBaseChunkSize` |
| Window sheet canvas | 3200 x 2400 px | `VHDocWnd` |
| DNA sheet canvas | 2800 x 2600 px | `VHDocDNA` |
| DNA rows | 256 | `VHDocDNA::Gen256Stat` |

The maximum source size for processing is 32 768 bytes (32 KiB). The cap
follows directly from storing positions as `uint16_t` bit indices. Larger
files are rejected by `VHAppCoreCfg::ParseArgs()` before the output folder
is created (exit code `1`) and re-checked by `VHSrcDataArray::LoadSource()`
(exit code `3`) in case the file grows between the two phases.
