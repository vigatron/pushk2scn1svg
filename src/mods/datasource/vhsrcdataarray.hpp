#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Loads and stores a source data array.
 */
class VHSrcDataArray
{
public:
    /**
     * @brief Loads a source file into the internal buffer with a minimum size check.
     *
     * @param fname Path to the source file.
     * @param minsize Minimum required file size in bytes.
     * @return Reference to the loaded data buffer; empty if loading fails or the file is too small.
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
