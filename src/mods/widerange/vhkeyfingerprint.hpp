#pragma once

#include "vhplatform.hpp"

#include <vector>

/**
 * @brief Bit fingerprint of the positions of one byte value.
 *
 * Stores, for a single byte value, the set of its positions in the scanned
 * buffer. Positions are kept either in absolute coordinates (offsets from the
 * start of the buffer, see putabs) or in relative coordinates (offsets from
 * the first occurrence of the same value, see putrel). The coordinate space is
 * fixed by the first put* call and reported by isRelative().
 *
 * The bit array is always indexed in the selected coordinate space, so
 * status(pos) must be queried with the same kind of position that was stored.
 */
class VHKeyFingerPrint
{
public:
    /**
     * @brief Creates an empty fingerprint with a zeroed bit set.
     */
    VHKeyFingerPrint();

    /**
     * @brief Registers an absolute position of the value.
     *
     * The first call fixes the coordinate space to absolute, stores the
     * position as the start/end of the range and marks the bit.
     *
     * @param pos Absolute offset from the start of the scanned buffer.
     */
    void putabs(uint16_t pos);

    /**
     * @brief Registers a relative position of the value.
     *
     * The first call fixes the coordinate space to relative, stores the
     * absolute position of the first occurrence as the range start, marks
     * relative offset 0, and the following calls mark pos - startpos.
     *
     * @param pos Absolute offset of the occurrence in the scanned buffer.
     */
    void putrel(uint16_t pos);

    /**
     * @brief Sets the bit at the given position in the scan coordinate space.
     *
     * @param pos Absolute position for absolute fingerprints, relative offset
     *            for relative fingerprints.
     */
    void mark(uint16_t pos);

    /**
     * @brief Returns the bit at the given position in the scan coordinate space.
     *
     * @param pos Absolute position for absolute fingerprints, relative offset
     *            for relative fingerprints.
     * @return true if the position is marked, false otherwise.
     */
    bool status(uint16_t pos) const;

    /**
     * @brief Returns true if at least one position was registered.
     *
     * @return true if the fingerprint contains data.
     */
    bool valid() const;

    /**
     * @brief Returns true if no position was registered.
     *
     * @return true if the fingerprint is empty.
     */
    bool empty() const;

    /**
     * @brief Prints the range and all marked positions to stdout.
     */
    void dump() const;

    /**
     * @brief Returns the absolute position of the first occurrence.
     *
     * @return First occurrence position, 0 for an empty fingerprint.
     */
    uint16_t getStartPos() const { return startpos; }

    /**
     * @brief Returns the absolute position of the last occurrence.
     *
     * @return Last occurrence position, 0 for an empty fingerprint.
     */
    uint16_t getEndPos() const { return endpos; }

    /**
     * @brief Returns the distance between the last and the first occurrence.
     *
     * @return Range width in bytes, 0 for an empty fingerprint.
     */
    uint16_t getSpan() const { return static_cast<uint16_t>(endpos - startpos); }

    /**
     * @brief Returns the coordinate space used by the fingerprint.
     *
     * @return true for relative offsets, false for absolute positions.
     */
    bool isRelative() const { return relmode; }

    /// Maximum number of positions addressable by the bit set.
    static const size_t MAX_RANGE = 32 * F1K;

private:
    bool avail;
    bool relmode;
    uint16_t startpos;
    uint16_t endpos;

    std::vector<uint8_t> arr;
};
