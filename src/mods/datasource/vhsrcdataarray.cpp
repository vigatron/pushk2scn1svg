#include "datasource/vhsrcdataarray.hpp"

#include <filesystem>
#include <fstream>
#include <system_error>

#include "VHSHA256.hpp"
#include "global.hpp"

/**
 * @brief Loads a source file into the internal buffer with size checks.
 *
 * On any failure the buffer and the SHA-256 string are left empty and the
 * stored file size is reset to 0, so the object never holds a partially
 * loaded state. The file is opened in binary mode to avoid newline
 * translation on platforms where text mode is not transparent.
 *
 * @param fname Path to the source file.
 * @param minsize Minimum required file size in bytes.
 * @return Reference to the loaded data buffer; empty if loading fails,
 * the file is too small or too large.
 */
const std::vector<uint8_t> &VHSrcDataArray::LoadSource(const std::string &fname, int minsize)
{
    arrsrc.clear();
    strsha256.clear();
    fsize = 0;

    const size_t minbytes = (minsize > 0) ? static_cast<size_t>(minsize) : 0u;

    std::error_code ec;
    const std::uintmax_t filelen = std::filesystem::file_size(fname, ec);
    if (ec)
    {
        const std::string msg = "Cant read file " + fname;
        dbg.txtnl(msg.c_str());
        return arrsrc;
    }

    if (filelen > MAX_SOURCE_SIZE)
    {
        const std::string msg = "File too large " + fname + " (max " +
            std::to_string(MAX_SOURCE_SIZE) + " bytes)";
        dbg.txtnl(msg.c_str());
        return arrsrc;
    }

    fsize = static_cast<size_t>(filelen);

    if (fsize < minbytes)
    {
        const std::string msg = "Minimal size less than " + std::to_string(minbytes);
        dbg.txtnl(msg.c_str());
        fsize = 0;
        return arrsrc;
    }

    std::ifstream in(fname, std::ios::binary);
    if (!in)
    {
        const std::string msg = "Cant read file " + fname;
        dbg.txtnl(msg.c_str());
        fsize = 0;
        return arrsrc;
    }

    arrsrc.resize(fsize);
    if (fsize > 0)
    {
        in.read(reinterpret_cast<char *>(arrsrc.data()), static_cast<std::streamsize>(fsize));
        if (!in || static_cast<size_t>(in.gcount()) != fsize)
        {
            arrsrc.clear();
            const std::string msg = "Cant read file " + fname;
            dbg.txtnl(msg.c_str());
            fsize = 0;
            return arrsrc;
        }
    }

    strsha256 = VHSHA256::hashHex(arrsrc);
    return arrsrc;
}
