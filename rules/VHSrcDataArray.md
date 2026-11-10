# VHSrcDataArray

## Файл / Модуль

- Заголовок: `src/mods/datasource/vhsrcdataarray.hpp`
- Реализация: `src/mods/datasource/vhsrcdataarray.cpp`
- Класс: `class VHSrcDataArray`

## Назначение

Загрузка исходного бинарного файла в память и хранение его содержимого
в виде массива байтов. Вычисляет SHA-256 хеш загруженных данных и хранит
размер файла. Используется как источник данных для сканирования и генерации
SVG-документов.

## Ответственность

- Чтение файла по пути с проверкой минимального размера.
- Хранение содержимого в `std::vector<uint8_t>`.
- Вычисление и хранение SHA-256 хеша (через `VHSHA256`).
- Предоставление доступа к данным, хешу и размеру через const-геттеры.

## Поля / Данные

| Поле | Тип | Назначение |
|---|---|---|
| `arrsrc` | `std::vector<uint8_t>` | Содержимое исходного файла. |
| `strsha256` | `std::string` | Hex-строка SHA-256 (64 символа) от `arrsrc`. Пустая, если файл не загружен. |
| `fsize` | `size_t` | Размер файла в байтах; `0`, если файл не загружен (в том числе после ошибки). |

## Методы

| Сигнатура | Описание |
|---|---|
| `const std::vector<uint8_t> &LoadSource(const std::string &fname, int minsize)` | Загружает файл `fname` через `VHFile`. Если `flen()` вернул `< 0` (файл не открылся), размер меньше `minsize` или больше `INT_MAX` — очищает буфер и хеш, обнуляет `fsize`, пишет сообщение в глобальный `dbg` и возвращает пустую ссылку. Иначе заполняет буфер, вычисляет SHA-256 и возвращает ссылку. |
| `const std::vector<uint8_t> &data() const` | Возвращает ссылку на загруженные данные. |
| `const std::string &strSHA256() const` | Возвращает hex-строку SHA-256 загруженных данных. |
| `size_t filesize() const` | Возвращает размер исходного файла в байтах. |

## Связи / Зависимости

- **Входящие:** `VHAppCore` (владеет объектом как поле `datasource`, вызывает `datasource.LoadSource()` из метода `VHAppCore::LoadSource`), `main.cpp` (косвенно, через `appcore.datasource.data()` / `strSHA256()` / `filesize()`).
- **Исходящие:** `VHFile` (submodule `vhlibfsys`) — чтение файла; `VHSHA256` — хеширование; глобальный `dbg` — сообщения об ошибках.
- Используется также как вход для `VHDocWnd::GenerateDocument` и `VHDocDNA::GenerateDocument` (через `strSHA256()` и `filesize()`).

## Использование

```cpp
// Напрямую (вне VHAppCore) — не рекомендуется, VHAppCore владеет объектом:
// VHSrcDataArray datasource;
// const auto &arrsrc = datasource.LoadSource(appcore.appcfg.fnameSource, appcore.appcfg.subsize);

// Через VHAppCore (основной путь):
if (vok != appcore.LoadSource())
    return verror(3);
const std::vector<uint8_t> &arrsrc = appcore.datasource.data();
```

## Ограничения / Замечания

- При неудаче `LoadSource` возвращает пустой вектор, но не сигнал ошибки — вызывающий код обязан сам проверить размер.
- Удачный вызов можно повторить — буфер очищается в начале (`arrsrc.clear()`).
- При ошибке `strsha256` остаётся пустой, `fsize = 0`; частично загруженного состояния нет.
- `flen()` возвращает `int`; отрицательное значение (ошибка открытия) обрабатывается до приведения к `size_t`.
