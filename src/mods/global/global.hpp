#pragma once

#include "dbg.hpp"
#include "vhsvg.hpp"
#include "colors.hpp"
#include "vhmemblk.hpp"

extern DBG     dbg;
extern VHSVG   svg;

verr LoadSource(std::string fname, int minsize, VHMemBlock * r);
