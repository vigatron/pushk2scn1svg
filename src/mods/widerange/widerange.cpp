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
verr VHWideRangeScan::scan(const std::vector<uint8_t> &arrsrc)
{
    //
    if (arrsrc.size() < 2)
        return verror(1);

    //
    if (arrsrc.size() > VHKeyFingerPrint::MAX_RANGE)
        return verror(2);

    //
    for (size_t idx = 0; idx < arrsrc.size(); idx++)
    {
        uint8_t sym = arrsrc[idx];
        arrelms[sym].put(sym, idx);
    }

    // Results
    for (size_t sym = 0; sym < MAX_ELMS; sym++)
    {
        const VHKeyFingerPrint &obj = wobj(sym);

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

    // exit(1);

    return vok;
}
