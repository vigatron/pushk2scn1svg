#include "datasource/vhsrcdataarray.hpp"

#include "global.hpp"
#include "vhfile.hpp"

/**
 * @brief Loads a source file into a memory block with a minimum size check.
 *
 * @param fname Path to the source file.
 * @param minsize Minimum required file size in bytes.
 * @param r Destination memory block to fill.
 * @return vok on success, or an error status on failure.
 */
const std::vector<uint8_t> & VHSrcDataArray::LoadSource(const std::string &fname, int minsize)
{
    arrsrc.clear();
    
    VHMemBlock memblk;


    VHFile f(fname);

    const int fsize = f.flen();
    if (fsize < minsize)
    {
        const std::string msg = "Minimal size less than " + std::to_string(minsize);
        dbg.txtnl(msg.c_str());
        return arrsrc;
    }

    if (! memblk.LoadFromFile(fname.c_str()))
    {
        const std::string msg = "Cant read file " + std::to_string(minsize);
        dbg.txtnl(msg.c_str());
        return arrsrc;
    }

    // Convert source to std::vector
    std::vector<uint8_t> newblock(memblk.ptr(), memblk.ptr() + memblk.sz());

    arrsrc = newblock;

    return arrsrc;
}
