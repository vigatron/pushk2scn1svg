#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "vhplatform.hpp"

/**
 * @brief Loads and stores a source data array.
 */
class VHSrcDataArray
{
public:
    /**
     * @brief Maximum supported source size in bytes (32 KiB).
     *
     * The limit follows from storing positions as uint16_t bit indices in
     * VHKeyFingerPrint. Larger files are rejected before allocation.
     */
    static constexpr size_t MAX_SOURCE_SIZE = F32K;

    /**
     * @brief Loads a source file into the internal buffer with size checks.
     *
     * The file is read in binary mode. Its size must be in
     * [minsize, MAX_SOURCE_SIZE]; otherwise the buffer and the SHA-256
     * string are left empty and the stored file size is reset to 0.
     *
     * @param fname Path to the source file.
     * @param minsize Minimum required file size in bytes.
     * @return Reference to the loaded data buffer; empty if loading fails,
     * the file is too small or too large.
     */
    const std::vector<uint8_t> &LoadSource(const std::string &fname, int minsize);

    /**
     * @brief Returns the currently loaded source data.
     *
     * @return Reference to the internal data buffer.
     */
    const std::vector<uint8_t> &data() const { return arrsrc; }

    /**
     * @brief Returns the source file SHA256 hash as a hexadecimal string.
     *
     * @return SHA256 hash of the loaded source data.
     */
    const std::string &strSHA256() const { return strsha256; }

    /**
     * @brief Returns the loaded source file size in bytes.
     *
     * @return Source file size in bytes.
     */
    size_t filesize() const { return fsize; }

private:
    std::vector<uint8_t> arrsrc;

    std::string strsha256;

    size_t fsize = 0;
};
