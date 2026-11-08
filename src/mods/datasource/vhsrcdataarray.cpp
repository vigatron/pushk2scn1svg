#include "datasource/vhsrcdataarray.hpp"

#include "VHSHA256.hpp"
#include "global.hpp"
#include "vhfile.hpp"

/**
 * @brief Loads a source file into the internal buffer with a minimum size check.
 *
 * @param fname Path to the source file.
 * @param minsize Minimum required file size in bytes.
 * @return Reference to the loaded data buffer; empty if loading fails or the file is too small.
 */
const std::vector<uint8_t> &VHSrcDataArray::LoadSource(const std::string &fname, int minsize)
{
    arrsrc.clear();

    VHFile f(fname);

    fsize = f.flen();
    if (fsize < minsize)
    {
        const std::string msg = "Minimal size less than " + std::to_string(minsize);
        dbg.txtnl(msg.c_str());
        strsha256 = VHSHA256::hashHex(arrsrc);
        return arrsrc;
    }

    arrsrc.resize(static_cast<size_t>(fsize));
    if (fsize > 0 && !f.read(arrsrc.data(), fsize, 0))
    {
        arrsrc.clear();
        const std::string msg = "Cant read file " + std::to_string(minsize);
        dbg.txtnl(msg.c_str());
        strsha256 = VHSHA256::hashHex(arrsrc);
        return arrsrc;
    }

    strsha256 = VHSHA256::hashHex(arrsrc);
    return arrsrc;
}
