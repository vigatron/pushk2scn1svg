#pragma once

#include "vhplatform.hpp"
#include "structs/blockprops.hpp"
#include "draw/draw.hpp"
#include "vhsvg.hpp"

class VHDocWnd
{
public:
    /**
     *
     */
    verr GenerateDocument(const std::vector<uint8_t> &arrsrc, const std::string &fnamewnd);

    /**
     *
     */
    void Draw2X4Content(VHSVG &svg, const std::vector<uint8_t> &arrsrc, const VHSVGRect &rect);

    /**
     *
     */
    std::vector<BlockProps> GenSamplesArray(const std::vector<uint8_t> &arrsrc, int subsize);
};
