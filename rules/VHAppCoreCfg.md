# VHAppCoreCfg

## Файл / Модуль

- Заголовок: `src/mods/vhappcorecfg/vhappcorecfg.hpp`
- Реализация: `src/mods/vhappcorecfg/vhappcorecfg.cpp`
- Класс: `class VHAppCoreCfg`

## Назначение

Конфигурация приложения: имена входных/выходных файлов, папка вывода и
размер окна сканирования. Специально выделен из `VHAppCore` для парсинга
командной строки (`ParseArgs`) — конфиг отделён от состояния анализа
(исходных данных и результатов сканирований). Синтаксис, состав параметров,
значения по умолчанию и правила разбора описаны в
[CMDLine.md](CMDLine.md); здесь фиксируется только хранение результата.

## Ответственность

- Разбор командной строки (через `vhlibargs`) и валидация входных данных.
- Формирование имён выходных SVG-файлов из имени исходного файла и папки
  вывода.
- Хранение размера окна сканирования (`subsize`).

## Поля / Данные

| Поле | Тип | Назначение |
|---|---|---|
| `fnameSource` | `std::string` | Путь к исходному бинарному файлу (`-fname`). |
| `fnameWND` | `std::string` | Путь к выходному SVG «оконного» документа (`<out>_wnd.svg`). |
| `fnameDNAAbs` | `std::string` | Путь к выходному SVG DNA-отпечатка абс. (`<out>_dna_abs.svg`). |
| `fnameDNARel` | `std::string` | Путь к выходному SVG DNA-отпечатка отн. (`<out>_dna_rel.svg`). |
| `dirOutput` | `std::string` | Папка вывода (`-outfolder`); пусто = текущая рабочая папка. |
| `subsize` | `uint16_t` | Размер окна сканирования (`-wndsz`); диапазон и значение по умолчанию — в `CMDLine.md`. |

## Методы

| Сигнатура | Описание |
|---|---|
| `verr ParseArgs(int argc, char *argv[])` | Реализует требования [CMDLine.md](CMDLine.md): разбирает `argv` через `VHArgsParser::VHCmdLineParser` (`vhlibargs`), проверяет `-fname` (непустое, `VHFSYS::FileExist`) и создаёт `-outfolder` при отсутствии (`VHFSYS::CreateNestedDir`), заполняет поля конфига. Имена выходных файлов формируются из `VHFSYS::ExtractFilename()` и `dirOutput`. Возвращает `vok` или ошибку (`verrmsg(1, ...)`, `verrmsg(3, ...)`, `verrmsg(4, ...)`). |

## Связи / Зависимости

- **Входящие:** `VHAppCore` (владеет объектом как поле `appcfg`, методы ядра читают поля конфига), `main.cpp` (косвенно, через `appcore.appcfg.ParseArgs`).
- **Исходящие:** `VHArgsParser` (парсинг), `VHFSYS` (проверка существования файла, создание папки вывода, извлечение имени), `vhplatform` (`verr`/`vok`), `std::filesystem` (склейка папки вывода и имени файла).
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
- `dirOutput` создаётся автоматически вместе с недостающими родительскими
  каталогами (`VHFSYS::CreateNestedDir`); если создание не удалось,
  `ParseArgs` возвращает ошибку (код `4`).
- Метод `ParseArgs` зависит от конкретной реализации `vhlibargs` — тестирование возможно только через передачу `argc/argv`. Особенности парсера и полный список правил — в [CMDLine.md](CMDLine.md).
