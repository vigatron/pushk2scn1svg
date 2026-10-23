#pragma once

#include "vhplatform.hpp"

#include "structs/blockprops.hpp"

#include "draw/draw.hpp"


class VHDocDNA
{
public:
    /**
     *
     */
    verr GenerateDocument(std::vector<uint8_t> arrsrc, const std::string &fnamedna);
};
