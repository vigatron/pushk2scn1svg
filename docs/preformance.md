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
