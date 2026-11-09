# VHAppCore

## Файл / Модуль

- Заголовок: `src/mods/cfg/appcfgstruct.hpp`
- Реализация метода парсинга: `src/mods/cfg/appcfg.cpp`
- Класс: `class VHAppCore`

## Назначение

Центральный объект конфигурации приложения и состояния анализа.
Объединяет в себе настройки командной строки, имена входных/выходных файлов,
исходные данные (через `VHSrcDataArray`) и результаты двух сканирований
(абсолютного и относительного) wide-range анализа.
Объект создаётся в `main.cpp`, заполняется методами `ParseArgs()`,
`LoadAndScan()`, `GenerateWnd()`, `GenerateDNAAbs()` и `GenerateDNARel()`.

## Ответственность

- Парсинг командной строки и валидация входных данных.
- Формирование имён выходных SVG-файлов из имени исходного файла.
- Хранение размера окна сканирования (`subsize`).
- Загрузка исходного файла в память (`datasource`) и проверка минимального размера.
- Выполнение двух wide-range сканирований (`wscanabs`, `wscanrel`) над загруженными данными.
- Генерация WND-документа (`GenerateWnd` через `VHDocWnd`).
- Генерация DNA-документов (`GenerateDNAAbs` / `GenerateDNARel` через `VHDocDNA`).
- Хранение результатов сканирований до генерации документов.

## Поля / Данные

| Поле | Тип | Назначение |
|---|---|---|
| `fnameSource` | `std::string` | Путь к исходному бинарному файлу (`-fname`). |
| `fnameWND` | `std::string` | Путь к выходному SVG «оконного» документа (`<base>_wnd.svg`). |
| `fnameDNAAbs` | `std::string` | Путь к выходному SVG DNA-отпечатка абс. (`<base>_dna_abs.svg`). |
| `fnameDNARel` | `std::string` | Путь к выходному SVG DNA-отпечатка отн. (`<base>_dna_rel.svg`). |
| `subsize` | `uint16_t` | Размер окна сканирования (`-wndsz`, диапазон 2..256, по умолчанию 128). |
| `datasource` | `VHSrcDataArray` | Исходные данные: загруженный файл, SHA-256, размер файла. Владелец буфера данных (`arrsrc`), живёт пока жив `VHAppCore`. |
| `wscanabs` | `VHWideRangeScan` | Результат сканирования в абсолютных позициях. |
| `wscanrel` | `VHWideRangeScan` | Результат сканирования в относительных смещениях. |

## Методы

| Сигнатура | Описание |
|---|---|
| `verr ParseArgs(int argc, char *argv[])` | Парсит командную строку через `VHArgsParser::VHCmdLineParser` (`vhlibargs`). Проверяет: синтаксис парсера, непустое имя файла `-fname`, существование файла (`VHFSYS::FileExist`). Формирует имена выходных файлов через `VHFSYS::ExtractFilename()` и присваивает `subsize` из `-wndsz`. Возвращает `vok` или ошибку (`verrmsg(1, ...)`, `verrmsg(3, ...)`). |
| `verr LoadAndScan()` | Загружает исходный файл в `datasource` через `datasource.LoadSource(fnameSource, subsize)` (требуется предварительный вызов `ParseArgs`). Проверяет размер загруженных данных (`>= subsize`), затем выполняет `wscanabs.scanabs(arrsrc)` и `wscanrel.scanrel(arrsrc)`. Возвращает `vok` или ошибку (`verror(1)` — файл слишком мал, `verrmsg(1, ...)` — ошибка abs/rel сканирования). |
| `verr GenerateWnd()` | Генерирует WND SVG-документ через `VHDocWnd::GenerateDocument(datasource.data(), fnameWND, datasource.strSHA256(), datasource.filesize())` (требуется предварительный вызов `LoadAndScan`). Возвращает `vok` или ошибку (`verrmsg(1, "Error generating docwnd")`). |
| `verr GenerateDNAAbs()` | Генерирует DNA ABS SVG-документ через `VHDocDNA::GenerateDocument("DNA ABS", wscanabs, fnameSource, fnameDNAAbs, datasource.strSHA256(), datasource.filesize())` (требуется предварительный вызов `LoadAndScan`). Возвращает `vok` или ошибку (`verrmsg(2, "Error generating docdna abs")`). |
| `verr GenerateDNARel()` | Генерирует DNA REL SVG-документ через `VHDocDNA::GenerateDocument("DNA REL", wscanrel, fnameSource, fnameDNARel, datasource.strSHA256(), datasource.filesize())` (требуется предварительный вызов `LoadAndScan`). Возвращает `vok` или ошибку (`verrmsg(2, "Error generating docdna rel")`). |

## Связи / Зависимости

- **Входящие:** `main.cpp`, `VHDocDNA` (косвенно, через поле `wscan*`).
- **Исходящие:** `VHSrcDataArray` (`datasource` — загрузка исходных данных), `VHWideRangeScan` (`wscanabs`, `wscanrel`), `VHDocWnd` (генерация WND-документа), `VHDocDNA` (генерация DNA-документов), `VHArgsParser` (парсинг), `VHFSYS` (проверка существования файла, извлечение имени), `vhplatform` (`verr`/`vok`).
- Реализация зависит от `vhfile.hpp` / `vhfsys.hpp` (submodule `vhlibfsys`), `generatewnd/generatewnd.hpp` и `generatedna/generatedna.hpp`.

## Использование

```cpp
VHAppCore appcore;
if (vok != appcore.ParseArgs(argc, argv))
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
метод `VHAppCore::GenerateWnd()` (реализация в `cfg/appcfg.cpp`).

До переноса генерации DNA-документов внутрь класса оба блока с локальным
`VHDocDNA docdna;` жили в `main.cpp`. После переноса генерация —
методы `VHAppCore::GenerateDNAAbs()` и `VHAppCore::GenerateDNARel()`
(реализация в `cfg/appcfg.cpp`).

## Ограничения / Замечания

- Класс смешивает конфигурацию (имена файлов) и состояние анализа (сканирования) — при расширении стоит разделить.
- `subsize` в текущей логике используется только как минимальный размер
  загрузки источника (`VHSrcDataArray::LoadSource`); размеры чанков при
  генерации WND-документа задаются константами `VHDocWnd` (`kBaseChunkSize << row`).
- Метод `ParseArgs` зависит от конкретной реализации `vhlibargs` — тестирование возможно только через передачу `argc/argv`.
