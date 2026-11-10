#include "vhkeyfingerprint.hpp"

#include <cstdio>

/**
 * @brief Creates an empty fingerprint with a zeroed bit set.
 */
VHKeyFingerPrint::VHKeyFingerPrint()
    : avail(false),
      relmode(false),
      startpos(0),
      endpos(0),
      arr(MAX_RANGE / 8, 0)
{
}

/**
 * @brief Registers an absolute position of the value.
 *
 * @param pos Absolute offset from the start of the scanned buffer.
 */
void VHKeyFingerPrint::putabs(uint16_t pos)
{
    if (!avail)
    {
        avail = true;
        relmode = false;
        startpos = pos;
        endpos = pos;
    }
    else
    {
        if (pos < startpos)
            startpos = pos;
        if (pos > endpos)
            endpos = pos;
    }

    mark(pos);
}

/**
 * @brief Registers a relative position of the value.
 *
 * @param pos Absolute offset of the occurrence in the scanned buffer.
 */
void VHKeyFingerPrint::putrel(uint16_t pos)
{
    if (!avail)
    {
        avail = true;
        relmode = true;
        startpos = pos;
        endpos = pos;
        mark(0);
        return;
    }

    mark(static_cast<uint16_t>(pos - startpos));

    if (pos > endpos)
        endpos = pos;
}

/**
 * @brief Sets the bit at the given position in the scan coordinate space.
 *
 * @param pos Absolute position or relative offset depending on the mode.
 */
void VHKeyFingerPrint::mark(uint16_t pos)
{
    VHBits::BitSet(arr.data(), pos);
}

/**
 * @brief Returns the bit at the given position in the scan coordinate space.
 *
 * @param pos Absolute position or relative offset depending on the mode.
 * @return true if the position is marked, false otherwise.
 */
bool VHKeyFingerPrint::status(uint16_t pos) const
{
    return VHBits::BitVal(arr.data(), pos);
}

/**
 * @brief Returns true if at least one position was registered.
 *
 * @return true if the fingerprint contains data.
 */
bool VHKeyFingerPrint::valid() const
{
    return avail;
}

/**
 * @brief Returns true if no position was registered.
 *
 * @return true if the fingerprint is empty.
 */
bool VHKeyFingerPrint::empty() const
{
    return !avail;
}

/**
 * @brief Prints the range and all marked positions to stdout.
 */
void VHKeyFingerPrint::dump() const
{
    printf("[S %5u : E %3u] ", startpos, endpos);

    const auto* bits = arr.data();

    // In relative mode the bit set holds offsets from startpos, in absolute
    // mode it holds absolute positions; walk the matching range in both cases.
    const size_t from = relmode ? 0u : startpos;
    const size_t to = relmode ? getSpan() : endpos;

    for (size_t idx = from; idx <= to; idx++)
    {
        if (VHBits::BitVal(bits, static_cast<int>(idx)))
        {
            printf("%zu ", idx);
        }
    }
}
