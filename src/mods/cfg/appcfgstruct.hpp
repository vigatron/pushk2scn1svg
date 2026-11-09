#pragma once

#include <string>
#include <cinttypes>

#include "vhplatform.hpp"

#include "widerange/widerange.hpp"

/**
 * @brief Application core configuration and analysis state.
 */
class VHAppCore
{
    public:

    /**
     * @brief Parses command-line arguments and fills the core configuration.
     *
     * @param argc Number of arguments passed to the program.
     * @param argv Array of arguments passed to the program.
     * @return vok on success, or an error status on failure.
     */
    verr ParseArgs(int argc, char *argv[]);

    std::string fnameSource;
    std::string fnameWND;
    std::string fnameDNAAbs;
    std::string fnameDNARel;

    uint16_t subsize;

    // WideRange Scan Abs
    VHWideRangeScan wscanabs;

    // WideRange Scan Rel
    VHWideRangeScan wscanrel;

};

