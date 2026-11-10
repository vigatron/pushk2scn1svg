#include "vhappcore/vhappcore.hpp"

#include "generatedna/generatedna.hpp"
#include "generatewnd/generatewnd.hpp"

/**
 * @brief Loads the source file into the internal data source.
 *
 * @return vok on success, or an error status on failure.
 */
verr VHAppCore::LoadSource()
{
    // Load source
    const std::vector<uint8_t> &arrsrc =
        datasource.LoadSource(appcfg.fnameSource, appcfg.subsize);

    if (arrsrc.size() < appcfg.subsize)
        return verror(3);

    return vok;
}

/**
 * @brief Runs the absolute wide-range scan over the loaded source.
 *
 * @return vok on success, or an error status on failure.
 */
verr VHAppCore::ScanAbs()
{
    if (datasource.data().empty())
        return verrmsg(4, "Source data is not loaded");

    if (vok != wscanabs.scanabs(datasource.data()))
        return verrmsg(4, "Wide Range (abs) analisys failed");

    return vok;
}

/**
 * @brief Runs the relative wide-range scan over the loaded source.
 *
 * @return vok on success, or an error status on failure.
 */
verr VHAppCore::ScanRel()
{
    if (datasource.data().empty())
        return verrmsg(5, "Source data is not loaded");

    if (vok != wscanrel.scanrel(datasource.data()))
        return verrmsg(5, "Wide Range (rel) analisys failed");

    return vok;
}

/**
 * @brief Runs both wide-range scans over the loaded source.
 *
 * @return vok if both scans succeed, otherwise the first error status.
 */
verr VHAppCore::ScanAll()
{
    const verr r = ScanAbs();
    if (vok != r)
        return r;

    return ScanRel();
}

/**
 * @brief Generates the window (WND) SVG document from the loaded source.
 *
 * @return vok on success, or an error status on failure.
 */
verr VHAppCore::GenerateWnd()
{
    // Generate Wnd Doc / SVG
    VHDocWnd docwnd;
    verr r = docwnd.GenerateDocument(datasource.data(), appcfg.fnameWND, datasource.strSHA256(), datasource.filesize());
    if (vok != r)
        return verrmsg(1, "Error generating docwnd");

    return vok;
}

/**
 * @brief Generates the DNA ABS SVG document from the absolute scan.
 *
 * @return vok on success, or an error status on failure.
 */
verr VHAppCore::GenerateDNAAbs()
{
    // Generate DNA Doc Abs / SVG
    VHDocDNA docdna;
    verr r = docdna.GenerateDocument(
        "DNA ABS",
        wscanabs,
        appcfg.fnameSource,
        appcfg.fnameDNAAbs,
        datasource.strSHA256(),
        datasource.filesize());
    if (vok != r)
        return verrmsg(2, "Error generating docdna abs");

    return vok;
}

/**
 * @brief Generates the DNA REL SVG document from the relative scan.
 *
 * @return vok on success, or an error status on failure.
 */
verr VHAppCore::GenerateDNARel()
{
    // Generate DNA Doc Rel / SVG
    VHDocDNA docdna;
    verr r = docdna.GenerateDocument(
        "DNA REL",
        wscanrel,
        appcfg.fnameSource,
        appcfg.fnameDNARel,
        datasource.strSHA256(),
        datasource.filesize());
    if (vok != r)
        return verrmsg(2, "Error generating docdna rel");

    return vok;
}
