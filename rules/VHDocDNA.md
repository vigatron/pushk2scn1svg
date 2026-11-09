# VHDocDNA

## Файл / Модуль

- Заголовок: `src/mods/generatedna/generatedna.hpp`
- Реализация: `src/mods/generatedna/generatedna.cpp`
- Класс: `class VHDocDNA`

## Назначение

Генерация SVG-документа «ДНК» (DNA) из результата wide-range сканирования
(`VHWideRangeScan`): визуализация для каждого из 256 значений байта
строки установленных позиций (битовых линий). Используется дважды —
для абсолютного сканирования (`DNA ABS`) и относительного (`DNA REL`).

## Ответственность

- Расчёт макета документа: заголовок, тело, подвал.
- Отрисовка заголовка (подпись), подвала (имя файла, размер, SHA-256, дата/время) и тела (256 строк химически-отпечатков).
- Сборка документа через внутренний объект `VHSVG` и сохранение в файл.

## Поля / Данные

| Имя | Тип | Назначение |
|---|---|---|
| `doc_size_w` | `static const uint16_t = 2400` | Ширина документа SVG. |
| `doc_size_h` | `static const uint16_t = 2600` | Высота документа SVG. |
| `doc_content_dist_w` | `static const uint16_t = 240` | Горизонтальный отступ области контента. |
| `doc_content_dist_h` | `static const uint16_t = 200` | Вертикальный отступ области контента. |
| `svg` | `VHSVG` | Внутренний объект генерации SVG (состояние на время генерации). |

## Методы

| Сигнатура | Описание |
|---|---|
| `verr GenerateDocument(const std::string &caption, const VHWideRangeScan &range, const std::string &fname, const std::string &fnamedna, const std::string &strsha256, size_t filesize)` | Публичная точка входа. Создаёт области листа/бумаги, вычисляет макет, рисует заголовок и подвал, вызывает `Gen256Stat`, завершает SVG и сохраняет. Возвращает `vok`. |
| `void GenerateLayout(const VHSVGRect &rect, VHSVGRect &recthdr, VHSVGRect &rectbody, VHSVGRect &rectftr)` (private) | Рассчитывает прямоугольники заголовка (высота 120), тела и подвала (высота 120). |
| `void GenerateHeader(const std::string &caption, const VHSVGRect &rect)` (private) | Рисует заголовок: текст подписи (Arial 48) и рамку. |
| `void GenerateFooter(const VHSVGRect &rect, const std::string &filename, size_t filesize, const std::string &strsha256)` (private) | Рисует подвал: имя файла, размер, SHA-256, дату/время (`%d-%b-%Y %H:%M`, `localtime_r`/`localtime_s`). |
| `void Gen256Stat(VHSVG &svg, const VHSVGRect &rect, const VHWideRangeScan &wscan)` (private) | Рисует 256 строк (по одной на значение байта): фон строки, вертикальные линии для установленных битов отпечатка (`obj.status(pos)`), номер `#N` слева. При пустом скане (`width()==0`) ничего не рисует. |

## Связи / Зависимости

- **Входящие:** `VHAppCore` (вызывает `GenerateDocument` из методов `GenerateDNAAbs` / `GenerateDNARel` с данными из собственных полей `datasource`, `wscanabs`/`wscanrel` и имён файлов `fnameSource`/`fnameDNAAbs`/`fnameDNARel`).
- **Исходящие:** `VHSVG`/`VHSVGRect` (submodule `vhlibsvg`), `VHWideRangeScan`, `VHKeyFingerPrint`, `vhplatform` (`verr`/`vok`).

## Использование

```cpp
VHAppCore appcore;
// ... ParseArgs, LoadAndScan ...
if (vok != appcore.GenerateDNAAbs())
    return verror(1);
if (vok != appcore.GenerateDNARel())
    return verror(1);
```

Раньше `VHDocDNA` вызывался напрямую из `main.cpp` с локальным объектом
`VHDocDNA docdna;` (дважды — для `wscanabs` и `wscanrel`). После переноса
генерации внутрь `VHAppCore` вызовы идут через методы
`GenerateDNAAbs()` / `GenerateDNARel()`.

## Ограничения / Замечания

- `svg` — член класса: объект одноразовый, повторный вызов перезапишет документ.
- В `Gen256Stat` для каждого символа читаются `getStartPos()`/`getEndPos()` безусловна — для пустых отпечатков значения зависят от инициализации `VHKeyFingerPrint` (см. замечания в `VHKeyFingerPrint.md`).
- Ошибка сохранения SVG не проверяется.
