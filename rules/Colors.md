# Colors

## Файл / Модуль

- Заголовок: `src/mods/colors/colors.hpp`
- Реализация: `src/mods/colors/colors.cpp`
- Класс: `class Colors`

## Назначение

Палитра SVG-генераторов: набор именованных цветов (в формате hex-строк `#RRGGBB`
или именованных цветов SVG) для фона колонок, распространения, ширины, теней,
текста, а также рамок/линий DNA- и WND-документов. Используется классами
`VHSVGDraw` (модуль `vhsvgdraw`), `VHDocDNA` (модуль `generatedna`) и
`VHDocWnd` (модуль `generatewnd`).

## Ответственность

- Хранение цветовых констант палитры в виде статических строк и массива `colordensity`.
- Документирование палитры в комментариях (белый → зелёный → жёлтый → красный градиент).

## Поля / Данные

Все поля — `static std::string` (кроме `colordensity` — `static const uint32_t[4]`),
инициализируются в `colors.cpp`.

| Поле | Значение | Назначение |
|---|---|---|
| `clrbck1` | `#F0F0F0` | Светлый фон колонок (чередование); фон rect футера DNA-документа. |
| `clrbck2` | `#F4F4F4` | Тёмный фон колонок (чередование). |
| `clrspr` | `#ccd3e7` | Цвет распространения (min..max). |
| `clrmin` | `#2020FF` | Цвет минимума (в текущей отрисовке не используется). |
| `clrminl` | `#D0D0FF` | Светлый минимум (не используется). |
| `clrmax` | `#2020FF` | Цвет максимума (не используется). |
| `clrmaxl` | `#D0D0FF` | Светлый максимум (не используется). |
| `clrwdt` | `#6090FF` | Цвет ширины распределения (`wght`). |
| `clrshd_w` | `#E8E8E8` | Тень: обводка. |
| `clrshd_bb` | `#F8F8F8` | Тень: фон. |
| `clrshd_f` | `#DADADA` | Тень: обводка смещённого прямоугольника. |
| `clrshd_b` | `#F0F0F0` | Тень: фон смещённого прямоугольника. |
| `clrheadertxt` | `black` | Текст заголовка/футера и подписей DNA-документа. |
| `clrempty` | `#EEEEEE` | DNA: текст/рамка/фон «пустой» ячейки (объединяет прежние `COLOR_EMPTY_CELL`/`COLOR_EMPTY_BACK`). |
| `clrcellalt1` | `#8888FF` | DNA: линии ячеек при чётном `dx`. |
| `clrcellalt2` | `#6666FF` | DNA: линии ячеек при нечётном `dx`. |
| `clrline` | `gray` | Общий цвет линий/обводок (направляющие, рамки DNA/WND/Types). |
| `clrpaper` | `white` | Заливка бумажного поля документа. |
| `clrframe` | `#AAAAAA` | Обводки рамок Scale/Section. |
| `clrscalealt1` | `#D0D0D0` | Чередующийся фон Scale (нечётные полосы). |
| `clrscalealt2` | `#E0E0E0` | Чередующийся фон Scale (чётные полосы). |
| `clrouterframe` | `#CCCCCC` | Внешняя рамка Section. |
| `clrmarker` | `red` | Маркерные линии гистограммы в Density. |
| `clrseltext` | `green` | Текст выбранной ячейки в Types. |
| `clrselfill` | `#D0FFD0` | Заливка выбранной ячейки в Types. |
| `colordensity` | `{0xFFFFFF, 0x80E080, 0xFFFF00, 0xFF8080}` | Градиент плотности Density (бел.→зел.→жёлт.→красн.), используется `lincolor()`. |
| `colortxt` | `#777777` | Основной цвет текста. |
| `colortxtsh` | `#A7A7A7` | Тень текста (частично не используется). |
| `colortxtsh2` | `#C7C7C7` | Светлая тень текста (подложка под заголовком в Section). |

## Методы

Методов нет — класс только хранит данные.

## Связи / Зависимости

- **Входящие:** `vhsvgdraw/vhsvgdraw.cpp` (`VHSVGDraw`: Section/Scale/Types/Density), `generatedna/generatedna.cpp` (`VHDocDNA`: header/paper/ячейки), `generatewnd/generatewnd.cpp` (`VHDocWnd`: внешнее поле документа).
- **Исходящие:** только `std::string` и `uint32_t` (`colordensity`).

## Использование

```cpp
svg.rect(rect, 0, Colors::clrbck1, Colors::clrbck1);
svg.text(x, y, "label", "verdana", 32, Colors::colortxt);
```

## Ограничения / Замечания

- Поля мутабельны (`static std::string` без `const`; исключение — `colordensity`) — палитру можно случайно изменить из любого места.
- Часть полей (`clrmin`, `clrminl`, `clrmax`, `clrmaxl`, `colortxtsh`) объявлена, но в текущей отрисовке не используется.
- `"none"` (SVG paint-значение «без заливки/обводки») намеренно не вынесен в `Colors` — это не цвет.
- Файловые константы `COLOR_*` модуля `generatedna` устранены; все цвета задаются только через `Colors`.
- Класс не имеет конструктора/методов — в современном коде обычно заменяется на `constexpr const char*` константы или namespace.
