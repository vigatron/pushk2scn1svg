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
| `-fname` | `VHArgTypeFileName` | да | — (пусто) | `Source file` | `fnameSource` |
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
существующий файл (`VHFSYS::FileExist`; каталоги не принимаются).
Тип `VHArgTypeFileName` дополнительно отвергает значения, содержащие
символы `< > % * ? :`.

### `-wndsz`

Размер окна сканирования в байтах: целое число `2..256`, по умолчанию `128`.
В текущей реализации используется как **минимальный допустимый размер
исходного файла** (`VHSrcDataArray::LoadSource`); на размеры блоков
WND-документа не влияет — они жёстко заданы в `VHDocWnd` (`64 << row`).

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
- Для целочисленных параметров значение преобразуется через `std::stoi`:
  нечисловое значение приводит к необработанному исключению
  `std::invalid_argument`, слишком большое — к `std::out_of_range`,
  выход за диапазон (после успешного преобразования) — к ошибке парсера.

## Валидация и коды ошибок

`VHAppCoreCfg::ParseArgs` возвращает:

| Проверка | Сообщение | Код `ParseArgs` |
|---|---|---|
| Ошибка синтаксиса/разбора `vhlibargs` | `Error parsing command line` | `1` |
| Пустой `-fname` | `Parse args issue` + usage | `1` |
| `-fname` не существует или это каталог | `Wrong input file <path>` | `3` |
| `-outfolder` не удалось создать (нет прав, путь занят файлом и т.п.) | `Can't create output folder <path>` | `4` |

`main.cpp` на любую ошибку `ParseArgs` возвращает `verror(1)`, поэтому код
завершения процесса для всех ошибок CLI — `1`; коды `3`/`4` доступны только
как результат самого метода.

## Формирование имён выходных файлов

Базовое имя берётся из имени исходного файла без пути:
`base = VHFSYS::ExtractFilename(fnameSource)` (расширение сохраняется).

```text
<out>       = dirOutput пусто ? base : <dirOutput>/<base>
fnameWND    = <out>_wnd.svg
fnameDNAAbs = <out>_dna_abs.svg
fnameDNARel = <out>_dna_rel.svg
```

Пример: для `data/backobjs.recombo.cells` и `-outfolder out` получаются
`out/backobjs.recombo.cells_wnd.svg`,
`out/backobjs.recombo.cells_dna_abs.svg`,
`out/backobjs.recombo.cells_dna_rel.svg`.

## Примеры

```sh
# Все три SVG в текущую рабочую папку (по умолчанию)
./build/pushk2scn1svg -fname data/backobjs.recombo.cells

# Другое окно сканирования
./build/pushk2scn1svg -fname data/backobjs.recombo.cells -wndsz 64

# Выходные файлы в отдельную существующую папку
./build/pushk2scn1svg -fname data/backobjs.recombo.cells -outfolder out
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
