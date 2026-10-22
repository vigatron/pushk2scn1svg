#pragma once

#include "vhplatform.hpp"

#include <array>
#include <vector>

#include "vhkeyfingerprint.hpp"

/**
 * @brief Cross-range matches on 64K block
 */
class VHWideRangeScan
{
    public:

        VHWideRangeScan();

        verr scan(const std::vector<uint8_t> & arrsrc);

        static const size_t MAX_ELMS = 256;

        const VHKeyFingerPrint & wobj(size_t idx) { return arrelms[idx]; }

    private:

        //
        std::vector<VHKeyFingerPrint> arrelms;

        // Starting pos
        // static const size_t MAX_ELMS = F1K * 64;
        // std::array<uint8_t, MAX_ELMS> arrstart;

        // 64K-Bits = 8K-Bytes
        // static const size_t MAX_MEM_SIZE = 256 * MAX_ELMS;

};
