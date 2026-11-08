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

private:
    std::vector<uint8_t> arrsrc;
};
