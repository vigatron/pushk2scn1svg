# VHDocWnd

## Файл / Модуль

- Заголовок: `src/mods/generatewnd/generatewnd.hpp`
- Реализация: `src/mods/generatewnd/generatewnd.cpp`
- Класс: `class VHDocWnd`

## Назначение

Генерация SVG-документа «оконного» вида (wnd): сетка 2×4 на листе,
где каждая строка соответствует размеру чанка `kBaseChunkSize << row`
(64, 128, 256, 512 байт), левая колонка — Section-визуализация статистики
блоков, правая — Density-визуализация, внизу — легенда типов данных.

## Ответственность

- Расчёт макета документа (лист, поля, заголовок/подвал, сетка, легенда).
- Разбиение данных на блоки и вычисление `VHBlockStat` для каждого размера чанка.
- Отрисовка сетки (Section слева, Density справа) и легенды типов.
- Сборка документа через `VHSVG` и сохранение в файл.

## Константы макета

| Имя | Значение | Назначение |
|---|---|---|
| `kPaperWidth` | 3200 | Ширина листа SVG (пикс.). |
| `kPaperHeight` | 2400 | Высота листа SVG (пикс.). |
| `kPaperBorderX` | 100 | Горизонтальные поля листа. |
| `kPaperBorderY` | 100 | Вертикальные поля листа. |
| `kHeaderHeight` | 200 | Высота области заголовка. |
| `kFooterHeight` | 200 | Высота области подвала (легенда). |
| `kGridCols` | 2 | Количество колонок сетки. |
| `kGridRows` | 4 | Количество строк сетки. |
| `kBaseChunkSize` | 64 | Базовый размер чанка; строка `j` использует `64 << j`. |
| `kCellPadding` | 64 | Внутренний отступ ячейки сетки. |
| `kTypesPaddingX` | 64 | Горизонтальный отступ легенды. |
| `kTypesPaddingY` | 16 | Вертикальный отступ легенды. |

## Методы

| Сигнатура | Описание |
|---|---|
| `verr GenerateDocument(const std::vector<uint8_t> &arrsrc, const std::string &fnamewnd, const std::string &strsha256, size_t filesize) const` | Публичная точка входа. Строит макет (полный лист → область бумаги → область 2×4 → легенда/пропсы), создаёт `VHSVG`, рисует сетку и легенду, завершает документ и сохраняет в `fnamewnd` через `VHSvgTools::Save()`. Параметры `strsha256`/`filesize` в текущей реализации не используются. Возвращает `vok` или ошибку сохранения. |
| `void Draw2X4Content(VHSVG &svg, const std::vector<uint8_t> &arrsrc, const VHSVGRect &rect) const` (private) | Рисует сетку 2×4: для каждой строки вычисляет `chunksize`, строит массив `VHBlockStat` и для каждой колонки рисует `DrawShadowBox` + `DrawSection` (кол. 0) или `DrawDensity` (кол. 1). |
| `static std::vector<VHBlockStat> GenSamplesArray(const std::vector<uint8_t> &arrsrc, size_t subsize)` (private) | Разбивает `arrsrc` на блоки по `subsize`, для каждого вызывает `VHBlockStat::scan()`. При `subsize == 0` или данных меньше одного блока возвращает пустой вектор. |

## Связи / Зависимости

- **Входящие:** `VHAppCore` (вызывает `GenerateDocument` из метода `GenerateWnd` с данными из собственного поля `datasource` и именем файла `fnameWND`).
- **Исходящие:** `VHSVG`/`VHSVGRect` (submodule `vhlibsvg`), `VHBlockStat`, класс `VHSVGDraw` (модуль `vhsvgdraw`, статические методы `DrawSection`/`DrawDensity`/`DrawTypes`/`DrawShadowBox`), `Colors` (`src/mods/colors/`), `VHSvgTools` (`src/mods/svgtools/`).

## Использование

```cpp
VHAppCore appcore;
// ... ParseArgs, LoadSource ...
if (vok != appcore.GenerateWnd())
    return verror(6);
```

Раньше `VHDocWnd` вызывался напрямую из `main.cpp` с локальным объектом
`VHDocWnd docwnd;`. После переноса генерации внутрь `VHAppCore`
вызов идёт через метод `GenerateWnd()`.

## Ограничения / Замечания

- Размеры чанков жёстко зашиты (`64 << row`); пользовательский `-wndsz`
  на генерацию WND-документа не влияет (см. [CMDLine.md](CMDLine.md)).
- Сохранение проверяется пост-фактум через `VHSvgTools::Save()` (файл
  непустой и заканчивается `</svg>`); `VHSVG::savetosvg()` по-прежнему
  печатает собственное сообщение об успехе до этой проверки.
- Область `rect_props` (правая половина подвала) зарезервирована, но не рисуется (`(void)rect_props`).
