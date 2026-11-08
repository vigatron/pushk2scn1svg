#pragma once

#include "vhplatform.hpp"
#include "mem/vhmemblk.hpp"

#include <string>

/**
 * @brief Utilities for loading and working with source data arrays.
 */
class VHSrcDataArray
{
public:
    /**
     * @brief Loads a source file into a memory block with a minimum size check.
     *
     * @param fname Path to the source file.
     * @param minsize Minimum required file size in bytes.
     * @param r Destination memory block to fill.
     * @return vok on success, or an error status on failure.
     */
    static verr LoadSource(const std::string &fname, int minsize, VHMemBlock *r);
};
