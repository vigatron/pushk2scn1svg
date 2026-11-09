#pragma once

#include <string>
#include <cinttypes>

#include "vhplatform.hpp"

#include "datasource/vhsrcdataarray.hpp"
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

    /**
     * @brief Loads the source file and runs both wide-range scans.
     *
     * Loads the source file into the internal data source (see datasource)
     * using fnameSource and subsize set by ParseArgs, checks the loaded
     * size, then runs the absolute (wscanabs) and relative (wscanrel)
     * wide-range scans over the loaded data.
     *
     * @return vok on success, or an error status on failure (source too
     * small, absolute or relative scan failed).
     */
    verr LoadAndScan();

    /**
     * @brief Generates the window (WND) SVG document from the loaded source.
     *
     * Builds the WND document via VHDocWnd using the loaded source data
     * (datasource) and the output file name (fnameWND) set by ParseArgs.
     * Requires a prior successful LoadAndScan call.
     *
     * @return vok on success, or an error status on failure.
     */
    verr GenerateWnd();

    /**
     * @brief Generates the DNA ABS SVG document from the absolute scan.
     *
     * Builds the DNA document via VHDocDNA using the absolute wide-range
     * scan (wscanabs) and the output file name (fnameDNAAbs) set by
     * ParseArgs. Requires a prior successful LoadAndScan call.
     *
     * @return vok on success, or an error status on failure.
     */
    verr GenerateDNAAbs();

    /**
     * @brief Generates the DNA REL SVG document from the relative scan.
     *
     * Builds the DNA document via VHDocDNA using the relative wide-range
     * scan (wscanrel) and the output file name (fnameDNARel) set by
     * ParseArgs. Requires a prior successful LoadAndScan call.
     *
     * @return vok on success, or an error status on failure.
     */
    verr GenerateDNARel();

    std::string fnameSource;
    std::string fnameWND;
    std::string fnameDNAAbs;
    std::string fnameDNARel;

    uint16_t subsize;

    // Source data
    VHSrcDataArray datasource;

    // WideRange Scan Abs
    VHWideRangeScan wscanabs;

    // WideRange Scan Rel
    VHWideRangeScan wscanrel;

};

