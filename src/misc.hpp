#pragma once

#include "vhplatform.hpp"
#include "vhmemblk.hpp"
#include "blockprops.hpp"

std::vector<BlockProps> GenSamplesArray( const VHMemBlock & mblk , int subsize );

verr verrmsg(int x, std::string strerr);
