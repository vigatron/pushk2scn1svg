#pragma once

#include <string>
#include <cinttypes>

#include "widerange/widerange.hpp"

class VHAppCore
{
    public:

    std::string fnameSource;
    std::string fnameWND;
    std::string fnameDNAAbs;
    std::string fnameDNARel;

    uint16_t subsize;

    // WideRange Scan Abs
    VHWideRangeScan wscanabs;

    // WideRange Scan Rel
    VHWideRangeScan wscanrel;

};

