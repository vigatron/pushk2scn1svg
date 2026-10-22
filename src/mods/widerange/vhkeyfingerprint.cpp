#include "vhkeyfingerprint.hpp"

/**
 *
 */
VHKeyFingerPrint::VHKeyFingerPrint() : avail(false)
{
    spectrum.fill(0);
}

/**
 *
 */
void VHKeyFingerPrint::put(uint8_t sym, uint16_t pos)
{
    if (!avail)
    {
        avail = true;
        startpos = pos;
        // prevpos = pos;
        endpos = pos;
    }
    else
    {
        // count spc
        // uint16_t dx = pos - prevpos;
        uint16_t dx = pos - startpos;

        spectrum[dx]++;

        if (pos > endpos)
            endpos = pos;

        // prevpos = pos;
    }
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
    printf("S %3u | ", (uint16_t)startpos);

    for (size_t idx = startpos; idx <= endpos; idx++)
    {
        if (spectrum[idx])
        {
            // printf("@%u %u | ", (uint16_t)idx, spectrum[idx]);
            printf("%u (%u), ", (uint16_t)idx, spectrum[idx]);
        }
    }

    printf("E %3u ", (uint16_t)endpos);
}
