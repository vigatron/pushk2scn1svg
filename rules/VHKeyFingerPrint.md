# VHKeyFingerPrint

## Файл / Модуль

- Заголовок: `src/mods/widerange/vhkeyfingerprint.hpp`
- Реализация: `src/mods/widerange/vhkeyfingerprint.cpp`
- Класс: `class VHKeyFingerPrint`

## Назначение

Битовый отпечаток (фингерпринт) позиций одного значения байта внутри
сканируемого массива. Хранит битовый массив размером `MAX_RANGE` бит
и границы диапазона (`startpos`/`endpos`). Используется внутри
`VHWideRangeScan` для каждого из 256 значений.

## Ответственность

- Отметка позиций вхождения символа (абсолютных или относительных).
- Проверка установленного бита по позиции.
- Хранение первой и последней позиции вхождения.
- Дамп содержимого в stdout.

## Поля / Данные

| Поле | Тип | Назначение |
|---|---|---|
| `MAX_RANGE` | `static const size_t = 32 * F1K` (32768) | Максимальный размер сканируемого массива (в позициях). |
| `avail` | `bool` | Флаг: было ли хотя бы одно вхождение. |
| `startpos` | `uint16_t` | Первая позиция вхождения. |
| `endpos` | `uint16_t` | Последняя позиция вхождения. |
| `arr` | `std::vector<uint8_t>` | Битовый массив (`MAX_RANGE/8 = 4096` байт, инициализируется нулями). |

## Методы

| Сигнатура | Описание |
|---|---|
| `VHKeyFingerPrint()` | Устанавливает `avail=false`, выделяет битовый массив нулями. |
| `void putabs(uint8_t sym, uint16_t pos)` | Регистрирует абсолютную позицию `pos`. При первом вызове устанавливает `startpos`/`endpos`; далее вызывает `mark(pos)` и обновляет `endpos`. Параметр `sym` не используется. |
| `void putrel(uint8_t sym, uint16_t pos)` | Регистрирует относительную позицию: `dx = pos - startpos`, вызывает `mark(dx)`, обновляет `endpos`. Параметр `sym` не используется. |
| `void mark(uint16_t pos)` | Устанавливает бит `pos` через `VHBits::BitSet(arr.data(), pos)`. |
| `bool status(uint16_t pos) const` | Возвращает значение бита `pos` через `VHBits::BitVal(arr.data(), pos)`. |
| `bool valid() const` | `return avail;` |
| `bool empty() const` | `return !avail;` |
| `const uint16_t getStartPos() const` | Возвращает `startpos`. |
| `const uint16_t getEndPos() const` | Возвращает `endpos`. |
| `void dump() const` | Печатает диапазон `[S E]` и все установленные позиции от `startpos` до `endpos`. |

## Связи / Зависимости

- **Входящие:** `VHWideRangeScan` (хранит 256 объектов), `VHDocDNA::Gen256Stat()` (читает `status()`, `getStartPos()`, `getEndPos()`, `empty()` для отрисовки).
- **Исходящие:** `VHBits` (submodule `vhlibplatform`) — битовые операции; `vhplatform` (`F1K`).

## Использование

```cpp
VHKeyFingerPrint fp;
fp.putabs(0x41, 0);
fp.putabs(0x41, 5);
bool present = fp.status(5);  // true
```

## Ограничения / Замечания

- `startpos` / `endpos` не инициализируются конструктором — до первого `put*` их значения мусорные; `Gen256Stat` читает их для каждого символа безусловна.
- Первое вхождение не помечается `mark()` в `putabs`/`putrel` (помечаются только последующие) — битовый массив не содержит первую позицию.
- `putabs`/`putrel` принимают `uint16_t pos` — при размере массива > 65535 была бы обрезка, но `VHWideRangeScan` ограничивает сканирование `MAX_RANGE`.
- Память: 4096 байт на объект; 256 объектов в одном сканировании — ~1 МБ.
