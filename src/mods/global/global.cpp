#include "global.hpp"
#include "structs/blockprops.hpp"

VHLogger dbg;

// ----------------------------------------------------------------------------
void DumpSamples(const std::vector<BlockProps> &arr)
{
    int cnt = 0;
    for (const BlockProps &s : arr)
    {
        dbg.itxt("BLK #%3d    ", cnt++);
        dbg.i3txt("min=%4d  max=%4d  width=%d ", s._min, s._max, s.wght);
        dbg.nl();
    }
}
