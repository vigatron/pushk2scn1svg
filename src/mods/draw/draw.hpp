#pragma once

#include "vhplatform.hpp"

#include "structs/blockprops.hpp"

#include "vhsvg.hpp"

/** */
void DrawCenterLine(
    VHSVG &svg,
    const VHSVGRect &rect);

/** */
void DrawShadowBox(
    VHSVG &svg,
    const VHSVGRect &rect);

/** */
void DrawScale(
    VHSVG &svg,
    const VHSVGRect &rect,
    int size);

/** */
void DrawSection(
    VHSVG &svg,
    const VHSVGRect &rect,
    const std::vector<BlockProps> &arr,
    int blksize, int ws);

/** */
void DrawDensity(
    VHSVG &svg,
    const VHSVGRect &rect,
    const std::vector<BlockProps> &arr,
    int blksize, int ws);

/** */
void DrawTypes(
    VHSVG &svg,
    const VHSVGRect &rect);
