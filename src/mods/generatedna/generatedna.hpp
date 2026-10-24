#pragma once

#include "vhplatform.hpp"
#include "structs/blockprops.hpp"
#include "draw/draw.hpp"
#include "vhsvg.hpp"

#include "widerange/widerange.hpp"

#include "cfg/appcfgstruct.hpp"

class VHDocDNA
{
public:
    /**
     *
     */
    verr GenerateDocument(
        const VHWideRangeScan & range,
        const std::string &fnamedna);

private:

    void Gen256(VHSVG & svg, const VHWideRangeScan & wscan);

    // DOC Size
    static const uint16_t doc_size_w = 2400;
    static const uint16_t doc_size_h = 3000;

    // Content area
    static const uint16_t doc_content_dist_w = 200;
    static const uint16_t doc_content_dist_h = 200;

};
