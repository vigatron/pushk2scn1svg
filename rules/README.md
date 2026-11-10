# Классы проекта vhpack81scansvg

Индекс описаний классов. Каждому классу соответствует свой файл в этой папке.

| Файл | Класс | Модуль | Назначение |
|---|---|---|---|
| [VHAppCore.md](VHAppCore.md) | `VHAppCore` | `src/mods/vhappcore/` | Состояние анализа: загрузка, сканирования, генерация SVG. Конфиг — в поле `appcfg`. |
| [VHAppCoreCfg.md](VHAppCoreCfg.md) | `VHAppCoreCfg` | `src/mods/vhappcorecfg/` | Конфигурация приложения (имена файлов, `dirOutput`, `subsize`); требования к CLI — в `CMDLine.md`. |
| [VHSrcDataArray.md](VHSrcDataArray.md) | `VHSrcDataArray` | `src/mods/datasource/` | Загрузка исходного файла в память, SHA-256, размер файла. |
| [BlockProps.md](BlockProps.md) | `BlockProps` | `src/mods/structs/` | Статистика одного блока данных: min/max/уникальные значения, гистограмма. |
| [VHWideRangeScan.md](VHWideRangeScan.md) | `VHWideRangeScan` | `src/mods/widerange/` | Сканирование массива, отпечатки позиций 256 значений (abs/rel). |
| [VHKeyFingerPrint.md](VHKeyFingerPrint.md) | `VHKeyFingerPrint` | `src/mods/widerange/` | Битовый отпечаток позиций одного значения байта. |
| [VHLogger.md](VHLogger.md) | `VHLogger` | `src/mods/vhlogger/` | Глобальный отладочный вывод (текст, числа, hex-дампы памяти). |
| [VHDocWnd.md](VHDocWnd.md) | `VHDocWnd` | `src/mods/generatewnd/` | Генерация SVG «оконного» документа (сетка 2×4: Section + Density). |
| [VHDocDNA.md](VHDocDNA.md) | `VHDocDNA` | `src/mods/generatedna/` | Генерация SVG «DNA»-документа из wide-range сканирования (256 строк). |
| [Colors.md](Colors.md) | `Colors` | `src/mods/draw/` | Палитра цветов SVG-генераторов. |
| [VHSHA256.md](VHSHA256.md) | `VHSHA256` | `src/mods/vhsha256/` | Вычисление SHA-256 хеша массива байтов. |

## Прочие документы

| Файл | Назначение |
|---|---|
| [CMDLine.md](CMDLine.md) | Параметры командной строки: `-fname`, `-wndsz`, `-outfolder`; синтаксис, правила разбора, валидация, коды ошибок, формирование имён выходных файлов. |

## Структура зависимостей

```text
main.cpp
 └─ VHAppCore (LoadSource, ScanAbs/Rel, GenerateWnd, GenerateDNAAbs/Rel) ← vhappcore
     ├─ VHAppCoreCfg appcfg (ParseArgs)    ← vhappcorecfg
     ├─ VHSrcDataArray datasource          ← datasource
     │   └─ VHSHA256
     ├─ VHWideRangeScan (×2)               ← widerange
     │   └─ VHKeyFingerPrint (×256)
     └─ VHDocWnd / VHDocDNA                ← generatewnd / generatedna
 └─ VHDocWnd                       ← generatewnd
     └─ BlockProps                 ← structs
     └─ draw: DrawSection/DrawDensity/DrawTypes/Colors ← draw
 └─ VHDocDNA                       ← generatedna
     └─ VHWideRangeScan + VHKeyFingerPrint
 └─ VHLogger (глобальный dbg)         ← vhlogger / global
```

## Замечания к документации

- Классы из `src/extmods/` (submodule `vhlibplatform`, `vhlibargs`, `vhlibfsys`, `vhlibsvg` — `VHArgsParser`, `VHFile`, `VHSVG` и т.д.) **не документируются** в этой папке — они только для чтения.
- Описания фиксируют фактическое поведение кода, включая ограничения
  (неинициализированные поля, unchecked-индексы, глобальный `VHLogger`).
  После исправления дефектов стоит обновить разделы «Ограничения / Замечания».
