#pragma once

#include "vhplatform.hpp"

#include "structs/blockprops.hpp"

#include "mem/vhmemblk.hpp"

#include "vhsvg.hpp"




void DrawCenterLine(const VHSVGRect &rect);

void DrawShadowBox(const VHSVGRect &rect);

void DrawScale(const VHSVGRect &rect, int size);

void DrawSection(
    const VHSVGRect &rect,
    const std::vector<BlockProps> &arr,
    int blksize, int ws);

void DrawDensity(
    const VHSVGRect &rect,
    const std::vector<BlockProps> &arr,
    int blksize, int ws);

void DrawTypes(const VHSVGRect &rect);

