#pragma once

#include "vhplatform.hpp"

#include "vhrect.hpp"
#include "blockprops.hpp"
#include "draw.hpp"

class VHDocDNA
{
public:
    /**
     *
     */
    verr GenerateDocument(std::vector<uint8_t> arrsrc, const std::string &fnamedna);
};
