#pragma once

#include "vhplatform.hpp"
#include "structs/blockprops.hpp"
#include "draw/draw.hpp"
#include "vhsvg.hpp"

#include "widerange/widerange.hpp"

class VHDocDNA
{
public:
    /**
     *
     */
    verr GenerateDocument(std::vector<uint8_t> arrsrc, const std::string &fnamedna);

private:

    void Gen256(VHSVG & svg, const VHWideRangeScan & wscan);
};
