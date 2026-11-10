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
`appcfg.ParseArgs()` из `VHAppCoreCfg`, затем методами `LoadSource()`,
`ScanAbs()` / `ScanRel()` и методами `GenerateWnd()`, `GenerateDNAAbs()`,
`GenerateDNARel()`.

## Ответственность

- Хранение конфигурации приложения (`appcfg` — см. `VHAppCoreCfg`).
- Загрузка исходного файла в память (`LoadSource`, поле `datasource`) и проверка минимального и максимального размера.
- Выполнение абсолютного (`ScanAbs`) и относительного (`ScanRel`) wide-range сканирований; `ScanAll` — удобная обёртка над обоими.
- Генерация WND-документа (`GenerateWnd` через `VHDocWnd`) — требует только загруженных данных, без сканирований.
- Генерация DNA-документов (`GenerateDNAAbs` / `GenerateDNARel` через `VHDocDNA`).
- Хранение результатов сканирований до генерации документов.

## Поля / Данные

| Поле | Тип | Назначение |
|---|---|---|
| `appcfg` | `VHAppCoreCfg` | Конфигурация приложения (имена файлов, `dirOutput`, `subsize`, парсинг командной строки — см. `CMDLine.md`). |
| `datasource` | `VHSrcDataArray` | Исходные данные: загруженный файл, SHA-256, размер файла. Владелец буфера данных (`arrsrc`), живёт пока жив `VHAppCore`. |
| `wscanabs` | `VHWideRangeScan` | Результат сканирования в абсолютных позициях. |
| `wscanrel` | `VHWideRangeScan` | Результат сканирования в относительных смещениях. |

## Методы

| Сигнатура | Описание |
|---|---|
| `verr LoadSource()` | Загружает исходный файл в `datasource` через `datasource.LoadSource(appcfg.fnameSource, appcfg.subsize)` (требуется предварительный вызов `appcfg.ParseArgs`). Проверяет размер загруженных данных (`>= appcfg.subsize`); верхняя граница (`MAX_SOURCE_SIZE` = 32 768 байт) проверяется внутри `VHSrcDataArray::LoadSource`. Возвращает `vok`, `verror(3)` — файл не загружен, слишком мал или слишком велик. |
| `verr ScanAbs()` | Проверяет, что данные загружены, и выполняет `wscanabs.scanabs(datasource.data())`. Возвращает `vok`, `verrmsg(4, "Source data is not loaded")` или `verrmsg(4, "Wide Range (abs) analisys failed")`. |
| `verr ScanRel()` | Проверяет, что данные загружены, и выполняет `wscanrel.scanrel(datasource.data())`. Возвращает `vok`, `verrmsg(5, "Source data is not loaded")` или `verrmsg(5, "Wide Range (rel) analisys failed")`. |
| `verr ScanAll()` | Удобная обёртка: вызывает `ScanAbs()`, затем `ScanRel()`. Если ABS-скан не удался, REL не запускается. Возвращает первый полученный код ошибки. |
| `verr GenerateWnd()` | Проверяет, что данные загружены (`datasource.data()` непустой), затем генерирует WND SVG-документ через `VHDocWnd::GenerateDocument(datasource.data(), appcfg.fnameWND, datasource.strSHA256(), datasource.filesize())` (сканирования не нужны). Возвращает `vok`, `verrmsg(6, "Source data is not loaded")` или `verrmsg(6, "Error generating docwnd")`. |
| `verr GenerateDNAAbs()` | Проверяет, что ABS-скан выполнен и его ширина совпадает с размером загруженного файла (`wscanabs.width() != 0 && wscanabs.width() == datasource.filesize()`), затем генерирует DNA ABS SVG-документ через `VHDocDNA::GenerateDocument(...)`. Возвращает `vok`, `verrmsg(7, "ABS scan is not available")` или `verrmsg(7, "Error generating docdna abs")`. |
| `verr GenerateDNARel()` | Проверяет, что REL-скан выполнен и его ширина совпадает с размером загруженного файла (`wscanrel.width() != 0 && wscanrel.width() == datasource.filesize()`), затем генерирует DNA REL SVG-документ через `VHDocDNA::GenerateDocument(...)`. Возвращает `vok`, `verrmsg(8, "REL scan is not available")` или `verrmsg(8, "Error generating docdna rel")`. |

