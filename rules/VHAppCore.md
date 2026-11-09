# VHAppCore

## Файл / Модуль

- Заголовок: `src/mods/vhappcore/vhappcore.hpp`
- Реализация методов: `src/mods/vhappcore/vhappcore.cpp`
- Класс: `class VHAppCore`

## Назначение

Центральный объект состояния анализа приложения.
Объединяет конфигурацию (`VHAppCoreCfg appcfg`), исходные данные
(через `VHSrcDataArray`) и результаты двух сканирований
(абсолютного и относительного) wide-range анализа.
Объект создаётся в `main.cpp`, заполняется методом
`appcfg.ParseArgs()` из `VHAppCoreCfg` и методами `LoadAndScan()`,
`GenerateWnd()`, `GenerateDNAAbs()` и `GenerateDNARel()`.

## Ответственность

- Хранение конфигурации приложения (`appcfg` — см. `VHAppCoreCfg`).
- Загрузка исходного файла в память (`datasource`) и проверка минимального размера.
- Выполнение двух wide-range сканирований (`wscanabs`, `wscanrel`) над загруженными данными.
- Генерация WND-документа (`GenerateWnd` через `VHDocWnd`).
- Генерация DNA-документов (`GenerateDNAAbs` / `GenerateDNARel` через `VHDocDNA`).
- Хранение результатов сканирований до генерации документов.

## Поля / Данные

| Поле | Тип | Назначение |
|---|---|---|
| `appcfg` | `VHAppCoreCfg` | Конфигурация приложения (имена файлов, `subsize`, парсинг командной строки). |
| `datasource` | `VHSrcDataArray` | Исходные данные: загруженный файл, SHA-256, размер файла. Владелец буфера данных (`arrsrc`), живёт пока жив `VHAppCore`. |
| `wscanabs` | `VHWideRangeScan` | Результат сканирования в абсолютных позициях. |
| `wscanrel` | `VHWideRangeScan` | Результат сканирования в относительных смещениях. |

## Методы

| Сигнатура | Описание |
|---|---|
| `verr LoadAndScan()` | Загружает исходный файл в `datasource` через `datasource.LoadSource(appcfg.fnameSource, appcfg.subsize)` (требуется предварительный вызов `appcfg.ParseArgs`). Проверяет размер загруженных данных (`>= appcfg.subsize`), затем выполняет `wscanabs.scanabs(arrsrc)` и `wscanrel.scanrel(arrsrc)`. Возвращает `vok` или ошибку (`verror(1)` — файл слишком мал, `verrmsg(1, ...)` — ошибка abs/rel сканирования). |
| `verr GenerateWnd()` | Генерирует WND SVG-документ через `VHDocWnd::GenerateDocument(datasource.data(), appcfg.fnameWND, datasource.strSHA256(), datasource.filesize())` (требуется предварительный вызов `LoadAndScan`). Возвращает `vok` или ошибку (`verrmsg(1, "Error generating docwnd")`). |
| `verr GenerateDNAAbs()` | Генерирует DNA ABS SVG-документ через `VHDocDNA::GenerateDocument("DNA ABS", wscanabs, appcfg.fnameSource, appcfg.fnameDNAAbs, datasource.strSHA256(), datasource.filesize())` (требуется предварительный вызов `LoadAndScan`). Возвращает `vok` или ошибку (`verrmsg(2, "Error generating docdna abs")`). |
| `verr GenerateDNARel()` | Генерирует DNA REL SVG-документ через `VHDocDNA::GenerateDocument("DNA REL", wscanrel, appcfg.fnameSource, appcfg.fnameDNARel, datasource.strSHA256(), datasource.filesize())` (требуется предварительный вызов `LoadAndScan`). Возвращает `vok` или ошибку (`verrmsg(2, "Error generating docdna rel")`). |

## Связи / Зависимости

- **Входящие:** `main.cpp`, `VHDocDNA` (косвенно, через поле `wscan*`).
- **Исходящие:** `VHAppCoreCfg` (`appcfg` — конфигурация и парсинг), `VHSrcDataArray` (`datasource` — загрузка исходных данных), `VHWideRangeScan` (`wscanabs`, `wscanrel`), `VHDocWnd` (генерация WND-документа), `VHDocDNA` (генерация DNA-документов), `vhplatform` (`verr`/`vok`).
- Реализация зависит от `generatewnd/generatewnd.hpp` и `generatedna/generatedna.hpp`.

## Использование

```cpp
VHAppCore appcore;
if (vok != appcore.appcfg.ParseArgs(argc, argv))
    return verror(1);
if (vok != appcore.LoadAndScan())
    return verror(1);
if (vok != appcore.GenerateWnd())
    return verror(1);
if (vok != appcore.GenerateDNAAbs())
    return verror(1);
if (vok != appcore.GenerateDNARel())
    return verror(1);
```

До переноса `ParseArgs` внутрь класса это была свободная функция
`verr ParseArgs(int argc, char *argv[], VHAppCore &appcfg)` из модуля
`src/mods/cmdline/`. После переноса папка `cmdline/` удалена.

До переноса загрузки и сканирования внутрь класса этот блок жил в `main.cpp`
с локальным `VHSrcDataArray datasource;`. После переноса `datasource` —
поле `VHAppCore`, а загрузка и сканирования — метод `LoadAndScan()`.

До переноса генерации WND-документа внутрь класса блок с локальным
`VHDocWnd docwnd;` жил в `main.cpp`. После переноса генерация —
метод `VHAppCore::GenerateWnd()` (реализация в `vhappcore/vhappcore.cpp`).

До переноса генерации DNA-документов внутрь класса оба блока с локальным
`VHDocDNA docdna;` жили в `main.cpp`. После переноса генерация —
методы `VHAppCore::GenerateDNAAbs()` и `VHAppCore::GenerateDNARel()`
(реализация в `vhappcore/vhappcore.cpp`).

Конфигурация (`fnameSource`, `fnameWND`, `fnameDNAAbs`, `fnameDNARel`,
`subsize`) и парсинг командной строки (`ParseArgs`) выделены из `VHAppCore`
в отдельный класс `VHAppCoreCfg` (`vhappcorecfg/`). В `VHAppCore` конфиг
хранится как поле `VHAppCoreCfg appcfg;`, вызов — `appcore.appcfg.ParseArgs(...)`.

## Ограничения / Замечания

- Класс-классификация соблюдает правило `AGENTS.md`: файлы класса находятся в собственной папке `src/mods/vhappcore/`.
- `appcfg.subsize` в текущей логике используется только как минимальный размер
  загрузки источника (`VHSrcDataArray::LoadSource`); размеры чанков при
  генерации WND-документа задаются константами `VHDocWnd` (`kBaseChunkSize << row`).
