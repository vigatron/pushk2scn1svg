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
     * @brief Loads the source file into the internal data source.
     *
     * Loads appcfg.fnameSource into datasource using appcfg.subsize as the
     * minimum accepted size. Both fields must be set by appcfg.ParseArgs
     * before the call.
     *
     * @return vok on success, or an error status if the file cannot be
     * loaded or is smaller than appcfg.subsize.
     */
    verr LoadSource();

    /**
     * @brief Runs the absolute wide-range scan over the loaded source.
     *
     * Fills wscanabs with absolute positions of every byte value. Requires a
     * prior successful LoadSource call.
     *
     * @return vok on success, or an error status if the source is not loaded
     * or the scan fails.
     */
    verr ScanAbs();

    /**
     * @brief Runs the relative wide-range scan over the loaded source.
     *
     * Fills wscanrel with offsets from the first occurrence of every byte
     * value. Requires a prior successful LoadSource call.
     *
     * @return vok on success, or an error status if the source is not loaded
     * or the scan fails.
     */
    verr ScanRel();

    /**
     * @brief Runs both wide-range scans over the loaded source.
     *
     * Convenience wrapper for ScanAbs followed by ScanRel. Requires a prior
     * successful LoadSource call. If ScanAbs fails, ScanRel is not started.
     *
     * @return vok if both scans succeed, otherwise the first error status.
     */
    verr ScanAll();

    /**
     * @brief Generates the window (WND) SVG document from the loaded source.
     *
     * Builds the WND document via VHDocWnd using the loaded source data
     * (datasource) and the output file name (appcfg.fnameWND) set by
     * appcfg.ParseArgs. Requires a prior successful LoadSource call; no
     * wide-range scan is needed.
     *
     * @return vok on success, or an error status on failure.
     */
    verr GenerateWnd();

    /**
     * @brief Generates the DNA ABS SVG document from the absolute scan.
     *
     * Builds the DNA document via VHDocDNA using the absolute wide-range
     * scan (wscanabs) and the output file name (appcfg.fnameDNAAbs) set by
     * appcfg.ParseArgs. Requires prior successful LoadSource and ScanAbs
     * calls.
     *
     * @return vok on success, or an error status on failure.
     */
    verr GenerateDNAAbs();

    /**
     * @brief Generates the DNA REL SVG document from the relative scan.
     *
     * Builds the DNA document via VHDocDNA using the relative wide-range
     * scan (wscanrel) and the output file name (appcfg.fnameDNARel) set by
     * appcfg.ParseArgs. Requires prior successful LoadSource and ScanRel
     * calls.
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