## Связи / Зависимости

- **Входящие:** `main.cpp`, `VHDocDNA` (косвенно, через поле `wscan*`).
- **Исходящие:** `VHAppCoreCfg` (`appcfg` — конфигурация и парсинг), `VHSrcDataArray` (`datasource` — загрузка исходных данных), `VHWideRangeScan` (`wscanabs`, `wscanrel`), `VHDocWnd` (генерация WND-документа), `VHDocDNA` (генерация DNA-документов), `vhplatform` (`verr`/`vok`).
- Реализация зависит от `generatewnd/generatewnd.hpp` и `generatedna/generatedna.hpp`.

## Использование

```cpp
VHAppCore appcore;
if (vok != appcore.appcfg.ParseArgs(argc, argv))
    return verror(1);
if (vok != appcore.LoadSource())
    return verror(3);
if (vok != appcore.ScanAbs())
    return verror(4);
if (vok != appcore.ScanRel())
    return verror(5);
if (vok != appcore.GenerateWnd())
    return verror(6);
if (vok != appcore.GenerateDNAAbs())
    return verror(7);
if (vok != appcore.GenerateDNARel())
    return verror(8);
```

Парсинг командной строки и его параметры вынесены в `VHAppCoreCfg`
(`appcfg.ParseArgs`); синтаксис, правила разбора и коды ошибок описаны
в [CMDLine.md](CMDLine.md).

До переноса загрузки и сканирования внутрь класса этот блок жил в `main.cpp`
с локальным `VHSrcDataArray datasource;`. После переноса `datasource` —
поле `VHAppCore`. Изначально загрузка и оба сканирования выполнялись одним
методом `LoadAndScan()`; затем он разделён на `LoadSource()`, `ScanAbs()`
и `ScanRel()` (плюс обёртка `ScanAll()`), чтобы фазы можно было вызывать
и диагностировать независимо. `GenerateWnd()` теперь требует только
`LoadSource()` и не зависит от результатов сканирований.

До переноса генерации WND-документа внутрь класса блок с локальным
`VHDocWnd docwnd;` жил в `main.cpp`. После переноса генерация —
метод `VHAppCore::GenerateWnd()` (реализация в `vhappcore/vhappcore.cpp`).

До переноса генерации DNA-документов внутрь класса оба блока с локальным
`VHDocDNA docdna;` жили в `main.cpp`. После переноса генерация —
методы `VHAppCore::GenerateDNAAbs()` и `VHAppCore::GenerateDNARel()`
(реализация в `vhappcore/vhappcore.cpp`).

Конфигурация (`fnameSource`, `fnameWND`, `fnameDNAAbs`, `fnameDNARel`,
`dirOutput`, `subsize`) и парсинг командной строки (`ParseArgs`) выделены
из `VHAppCore` в отдельный класс `VHAppCoreCfg` (`vhappcorecfg/`).
В `VHAppCore` конфиг хранится как поле `VHAppCoreCfg appcfg;`, вызов —
`appcore.appcfg.ParseArgs(...)`. Требования к CLI — в `CMDLine.md`.

## Ограничения / Замечания

- Класс-классификация соблюдает правило `AGENTS.md`: файлы класса находятся в собственной папке `src/mods/vhappcore/`.
- Класс не хранит отдельные флаги успешности фаз. Вместо них проверяются
  фактические данные: `GenerateWnd` требует непустой `datasource.data()`,
  `GenerateDNAAbs`/`GenerateDNARel` — скан с шириной, равной размеру файла.
  Порядок `LoadSource` → `ScanAbs`/`ScanRel` → `Generate*` остаётся
  ответственностью вызывающего кода.
- `appcfg.subsize` в текущей логике используется только как минимальный размер
  загрузки источника (`VHSrcDataArray::LoadSource`); размеры чанков при
  генерации WND-документа задаются константами `VHDocWnd` (`kBaseChunkSize << row`).
