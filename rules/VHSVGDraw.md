# VHSVGDraw

## Файл / Модуль

- Заголовок: `src/mods/vhsvgdraw/vhsvgdraw.hpp`
- Реализация: `src/mods/vhsvgdraw/vhsvgdraw.cpp`
- Класс: `class VHSVGDraw`

## Назначение

Набор stateless-помощников отрисовки элементов SVG «оконного» документа (WND):
направляющие линии, теневая рамка, шкала, секционный и плотностной виды
блоков, легенда типов данных. Используется генератором `VHDocWnd`.

## Ответственность

- Отрисовка примитивов и составных элементов WND-листа поверх `VHSVG`.
- Инкапсуляция вспомогательных процедур и локальных данных, ранее
  распределённых по модулю `draw/` (цветовые преобразования для Density,
  список типов легенды).

## Публичный API (статические методы)

| Метод | Описание |
|---|---|
| `static void DrawCenterLine(VHSVG &svg, const VHSVGRect &rect)` | Рисует центральную и четвертные горизонтальные направляющие. |
| `static void DrawShadowBox(VHSVG &svg, const VHSVGRect &rect)` | Рисует прямоугольник с тенью. |
| `static void DrawScale(VHSVG &svg, const VHSVGRect &rect, int size)` | Рисует шкалу (полосы деления и подписи) в заданной области. |
| `static void DrawSection(VHSVG &svg, const VHSVGRect &rect, const std::vector<BlockProps> &arr, int blksize, int ws)` | Рисует секционный вид: фон колонок, min/max-размах и «ширину» (`wght`), рамки, подпись окна, шкалу. |
| `static void DrawDensity(VHSVG &svg, const VHSVGRect &rect, const std::vector<BlockProps> &arr, int blksize, int ws)` | Рисует плотностной вид: диапазон блока и отметки значений гистограммы, направляющие, дельта-полосы, шкалу. |
| `static void DrawTypes(VHSVG &svg, const VHSVGRect &rect)` | Рисует легенду типов данных (сетка 7×4, 28 наименований) с выделением выбранного элемента. |

## Внутренние (private static) элементы

- **Section:** `draw_column_spread`, `draw_col_bck`.
- **Density:** `rgb_to_compo`, `scaleval`, `hexval`, `strrgb` (перегрузки), `lincolor`, `draw_column_mask`; палитра — `Colors::colordensity` (`0xFFFFFF, 0x80E080, 0xFFFF00, 0xFF8080`).
- **Types:** `types()` (список имён типов), `gettype(i, j)`.

## Связи / Зависимости

- **Входящие:** `VHDocWnd` (`src/mods/generatewnd/`) — вызывает `DrawShadowBox`, `DrawSection`, `DrawDensity`, `DrawTypes`.
- **Исходящие:** `VHSVG`/`VHSVGRect` (`vhlibsvg`, extmod), `BlockProps` (`src/mods/structs/`), `Colors` (`src/mods/colors/`), утилиты `VHSVGTextTools::szhex()` (`vhlibsvg`), `global.hpp`.

## Ограничения / Замечания

- Класс не хранит состояние; все методы — `static` (вариант A объединения бывшего модуля `draw/`).
- Поведение перенесено из прежних свободных функций без изменения алгоритмов; известные особенности рендеринга (параметры `blksize`/`ws` для шкалы и подписи, использование гистограммы в Density, порядок индексации типов 7×4, шрифты `verdana`/`noto`) сохранены и отслеживаются в [README.md](../README.md) (§10, §14).
- Палитра Density хранится в классе `Colors` (`colordensity`) и используется в `lincolor()`; независимых палитр больше нет.
