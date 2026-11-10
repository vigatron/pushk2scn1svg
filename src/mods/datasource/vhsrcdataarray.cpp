#include "datasource/vhsrcdataarray.hpp"

#include <climits>

#include "VHSHA256.hpp"
#include "global.hpp"
#include "vhfile.hpp"

/**
 * @brief Loads a source file into the internal buffer with a minimum size check.
 *
 * On any failure the buffer and the SHA-256 string are left empty and the
 * stored file size is reset to 0, so the object never holds a partially
 * loaded state.
 *
 * @param fname Path to the source file.
 * @param minsize Minimum required file size in bytes.
 * @return Reference to the loaded data buffer; empty if loading fails or the file is too small.
 */
const std::vector<uint8_t> &VHSrcDataArray::LoadSource(const std::string &fname, int minsize)
{
    arrsrc.clear();
    strsha256.clear();
    fsize = 0;

    const size_t minbytes = (minsize > 0) ? static_cast<size_t>(minsize) : 0u;

    VHFile f(fname);

    // VHFile::flen() returns -1 on failure; converting it to size_t directly
    // would turn the error into a huge allocation request.
    const int len = f.flen();
    if (len < 0)
    {
        const std::string msg = "Cant read file " + fname;
        dbg.txtnl(msg.c_str());
        return arrsrc;
    }

    fsize = static_cast<size_t>(len);

    if (fsize < minbytes)
    {
        const std::string msg = "Minimal size less than " + std::to_string(minbytes);
        dbg.txtnl(msg.c_str());
        fsize = 0;
        return arrsrc;
    }

    // VHFile::read() takes an int length, so larger files cannot be loaded.
    if (fsize > static_cast<size_t>(INT_MAX))
    {
        const std::string msg = "File too large " + fname;
        dbg.txtnl(msg.c_str());
        fsize = 0;
        return arrsrc;
    }

    arrsrc.resize(fsize);
    if (fsize > 0 && !f.read(arrsrc.data(), static_cast<int>(fsize), 0))
    {
        arrsrc.clear();
        const std::string msg = "Cant read file " + fname;
        dbg.txtnl(msg.c_str());
        fsize = 0;
        return arrsrc;
    }

    strsha256 = VHSHA256::hashHex(arrsrc);
    return arrsrc;
}
