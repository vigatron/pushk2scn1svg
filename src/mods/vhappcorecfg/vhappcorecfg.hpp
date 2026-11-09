#pragma once

#include <string>
#include <cinttypes>

#include "vhplatform.hpp"

/**
 * @brief Application core configuration (command-line parameters).
 */
class VHAppCoreCfg
{
    public:

    /**
     * @brief Parses command-line arguments and fills the configuration.
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

};
