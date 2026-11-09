# DBG

## Файл / Модуль

- Заголовок: `src/mods/dbg/dbg.hpp`
- Реализация: `src/mods/dbg/dbg.cpp`
- Класс: `class DBG`
- Экземпляр: глобальный `extern DBG dbg;` (`src/mods/global/global.hpp` / `global.cpp`)

## Назначение

Отладочный вывод: форматированная печать текста, чисел, шестнадцатеричных
значений и дампов памяти. Выводит в stdout или в файл (если ранее вызван
`setfname`). Используется как глобальный логгер во всех модулях проекта
(подключён через `global/global.hpp`).

## Ответственность

- Форматированный текстовый вывод (строки, числа, float, разделители).
- Дамп блоков памяти в hex-представлении.
- Переключение вывода stdout → файл.
- Сброс буфера вывода в stdout.

## Поля / Данные

| Поле | Тип | Назначение |
|---|---|---|
| `_fname` | `std::string` | Имя файла вывода (если задано). |
| `file` | `std::ofstream` | Файловый поток вывода (открывается в `setfname`). |
| `outBuf[512]` | `char[512]` (глобальный, в `dbg.cpp`) | Внутренний буфер форматирования (`snprintf`). |

## Методы (публичные)

| Сигнатура | Описание |
|---|---|
| `DBG()` / `~DBG()` | Конструктор пустой; деструктор закрывает файл, если открыт. |
| `void setfname(std::string fname)` | Открывает файл для вывода (`file.open(_fname)`). |
| `void nl()` | Выводит перевод строки. |
| `void spc()` | Выводит пробел. |
| `void txt(const char *str)` | Выводит строку. |
| `void txtnl(const char *str)` | Выводит строку и перевод строки. |
| `void hx8(uint8_t val)` | Выводит байт в hex (`%.2X`). |
| `void itxt(const char *txt, int v)` | Выводит `txt` как формат с одним int. |
| `void i2txt(const char *txt, int v1, int v2)` | Формат с двумя int. |
| `void i3txt(const char *txt, int v1, int v2, int v3)` | Формат с тремя int. |
| `void ftxt(const char *txt, float f)` | Формат с float. |
| `void sep()` | Выводит разделитель из 16 групп `--------` и перевод строки. |
| `void mem(uint8_t *ptr, int wdt, int align=-1)` | Hex-дамп `wdt` байтов, выравнивает до `align`. |
| `void memblk(uint8_t *ptr, int w, int icnt=32)` | Hex-дамп блока памяти в виде строк с адресами/смещениями. |
| `void memblk16spc(int16_t *ptr, int w, int icnt=32)` | Дамп 16-битных значений; отрицательные печатаются как `--`. |
| `void flush()` | `fflush(stdout)`. |
| `void disiobuff()` | Отключает буферизацию stdout (`setbuf(stdout, NULL)`). |

Приватные: `prnbuf()` (пишет `outBuf` в stdout или файл), `lout()`, `lout_h8()`,
`lout_itxt()`, `lout_i2txt()`, `lout_i3txt()`, `lout_ftxt()` — обёртки
`snprintf` + `prnbuf`.

## Связи / Зависимости

- **Входящие:** глобальный `dbg` используется в `VHSrcDataArray` (сообщения
  об ошибках загрузки), `DumpSamples()` (`global.cpp`), и может использоваться
  любым модулем через `#include "global/global.hpp"`.
- **Исходящие:** `printf`/`snprintf`, `std::ofstream`.

## Использование

```cpp
extern DBG dbg;  // из global/global.hpp
dbg.txtnl("Loading...");
dbg.i3txt("min=%4d  max=%4d  width=%d ", blk._min, blk._max, blk.wght);
```

## Ограничения / Замечания

- **Глобальный буфер `outBuf[512]`** — не потокобезопасно, не реентерабельно.
- **Форматный дефект:** `lout_itxt/i2txt/i3txt/ftxt` передают пользовательский
  `txt` как формат в `snprintf` — строка с `%` ломает форматирование.
- `mem()` / `memblk()` не проверяют `ptr == nullptr` и не проверяют границы — вызывающий обязан гарантировать валидный диапазон.
- `setfname()` открывает файл без проверки успеха открытия.
