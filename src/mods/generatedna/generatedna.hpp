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
     * @brief Generates a DNA document SVG from a wide range scan.
     *
     * @param range Wide range scan to visualize.
     * @param fnamedna Output SVG file path.
     * @param strsha256 Source file SHA256 hash.
     * @param filesize Source file size in bytes.
     * @return vok on success, or an error status on failure.
     */
    verr GenerateDocument(
        const VHWideRangeScan & range,
        const std::string &fnamedna,
        const std::string &strsha256,
        size_t filesize);

    void GenerateHeader(
        int startx, int starty, int w, int h,
        const std::string & filename,
        size_t filesize,
        const std::string & strsha256);

private:

    void Gen256(VHSVG & svg, const VHWideRangeScan & wscan);

    // DOC Size
    static const uint16_t doc_size_w = 2000;
    static const uint16_t doc_size_h = 2600;

    // Content area
    static const uint16_t doc_content_dist_w = 300;
    static const uint16_t doc_content_dist_h = 200;

    // === SVG GENERATION ===
    VHSVG svg;

};
