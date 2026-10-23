#pragma once

#include "vhplatform.hpp"

#include "structs/blockprops.hpp"


#include "vhrect.hpp"

#include "mem/vhmemblk.hpp"

#include "vhsvg.hpp"




void DrawCenterLine(const VHRect &rect);

void DrawShadowBox(const VHRect &rect);

void DrawScale(const VHRect &rect, int size);

void DrawSection(
    const VHRect &rect,
    const std::vector<BlockProps> &arr,
    int blksize, int ws);

void DrawDensity(
    const VHRect &rect,
    const std::vector<BlockProps> &arr,
    int blksize, int ws);

void DrawTypes(const VHRect &rect);

