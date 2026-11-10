#include "widerange.hpp"

/**
 * @brief Creates an empty scan with MAX_ELMS zeroed fingerprints.
 */
VHWideRangeScan::VHWideRangeScan()
    : arrwidth(0),
      isrel(false)
{
    arrelms.resize(MAX_ELMS);
}

/**
 * @brief Returns the fingerprint of the given byte value.
 *
 * @param idx Byte value in [0, MAX_ELMS); not bounds-checked.
 * @return Fingerprint of the value.
 */
const VHKeyFingerPrint & VHWideRangeScan::operator[](size_t idx) const
{
    return arrelms[idx];
}

/**
 * @brief Scans the buffer in absolute coordinates.
 *
 * @param arrsrc Source buffer; its size must be in [2, MAX_RANGE].
 * @return vok on success, an error status if the size is out of range.
 */
verr VHWideRangeScan::scanabs(const std::vector<uint8_t> &arrsrc)
{
    //
    if (arrsrc.size() < 2)
        return verror(1);

    //
    if (arrsrc.size() > VHKeyFingerPrint::MAX_RANGE)
        return verror(2);

    // Save for later analisys
    arrwidth = arrsrc.size();
    isrel = false;

    //
    for (size_t idx = 0; idx < arrsrc.size(); idx++)
    {
        uint8_t sym = arrsrc[idx];
        arrelms[sym].putabs(static_cast<uint16_t>(idx));
    }

    return vok;
}

/**
 * @brief Scans the buffer in relative coordinates.
 *
 * @param arrsrc Source buffer; its size must be in [2, MAX_RANGE].
 * @return vok on success, an error status if the size is out of range.
 */
verr VHWideRangeScan::scanrel(const std::vector<uint8_t> &arrsrc)
{
    //
    if (arrsrc.size() < 2)
        return verror(1);

    //
    if (arrsrc.size() > VHKeyFingerPrint::MAX_RANGE)
        return verror(2);

    // Save for later analisys
    arrwidth = arrsrc.size();
    isrel = true;

    //
    for (size_t idx = 0; idx < arrsrc.size(); idx++)
    {
        uint8_t sym = arrsrc[idx];
        arrelms[sym].putrel(static_cast<uint16_t>(idx));
    }

    return vok;
}
