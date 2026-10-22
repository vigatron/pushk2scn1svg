#pragma once

#include "vhplatform.hpp"

#include "vhrect.hpp"
#include "blockprops.hpp"
#include "draw.hpp"

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
    void Draw2X4Content(const std::vector<uint8_t> &arrsrc, const VHRect &rect);

    /**
     *
     */
    std::vector<BlockProps> GenSamplesArray(const std::vector<uint8_t> &arrsrc, int subsize);
};
