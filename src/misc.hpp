#pragma once

#include "string.h"

#include <vector>
#include <string>

#include "vhplatform.hpp"
#include "vhmemblk.hpp"

struct stret { uint8_t min; uint8_t max; uint8_t wght; };

std::vector<stret> GenSamplesArray( const VHMemBlock & mblk , int subsize );

verr verrmsg(int x, std::string strerr);
