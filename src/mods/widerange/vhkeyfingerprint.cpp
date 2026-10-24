#include "vhkeyfingerprint.hpp"

/**
 *
 */
VHKeyFingerPrint::VHKeyFingerPrint() : avail(false)
{
    arr.resize(MAX_RANGE/8, 0);
}

/**
 *
 */
void VHKeyFingerPrint::putabs(uint8_t sym, uint16_t pos)
{
    if (!avail)
    {
        avail = true;
        startpos = pos;
        endpos = pos;
    }
    else
    {
        mark(pos);
        if (pos > endpos)
            endpos = pos;
    }
}

/**
 *
 */
void VHKeyFingerPrint::putrel(uint8_t sym, uint16_t pos)
{
    if (!avail)
    {
        avail = true;
        startpos = pos;
        endpos = pos;
    }
    else
    {
        uint16_t dx = pos - startpos;
        mark(dx);
        if (pos > endpos)
            endpos = pos;
    }
}

/**
 * 
 */
void VHKeyFingerPrint::mark(uint16_t pos)
{
    VHBits::BitSet(arr.data(), pos);
}

/**
 *
 */
bool VHKeyFingerPrint::status(uint16_t pos) const
{
    return VHBits::BitVal(arr.data(), pos);
}


/**
 *
 */
bool VHKeyFingerPrint::valid() const
{
    return avail;
}

/**
 *
 */
bool VHKeyFingerPrint::empty() const
{
    return !avail;
}

/**
 *
 */
void VHKeyFingerPrint::dump() const
{
    printf("[S %5u : E %3u] ", startpos, endpos);

    const auto* bits = arr.data();

    for (size_t idx = startpos; idx < endpos + 1; idx++)
    {
        if (VHBits::BitVal(bits, idx))
        {
            printf("%zu ", idx);
        }
    }

}
