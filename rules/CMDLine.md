# CMDLine — параметры командной строки

## Статус документа

Это не описание класса, а описание параметров командной строки.
Документ является единственным источником требований к CLI: синтаксис,
состав параметров, правила разбора, валидация, коды ошибок и формирование
имён выходных файлов. Описания классов (`VHAppCoreCfg.md`, `VHAppCore.md`)
ссылаются сюда и не дублируют эти правила.

- Реализация: `VHAppCoreCfg::ParseArgs()` — `src/mods/vhappcorecfg/vhappcorecfg.cpp`.
- Библиотека разбора: `vhlibargs` (`VHArgsParser::VHCmdLineParser`),
  submodule `src/extmods/vhlibargs`, только для чтения.
- Точка вызова: `main.cpp` → `appcore.appcfg.ParseArgs(argc, argv)`.

## Синтаксис

```text
pushk2scn1svg -fname <source-file> [-wndsz <2..256>] [-outfolder <folder>]
```

Параметры передаются парами `-ключ значение`. Значение должно идти
следующим аргументом; форма `-ключ=значение` не поддерживается.
Порядок параметров произвольный.

## Параметры

| Ключ | Тип `vhlibargs` | Обязательный | По умолчанию | Описание (`ShowUsage`) | Поле `VHAppCoreCfg` |
|---|---|---|---|---|---|
| `-fname` | `VHArgTypeString` | да | — (пусто) | `Source file` | `fnameSource` |
| `-wndsz` | `VHArgTypeInteger` | нет | `128`, диапазон `2..256` | `Window size` | `subsize` |
| `-outfolder` | `VHArgTypeString` | нет | пусто = текущая рабочая папка | `Output Files folder` | `dirOutput` |

Имя приложения в usage: `pushk2scn1svg`; описание:
`scan filter tool. V0.1.0`. Полная первая строка usage:
`pushk2scn1svg scan filter tool. V0.1.0`.
Версия `V0.1.0` берётся из `APP_VERSION`, который генерируется из
`PROJECT_VERSION` в `CMakeLists.txt` (шаблон
`src/mods/vhappcorecfg/app_version.hpp.in`).

### `-fname`

Путь к исходному бинарному файлу. Должен быть непустым и указывать на
существующий **обычный файл**: проверка выполняется через
`std::filesystem::is_regular_file`, поэтому каталоги и специальные файлы
(устройства, FIFO и т.п.) не принимаются. Тип `VHArgTypeString`, а не
`VHArgTypeFileName`: ограничение `vhlibargs` на символы `< > % * ? :`
не применяется, допустимость пути определяет файловая система.

Размер исходного файла ограничен сверху: не более
`VHSrcDataArray::MAX_SOURCE_SIZE` = **32 768 байт (32 КиБ)**. Ограничение
следует из хранения позиций в `uint16_t` (`VHKeyFingerPrint::MAX_RANGE`).
Файл большего размера отвергается в `ParseArgs` до создания папки вывода;
та же проверка повторяется в `VHSrcDataArray::LoadSource`.

### `-wndsz`

Размер окна сканирования в байтах: целое число `2..256`, по умолчанию `128`.
Значение предварительно проверяется как обычное десятичное целое
(`std::from_chars`, без завершающих символов); `abc`, `2x` и переполнение
`int` отвергаются с сообщением `Invalid value for -wndsz: <value>` до
вызова `vhlibargs`. В текущей реализации используется как **минимальный
допустимый размер исходного файла** (`VHSrcDataArray::LoadSource`); на
размеры блоков WND-документа не влияет — они жёстко заданы в `VHDocWnd`
(`64 << row`).

### `-outfolder`

