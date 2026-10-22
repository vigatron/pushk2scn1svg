#include "widerange.hpp"

/**
 *
 */
VHWideByte::VHWideByte() : avail(false) // , startpos(0), endpos(0)
{
    spectrum.fill(0);
}

/**
 *
 */
void VHWideByte::put(uint8_t sym, uint16_t pos)
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
bool VHWideByte::valid() const
{
    return avail;
}

/**
 *
 */
bool VHWideByte::empty() const
{
    return !avail;
}

/**
 *
 */
void VHWideByte::dump() const
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
verr VHWideRangeScan::scan(const std::vector<uint8_t> &arrsrc)
{
    if (arrsrc.size() < 2)
        return verror(1);

    if (arrsrc.size() > VHWideByte::MAX_RANGE)
        return verror(2);

    for (size_t idx = 0; idx < arrsrc.size(); idx++)
    {
        uint8_t sym = arrsrc[idx];
        arrelms[sym].put(sym, idx);
    }

    // Results

    for (size_t sym = 0; sym < MAX_ELMS; sym++)
    {
        const VHWideByte &obj = wobj(sym);

        printf("#%.3d : ", (uint8_t)sym);

        if (obj.valid())
        {
            obj.dump();
        }
        else
        {
            printf("---");
        }
        printf("\n");
    }

    exit(1);

    return vok;
}
