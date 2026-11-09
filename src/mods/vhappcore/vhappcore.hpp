#pragma once

#include <string>
#include <cinttypes>

#include "vhplatform.hpp"

#include "datasource/vhsrcdataarray.hpp"
#include "vhappcorecfg/vhappcorecfg.hpp"
#include "widerange/widerange.hpp"

/**
 * @brief Application core analysis state.
 */
class VHAppCore
{
    public:

    /**
     * @brief Loads the source file and runs both wide-range scans.
     *
     * Loads the source file into the internal data source (see datasource)
     * using appcfg.fnameSource and appcfg.subsize set by
     * appcfg.ParseArgs, checks the loaded size, then runs the absolute
     * (wscanabs) and relative (wscanrel) wide-range scans over the
     * loaded data.
     *
     * @return vok on success, or an error status on failure (source too
     * small, absolute or relative scan failed).
     */
    verr LoadAndScan();

    /**
     * @brief Generates the window (WND) SVG document from the loaded source.
     *
     * Builds the WND document via VHDocWnd using the loaded source data
     * (datasource) and the output file name (appcfg.fnameWND) set by
     * appcfg.ParseArgs. Requires a prior successful LoadAndScan call.
     *
     * @return vok on success, or an error status on failure.
     */
    verr GenerateWnd();

    /**
     * @brief Generates the DNA ABS SVG document from the absolute scan.
     *
     * Builds the DNA document via VHDocDNA using the absolute wide-range
     * scan (wscanabs) and the output file name (appcfg.fnameDNAAbs) set by
     * appcfg.ParseArgs. Requires a prior successful LoadAndScan call.
     *
     * @return vok on success, or an error status on failure.
     */
    verr GenerateDNAAbs();

    /**
     * @brief Generates the DNA REL SVG document from the relative scan.
     *
     * Builds the DNA document via VHDocDNA using the relative wide-range
     * scan (wscanrel) and the output file name (appcfg.fnameDNARel) set by
     * appcfg.ParseArgs. Requires a prior successful LoadAndScan call.
     *
     * @return vok on success, or an error status on failure.
     */
    verr GenerateDNARel();

    // Configuration
    VHAppCoreCfg appcfg;

    // Source data
    VHSrcDataArray datasource;

    // WideRange Scan Abs
    VHWideRangeScan wscanabs;

    // WideRange Scan Rel
    VHWideRangeScan wscanrel;

};

