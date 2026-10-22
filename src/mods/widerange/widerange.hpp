#pragma once

#include "vhplatform.hpp"

#include <array>
#include <vector>

/**
 * 
 */
class VHWideByte
{
    public:

        //
        VHWideByte();

        //
        void put(uint8_t sym, uint16_t pos);


        bool valid() const;
        bool empty() const;
        void dump() const;

        static const size_t MAX_RANGE = 32 * F1K;

    private:
        bool avail;
        uint16_t startpos;
        uint16_t endpos;

        // uint16_t prevpos;


        std::array<uint16_t, MAX_RANGE> spectrum;
};

/**
 * @brief Cross-range matches on 64K block
 */
class VHWideRangeScan
{
    public:

        VHWideRangeScan();

        verr scan(const std::vector<uint8_t> & arrsrc);

        static const size_t MAX_ELMS = 256;

        const VHWideByte & wobj(size_t idx) { return arrelms[idx]; }

    private:

        //
        std::vector<VHWideByte> arrelms;

        // Starting pos
        // static const size_t MAX_ELMS = F1K * 64;
        // std::array<uint8_t, MAX_ELMS> arrstart;

        // 64K-Bits = 8K-Bytes
        // static const size_t MAX_MEM_SIZE = 256 * MAX_ELMS;

};
