#include "vhlogger.hpp"

#include <cstdio>
#include <cstring>
#include <string>

VHLogger::VHLogger() = default;

VHLogger::~VHLogger()
{
    if (file.is_open())
    {
        file.close();
    }
}

bool VHLogger::setfname(std::string_view fname)
{
    if (file.is_open())
    {
        file.close();
    }

    _fname.assign(fname);
    if (_fname.empty())
    {
        return false;
    }

    file.open(_fname, std::ios::out | std::ios::trunc);
    return file.is_open();
}

void VHLogger::flush()
{
    fflush(stdout);
    if (file.is_open())
    {
        file.flush();
    }
}

void VHLogger::disiobuff()
{
    setbuf(stdout, nullptr);
}

void VHLogger::nl()
{
    lout("\n");
}

void VHLogger::spc()
{
    lout(" ");
}

void VHLogger::txt(const char *str)
{
    lout(str);
}

void VHLogger::txtnl(const char *str)
{
    txt(str);
    nl();
}

void VHLogger::hx8(uint8_t val)
{
    char tmp[4];
    std::snprintf(tmp, sizeof(tmp), "%.2X", val);
    lout(tmp);
}

void VHLogger::itxt(const char *txt, int v)
{
    loutFmt(txt, v);
}

void VHLogger::i2txt(const char *txt, int v1, int v2)
{
    loutFmt(txt, v1, v2);
}

void VHLogger::i3txt(const char *txt, int v1, int v2, int v3)
{
    loutFmt(txt, v1, v2, v3);
}

void VHLogger::ftxt(const char *txt, float f)
{
    loutFmt(txt, static_cast<double>(f));
}

void VHLogger::sep()
{
    // Same output as before: 16 x "--------" + newline, in a single write.
    static const std::string line = std::string(128, '-') + '\n';
    writeRaw(line.data(), line.size());
}

void VHLogger::mem(const uint8_t *ptr, int wdt, int align)
{
    if (!ptr || wdt <= 0)
    {
        return;
    }

    std::string out;
    out.reserve(static_cast<std::size_t>(wdt) * 3);

    char tmp[4];
    for (int i = 0; i < wdt; i++)
    {
        std::snprintf(tmp, sizeof(tmp), "%.2X", ptr[i]);
        out += tmp;
        if (i != (wdt - 1))
        {
            out += ' ';
        }
    }

    if (align > wdt)
    {
        out.append(static_cast<std::size_t>(align - wdt) * 3, ' ');
    }

    writeRaw(out.data(), out.size());
}

void VHLogger::memblk(const uint8_t *ptr, int w, int icnt)
{
    if (!ptr || w <= 0 || icnt <= 0)
    {
        return;
    }

    const int rows = (w / icnt) + ((w % icnt) ? 1 : 0);
    std::string row;
    row.reserve(static_cast<std::size_t>(icnt) * 5 + 32);

    char tmp[32];
    for (int j = 0; j < rows; j++)
    {
        const int offs = j * icnt;
        const int cnt = (j != (rows - 1)) ? icnt : (w - offs);

        row.clear();
        std::snprintf(tmp, sizeof(tmp), "%8d %8X : ", offs, offs);
        row += tmp;

        for (int i = 0; i < cnt; i++)
        {
            if (!(i % 8))
            {
                row += ' ';
            }
            std::snprintf(tmp, sizeof(tmp), "%.2x ", ptr[offs + i]);
            row += tmp;
        }
        row += '\n';
        writeRaw(row.data(), row.size());
    }
    flush();
}

void VHLogger::memblk16spc(const int16_t *ptr, int w, int icnt)
{
    if (!ptr || w <= 0 || icnt <= 0)
    {
        return;
    }

    const int rows = (w / icnt) + ((w % icnt) ? 1 : 0);
    std::string row;
    row.reserve(static_cast<std::size_t>(icnt) * 5 + 32);

    char tmp[32];
    for (int j = 0; j < rows; j++)
    {
        const int offs = j * icnt;
        const int cnt = (j != (rows - 1)) ? icnt : (w - offs);

        row.clear();
        std::snprintf(tmp, sizeof(tmp), "%8d %8X : ", offs, offs);
        row += tmp;

        for (int i = 0; i < cnt; i++)
        {
            if (!(i % 8))
            {
                row += ' ';
            }
            const int16_t v = ptr[offs + i];
            if (v >= 0)
            {
                std::snprintf(tmp, sizeof(tmp), "%4x ", v);
                row += tmp;
            }
            else
            {
                row += "--  ";
            }
        }
        row += '\n';
        writeRaw(row.data(), row.size());
    }
    flush();
}

void VHLogger::writeRaw(const char *data, std::size_t len)
{
    if (!data || len == 0)
    {
        return;
    }

    if (!file.is_open())
    {
        std::fwrite(data, 1, len, stdout);
    }
    else
    {
        file.write(data, static_cast<std::streamsize>(len));
    }
}

void VHLogger::prnbuf()
{
    writeRaw(outBuf_.data(), std::strlen(outBuf_.data()));
}

void VHLogger::lout(const char *txt)
{
    if (!txt)
    {
        return;
    }

    std::snprintf(outBuf_.data(), outBuf_.size(), "%s", txt);
    prnbuf();
}

void VHLogger::loutFmt(const char *fmt, ...)
{
    if (!fmt)
    {
        return;
    }

    std::va_list args;
    va_start(args, fmt);
    // vsnprintf always NUL-terminates (truncating if needed), so
    // prnbuf() never reads past the buffer.
    std::vsnprintf(outBuf_.data(), outBuf_.size(), fmt, args);
    va_end(args);
    prnbuf();
}
