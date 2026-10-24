#include "widerange.hpp"


/**
 *
 */
VHWideRangeScan::VHWideRangeScan()
{
    arrelms.resize(MAX_ELMS);
}

/**
 *
 */
const VHKeyFingerPrint & VHWideRangeScan::operator[](size_t idx) const
{
    return arrelms[idx];
}

/**
 * 
 */
verr VHWideRangeScan::scanabs(const std::vector<uint8_t> &arrsrc)
{

    //
    if (arrsrc.size() < 2)
        return verror(1);

    //
    if (arrsrc.size() > VHKeyFingerPrint::MAX_RANGE)
        return verror(2);

    // Save for later analisys
    arrwidth = arrsrc.size();

    //
    for (size_t idx = 0; idx < arrsrc.size(); idx++)
    {
        uint8_t sym = arrsrc[idx];
        arrelms[sym].putabs(sym, idx);
    }
    
    return vok;
}

/**
 *
 */
verr VHWideRangeScan::scanrel(const std::vector<uint8_t> &arrsrc)
{
    //
    if (arrsrc.size() < 2)
        return verror(1);

    //
    if (arrsrc.size() > VHKeyFingerPrint::MAX_RANGE)
        return verror(2);

    // Save for later analisys
    arrwidth = arrsrc.size();

    //
    for (size_t idx = 0; idx < arrsrc.size(); idx++)
    {
        uint8_t sym = arrsrc[idx];
        arrelms[sym].putrel(sym, idx);
    }

    return vok;
}
