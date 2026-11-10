#pragma once

#include "vhplatform.hpp"
#include <vector>

/**
 * @brief Statistics of one fixed-size block of source bytes.
 *
 * The default constructor zeroes every field so an object that has not
 * been scanned yet never exposes indeterminate values.
 */
class VHBlockStat
{

public:
    VHBlockStat() = default;

    /**
     * @brief Scans a block and fills the statistics.
     *
     * @param arr Block bytes to analyze.
     */
    void scan(const std::vector<uint8_t> &arr)
    {

        int min = 256;
        int max = -1;

        memset(spc, 0, sizeof(uint8_t) * 256);

        for (int i = 0; i < static_cast<int>(arr.size()); i++)
        {
            uint8_t v = arr[i];
            if (v > max)
                max = v;
            if (v < min)
                min = v;
            if (spc[v] < 255)
                spc[v]++;
        }

        wght = 0;
        for (int i = 0; i < 256; i++)
        {
            if (spc[i])
                if (wght < 255)
                    wght++;
        }

        _min = min;
        _max = max;
    }

    uint8_t _min = 0;
    uint8_t _max = 0;

    uint8_t spc[256] = {};
    uint8_t wght = 0;
};
