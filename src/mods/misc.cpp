#include "misc.hpp"

// ----------------------------------------------------------------------------
std::vector<BlockProps> GenSamplesArray(const VHMemBlock &mblk, int subsize)
{
    std::vector<BlockProps> r;

    // Process block
    int blkcnt = mblk.sz() / subsize;
    for (int i = 0; i < blkcnt; i++)
    {
        BlockProps blk;
        blk.scan(mblk() + i * subsize, subsize);
        r.push_back(blk);
    }

    return r;
}
