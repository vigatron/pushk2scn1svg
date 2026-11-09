# VHLogger

## Файл / Модуль

- Заголовок: `src/mods/vhlogger/vhlogger.hpp`
- Реализация: `src/mods/vhlogger/vhlogger.cpp`
- Класс: `class VHLogger` (ранее `class DBG` в `src/mods/dbg/`)
- Экземпляр: глобальный `extern VHLogger dbg;` (`src/mods/global/global.hpp` / `global.cpp`, имя переменной сохранено)

## Назначение

Отладочный вывод: форматированная печать текста, чисел, шестнадцатеричных
значений и дампов памяти. Выводит в stdout или в файл (если ранее успешно
вызван `setfname`). Используется как глобальный логгер во всех модулях
проекта (подключён через `global/global.hpp`).

## Ответственность

- Форматированный текстовый вывод (строки, числа, float, разделители).
- Дамп блоков памяти в hex-представлении.
- Переключение вывода stdout → файл.
- Сброс буфера вывода в stdout и в файл.

## Поля / Данные

| Поле | Тип | Назначение |
|---|---|---|
| `_fname` | `std::string` | Имя файла вывода (если задано). |
| `file` | `std::ofstream` | Файловый поток вывода (открывается в `setfname`). |
| `outBuf_` | `std::array<char, 512>` (поле экземпляра) | Внутренний буфер форматирования (`vsnprintf`). |

## Методы (публичные)

| Сигнатура | Описание |
|---|---|
| `VHLogger()` / `~VHLogger()` | Конструктор пустой; деструктор закрывает файл, если открыт. Копирование запрещено (`= delete`). |
| `bool setfname(std::string_view fname)` | Закрывает предыдущий файл, открывает новый (`out\|trunc`). Возвращает `true` при успехе; при неудаче вывод остаётся в stdout. Пустое имя → `false`. |
| `void nl()` | Выводит перевод строки. |
| `void spc()` | Выводит пробел. |
| `void txt(const char *str)` | Выводит строку как обычный текст (не формат). `nullptr` игнорируется. |
| `void txtnl(const char *str)` | Выводит строку и перевод строки. `nullptr` — только перевод строки. |
| `void hx8(uint8_t val)` | Выводит байт в hex (`%.2X`). |
| `void itxt(const char *txt, int v)` | Выводит `txt` как формат с одним int. `nullptr`-формат игнорируется. |
| `void i2txt(const char *txt, int v1, int v2)` | Формат с двумя int. |
| `void i3txt(const char *txt, int v1, int v2, int v3)` | Формат с тремя int. |
| `void ftxt(const char *txt, float f)` | Формат с float. |
| `void sep()` | Выводит разделитель из 128 `-` и перевод строки (один вызов записи). |
| `void mem(const uint8_t *ptr, int wdt, int align=-1)` | Hex-дамп `wdt` байтов одной строкой, выравнивает до `align`. `ptr==nullptr` или `wdt<=0` — no-op. |
| `void memblk(const uint8_t *ptr, int w, int icnt=32)` | Hex-дамп блока памяти в виде строк с адресами/смещениями. `ptr==nullptr`, `w<=0` или `icnt<=0` — no-op. |
| `void memblk16spc(const int16_t *ptr, int w, int icnt=32)` | Дамп 16-битных значений (`%4x`); отрицательные печатаются как `--`. Те же проверки, что у `memblk`. |
| `void flush()` | `fflush(stdout)` + `file.flush()` (если файл открыт). |
| `void disiobuff()` | Отключает буферизацию stdout (`setbuf(stdout, nullptr)`). Вызывать до любого I/O stdout. |

Приватные: `prnbuf()` (пишет `outBuf_` в stdout или файл), `writeRaw()` —
единая точка записи в stdout/файл, `lout()` — обычный текст, `loutFmt()` —
единое variadic-ядро `vsnprintf` для `itxt/i2txt/i3txt/ftxt`. Дампы
`mem/memblk/memblk16spc` собирают строку и пишут её одним вызовом.

## Связи / Зависимости

- **Входящие:** глобальный `dbg` (`VHLogger`) используется в `VHSrcDataArray`
  (сообщения об ошибках загрузки), `DumpSamples()` (`global.cpp`), и может
  использоваться любым модулем через `#include "global/global.hpp"`.
- **Исходящие:** `fwrite`/`vsnprintf`, `std::ofstream`.

## Использование

```cpp
extern VHLogger dbg;  // из global/global.hpp
dbg.txtnl("Loading...");
dbg.i3txt("min=%4d  max=%4d  width=%d ", blk._min, blk._max, blk.wght);
```

## Ограничения / Замечания

- Не потокобезопасно: один глобальный экземпляр, буфер — поле экземпляра
  (не глобальный, в отличие от старого `outBuf[512]`), поэтому несколько
  экземпляров не конфликтуют по буферу, но одновременная запись из потоков
  всё равно не синхронизирована.
- `itxt/i2txt/i3txt/ftxt` трактуют первый аргумент как **формат-строку**:
  обычный текст с `%` нужно печатать через `txt/txtnl`. Формат с
  несоответствующим числом спецификаторов — UB вызывающего (как в `printf`).
- Форматирование длиннее 511 символов тихо обрезается `vsnprintf`.
- `mem()` / `memblk()` / `memblk16spc()` проверяют `nullptr` и неположительные
  `w/icnt` (no-op), но не могут проверить, что диапазон памяти валиден, —
  вызывающий обязан гарантировать читаемый диапазон.
