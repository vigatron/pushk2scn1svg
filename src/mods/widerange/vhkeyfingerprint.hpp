#pragma once

#include "vhplatform.hpp"

/**
 * 
 */
class VHKeyFingerPrint
{
    public:

        //
        VHKeyFingerPrint();

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

        std::array<uint16_t, MAX_RANGE> spectrum;
};
