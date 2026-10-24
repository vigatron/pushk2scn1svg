#pragma once

#include "vhplatform.hpp"

#include <array>
#include <vector>

#include "vhkeyfingerprint.hpp"

/**
 * @brief Cross-range matches for 256 syms / 64K block per sym
 */
class VHWideRangeScan
{
public:
    //
    VHWideRangeScan();

    //
    verr scanabs(const std::vector<uint8_t> &arrsrc);

    //
    verr scanrel(const std::vector<uint8_t> &arrsrc);

    //
    static const size_t MAX_ELMS = 256;

    //
    const VHKeyFingerPrint &operator[](size_t idx) const;

    //
    const size_t width() const { return arrwidth; }

private:
    //
    std::vector<VHKeyFingerPrint> arrelms;

    //
    size_t arrwidth;

    void dump()
    {
        // Results
        for (size_t sym = 0; sym < MAX_ELMS; sym++)
        {
            // const VHKeyFingerPrint &obj = wobj(sym);

            printf("#%.3d : ", (uint8_t)sym);

            if (arrelms[sym].valid())
            {
                arrelms[sym].dump();
            }
            else
            {
                printf("---");
            }
            printf("\n");
        }
    }
};
