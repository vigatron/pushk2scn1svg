#pragma once

#include "vhplatform.hpp"

#include <array>
#include <vector>

#include "vhkeyfingerprint.hpp"

/**
 * @brief Bit fingerprints of 256 byte values over a scanned buffer.
 *
 * One VHKeyFingerPrint is kept per byte value. Scanning is performed either
 * in absolute coordinates (scanabs) or in relative coordinates (scanrel);
 * the selected space is reported by relative() and must be respected by
 * consumers that query fingerprint bits.
 */
class VHWideRangeScan
{
public:
    /**
     * @brief Creates an empty scan with MAX_ELMS zeroed fingerprints.
     */
    VHWideRangeScan();

    /**
     * @brief Scans the buffer in absolute coordinates.
     *
     * @param arrsrc Source buffer; its size must be in [2, MAX_RANGE].
     * @return vok on success, an error status if the size is out of range.
     */
    verr scanabs(const std::vector<uint8_t> &arrsrc);

    /**
     * @brief Scans the buffer in relative coordinates.
     *
     * @param arrsrc Source buffer; its size must be in [2, MAX_RANGE].
     * @return vok on success, an error status if the size is out of range.
     */
    verr scanrel(const std::vector<uint8_t> &arrsrc);

    /**
     * @brief Returns true when the last scan used relative coordinates.
     *
     * @return Coordinate space of the scan results.
     */
    bool relative() const { return isrel; }

    /// Number of fingerprints (one per byte value).
    static const size_t MAX_ELMS = 256;

    /**
     * @brief Returns the fingerprint of the given byte value.
     *
     * @param idx Byte value in [0, MAX_ELMS); not bounds-checked.
     * @return Fingerprint of the value.
     */
    const VHKeyFingerPrint &operator[](size_t idx) const;

    /**
     * @brief Returns the width of the scanned buffer.
     *
     * @return Number of bytes passed to the last scan, 0 before any scan.
     */
    size_t width() const { return arrwidth; }

private:
    std::vector<VHKeyFingerPrint> arrelms;

    size_t arrwidth;

    bool isrel;

    /**
     * @brief Prints all fingerprints to stdout (diagnostic helper).
     */
    void dump()
    {
        // Results
        for (size_t sym = 0; sym < MAX_ELMS; sym++)
        {
            // const VHKeyFingerPrint &obj = wobj(sym);

            printf("#%.3d : ", (uint8_t)sym);

            if (arrelms[sym].valid())
            {
                arrelms[sym].dump();
            }
            else
            {
                printf("---");
            }
            printf("\n");
        }
    }
};
