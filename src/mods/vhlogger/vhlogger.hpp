#pragma once

#include <array>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <string_view>

/**
 * @brief Global debug / diagnostic logger.
 *
 * Formatted text output to stdout or, after a successful setfname() call,
 * to a file. Hexadecimal dumps of memory blocks are also supported.
 * Not thread-safe: one instance (global `dbg`) is intended for
 * single-threaded diagnostic output.
 */
class VHLogger {
    public:

        /**
         * @brief Constructs an empty logger writing to stdout.
         */
        VHLogger();

        /**
         * @brief Destroys the logger, closing the output file if open.
         */
        ~VHLogger();

        /**
         * @brief Redirects output to the given file, truncating it.
         *
         * Any previously opened file is closed first.
         *
         * @param fname Path to the output file.
         * @return true if the file was opened successfully, false otherwise
         *         (output stays on stdout in that case).
         */
        bool setfname(std::string_view fname);

        /**
         * @brief Writes a newline.
         */
        void nl();

        /**
         * @brief Writes a single space.
         */
        void spc();

        /**
         * @brief Writes plain text (not a format string).
         *
         * @param str Text to write; nullptr is ignored.
         */
        void txt(const char *str);

        /**
         * @brief Writes plain text followed by a newline.
         *
         * @param str Text to write; nullptr writes only the newline.
         */
        void txtnl(const char *str);

        /**
         * @brief Writes a byte as two uppercase hex digits.
         *
         * @param val Byte value to write.
         */
        void hx8(uint8_t val);

        /**
         * @brief Writes formatted text with one integer argument.
         *
         * @param txt Format string (e.g. "value=%4d "); nullptr is ignored.
         * @param v   Integer argument for the format string.
         */
        void itxt(const char *txt, int v);

        /**
         * @brief Writes formatted text with two integer arguments.
         *
         * @param txt Format string; nullptr is ignored.
         * @param v1  First integer argument.
         * @param v2  Second integer argument.
         */
        void i2txt(const char *txt, int v1, int v2);

        /**
         * @brief Writes formatted text with three integer arguments.
         *
         * @param txt Format string; nullptr is ignored.
         * @param v1  First integer argument.
         * @param v2  Second integer argument.
         * @param v3  Third integer argument.
         */
        void i3txt(const char *txt, int v1, int v2, int v3);

        /**
         * @brief Writes formatted text with one float argument.
         *
         * @param txt Format string; nullptr is ignored.
         * @param f   Float argument for the format string.
         */
        void ftxt(const char *txt, float f);

        /**
         * @brief Writes a separator line (128 '-' characters) and a newline.
         */
        void sep();

        /**
         * @brief Writes a hex dump of a byte block on one line.
         *
         * @param ptr   Pointer to the first byte; nullptr is ignored.
         * @param wdt   Number of bytes to dump; <= 0 is ignored.
         * @param align If not -1 and larger than wdt, pads with spaces to
         *              align columns (3 characters per byte).
         */
        void mem(const uint8_t *ptr, int wdt, int align = -1);

        /**
         * @brief Writes a hex dump of a byte block, one row per icnt bytes.
         *
         * Each row starts with decimal/hex offset, then lowercase hex bytes.
         *
         * @param ptr  Pointer to the first byte; nullptr is ignored.
         * @param w    Number of bytes to dump; <= 0 is ignored.
         * @param icnt Bytes per row; <= 0 is ignored.
         */
        void memblk(const uint8_t *ptr, int w, int icnt = 32);

        /**
         * @brief Writes a hex dump of 16-bit values, one row per icnt values.
         *
         * Non-negative values are printed as %4x, negative values as "--".
         *
         * @param ptr  Pointer to the first value; nullptr is ignored.
         * @param w    Number of values to dump; <= 0 is ignored.
         * @param icnt Values per row; <= 0 is ignored.
         */
        void memblk16spc(const int16_t *ptr, int w, int icnt = 32);

        /**
         * @brief Flushes buffered output (stdout and the output file).
         */
        void flush();

        /**
         * @brief Disables stdout buffering.
         *
         * Must be called before any stdout I/O for defined behaviour.
         */
        void disiobuff();

        VHLogger(const VHLogger &) = delete;
        VHLogger &operator=(const VHLogger &) = delete;

    private:

        /**
         * @brief Size of the internal formatting buffer.
         */
        static constexpr std::size_t kOutBufMax = 512;

        std::string _fname;
        std::ofstream file;
        std::array<char, kOutBufMax> outBuf_{};

        /**
         * @brief Writes the contents of outBuf_ to stdout or the output file.
         */
        void prnbuf();

        /**
         * @brief Writes raw bytes to stdout or the output file in one call.
         *
         * @param data Pointer to the bytes; nullptr or empty is ignored.
         * @param len  Number of bytes to write.
         */
        void writeRaw(const char *data, std::size_t len);

        /**
         * @brief Writes plain text (not a format string).
         *
         * @param txt Text to write; nullptr is ignored.
         */
        void lout(const char *txt);

        /**
         * @brief Formats text with variadic arguments into outBuf_ and writes it.
         *
         * @param fmt  Format string; nullptr is ignored.
         * @param ...  Arguments for the format string.
         */
        void loutFmt(const char *fmt, ...);
};
