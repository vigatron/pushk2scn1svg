# VHAppCoreCfg

## Файл / Модуль

- Заголовок: `src/mods/vhappcorecfg/vhappcorecfg.hpp`
- Реализация: `src/mods/vhappcorecfg/vhappcorecfg.cpp`
- Класс: `class VHAppCoreCfg`

## Назначение

Конфигурация приложения: параметры командной строки и имена входных/выходных
файлов. Специально выделен из `VHAppCore` для парсинга командной строки
(`ParseArgs`) — конфиг отделён от состояния анализа (исходных данных
и результатов сканирований).

## Ответственность

- Парсинг командной строки и валидация входных данных.
- Формирование имён выходных SVG-файлов из имени исходного файла.
- Хранение размера окна сканирования (`subsize`).

## Поля / Данные

| Поле | Тип | Назначение |
|---|---|---|
| `fnameSource` | `std::string` | Путь к исходному бинарному файлу (`-fname`). |
| `fnameWND` | `std::string` | Путь к выходному SVG «оконного» документа (`<base>_wnd.svg`). |
| `fnameDNAAbs` | `std::string` | Путь к выходному SVG DNA-отпечатка абс. (`<base>_dna_abs.svg`). |
| `fnameDNARel` | `std::string` | Путь к выходному SVG DNA-отпечатка отн. (`<base>_dna_rel.svg`). |
| `subsize` | `uint16_t` | Размер окна сканирования (`-wndsz`, диапазон 2..256, по умолчанию 128). |

## Методы

| Сигнатура | Описание |
|---|---|
| `verr ParseArgs(int argc, char *argv[])` | Парсит командную строку через `VHArgsParser::VHCmdLineParser` (`vhlibargs`). Проверяет: синтаксис парсера, непустое имя файла `-fname`, существование файла (`VHFSYS::FileExist`). Формирует имена выходных файлов через `VHFSYS::ExtractFilename()` и присваивает `subsize` из `-wndsz`. Возвращает `vok` или ошибку (`verrmsg(1, ...)`, `verrmsg(3, ...)`). |

## Связи / Зависимости

- **Входящие:** `VHAppCore` (владеет объектом как поле `appcfg`, методы ядра читают поля конфига), `main.cpp` (косвенно, через `appcore.appcfg.ParseArgs`).
- **Исходящие:** `VHArgsParser` (парсинг), `VHFSYS` (проверка существования файла, извлечение имени), `vhplatform` (`verr`/`vok`).
- Реализация зависит от `vhfile.hpp` / `vhfsys.hpp` (submodule `vhlibfsys`) и `argsparser.hpp` (submodule `vhlibargs`).

## Использование

```cpp
VHAppCore appcore;
if (vok != appcore.appcfg.ParseArgs(argc, argv))
    return verror(1);
```

## Ограничения / Замечания

- Поля конфига — публичные, заполняются только через `ParseArgs`.
- `subsize` используется как минимальный размер загрузки источника
  (`VHSrcDataArray::LoadSource`); размеры чанков при генерации
  WND-документа задаются константами `VHDocWnd` (`kBaseChunkSize << row`).
- Метод `ParseArgs` зависит от конкретной реализации `vhlibargs` — тестирование возможно только через передачу `argc/argv`.