Папка для трёх выходных SVG-файлов. Пустое значение (параметр не задан)
означает текущую рабочую папку — поведение по умолчанию. Если папка не
существует, она создаётся автоматически вместе со всеми недостающими
родительскими каталогами (`VHFSYS::CreateNestedDir`), например
`results/test2/latest`. Если создать папку не удалось (нет прав, путь
занят файлом и т.п.), `ParseArgs` завершается ошибкой; исключения
`std::filesystem` при создании перехватываются и также превращаются
в ошибку. Принимаются относительные и абсолютные пути; завершающий
разделитель пути допускается (на POSIX — `/`, на Windows — `/` или `\`).
Тип `VHArgTypeString` — ограничения на символы, как у `-fname`,
не применяются.

## Правила разбора (`vhlibargs`)

- Аргументы вида `-ключ` распознаются по префиксу `-`; значение — следующий
  аргумент командной строки.
- Неизвестный ключ → `Invalid key: <key>` и ошибка (код `1`).
- Повторный ключ → `Dublicated key: <key>` и ошибка (код `1`).
- Ключ без значения в конце строки → `key passed without param` (код `1`).
- Аргументы без префикса `-` (позиционные) собираются парсером, но
  `ParseArgs` их не читает и не отвергает.
- Если аргументов нет вообще (`argc < 2`), `ParseArgs` сам печатает usage
  один раз и завершается ошибкой (код `1`); `ParseCommandLine` в этом случае
  не вызывается.
- Для `-wndsz` значение сначала проверяется в `ParseArgs` через
  `std::from_chars`: токен должен целиком быть десятичным целым. Нечисловое
  значение (`abc`), завершающие символы (`2x`) и переполнение `int`
  отвергаются сообщением `Invalid value for -wndsz: <value>` (код `1`) без
  исключений. Затем `vhlibargs` преобразует значение через `std::stoi`;
  выход за диапазон `2..256` — ошибка парсера. Исключения `std::stoi`
  дополнительно перехватываются на случай будущих целочисленных ключей.

## Валидация и коды ошибок

`VHAppCoreCfg::ParseArgs` возвращает:

| Проверка | Сообщение | Код `ParseArgs` |
|---|---|---|
| Ошибка синтаксиса/разбора `vhlibargs` | `Error parsing command line` | `1` |
| `-wndsz` не является десятичным целым или переполняет `int` | `Invalid value for -wndsz: <value> (expected integer 2..256)` | `1` |
| Пустой `-fname` | `Parse args issue` + usage | `1` |
| `-fname` не существует, это каталог или специальный файл | `Wrong input file <path>` | `3` |
| Размер `-fname` больше `MAX_SOURCE_SIZE` (32 768) | `Source file too large <path> (max 32768 bytes)` | `5` |
| `-outfolder` не удалось создать (нет прав, путь занят файлом и т.п.) | `Can't create output folder <path>` | `4` |

`main.cpp` на любую ошибку `ParseArgs` возвращает `verror(1)`, поэтому код
завершения процесса для всех ошибок CLI — `1`; коды `3`/`4`/`5` доступны
только как результат самого метода. Если файл вырос между `ParseArgs` и
загрузкой, `VHSrcDataArray::LoadSource` повторно отвергает его с сообщением
`File too large <path> (max 32768 bytes)`, и `main` завершается с кодом `3`.

## Формирование имён выходных файлов

Базовое имя берётся из имени исходного файла без пути:
`base = std::filesystem::path(fnameSource).filename()` (расширение
сохраняется).

```text
<out>       = dirOutput пусто ? base : <dirOutput>/<base>
fnameWND    = <out>_wnd.svg
fnameDNAAbs = <out>_dna_abs.svg
fnameDNARel = <out>_dna_rel.svg
```

Пример: для `data/backobjs_recombo.cells` и `-outfolder out` получаются
`out/backobjs_recombo.cells_wnd.svg`,
`out/backobjs_recombo.cells_dna_abs.svg`,
`out/backobjs_recombo.cells_dna_rel.svg`.

## Примеры

```sh
# Все три SVG в текущую рабочую папку (по умолчанию)
./build/pushk2scn1svg -fname data/backobjs_recombo.cells

# Другое окно сканирования
./build/pushk2scn1svg -fname data/backobjs_recombo.cells -wndsz 64

# Выходные файлы в отдельную существующую папку
./build/pushk2scn1svg -fname data/backobjs_recombo.cells -outfolder out
```

## Связь с классами и `main`

- `VHAppCoreCfg` (`src/mods/vhappcorecfg/`) — реализует разбор и хранит
  результат в полях `fnameSource`, `fnameWND`, `fnameDNAAbs`, `fnameDNARel`,
  `dirOutput`, `subsize`.
- `VHAppCore` вызывает `appcfg.ParseArgs(argc, argv)` до `LoadSource()`.
- `main.cpp` передаёт `argc/argv` и преобразует ошибку в код возврата `1`.

## История переноса

- Изначально разбор командной строки был свободной функцией
  `verr ParseArgs(int argc, char *argv[], VHAppCore &appcfg)` в модуле
  `src/mods/cmdline/`. После переноса в `VHAppCoreCfg::ParseArgs` папка
  `cmdline/` удалена.
- Параметр `-outfolder` добавлен вместе с полем `dirOutput`; до этого
  выходные файлы всегда создавались в текущей рабочей папке.
- Детали CLI перенесены сюда из `VHAppCoreCfg.md` и `VHAppCore.md`;
  в описаниях классов остались только ссылки. Корневой `README.md`
  содержит краткую справку и синхронизирован с этим документом.
