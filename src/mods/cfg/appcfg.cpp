#include "cfg/appcfgstruct.hpp"

#include "argsparser.hpp"
#include "generatedna/generatedna.hpp"
#include "generatewnd/generatewnd.hpp"
#include "vhfile.hpp"
#include "vhfsys.hpp"


using namespace VHArgsParser;

/**
 * @brief Parses command-line arguments and fills the core configuration.
 *
 * @param argc Number of arguments passed to the program.
 * @param argv Array of arguments passed to the program.
 * @return vok on success, or an error status on failure.
 */
verr VHAppCore::ParseArgs(int argc, char *argv[])
{

    VHArgsParser::VHCmdLineParser cmdline;

    cmdline.setAppName("vhpackv81scan");
    cmdline.setAppDesc("PUSHK scan filter tool. V0.1.0");

    std::vector<VHArgTypeBase> arrcmd;

    arrcmd.push_back(VHArgTypeFileName("-fname", "", "Source file"));
    arrcmd.push_back(VHArgTypeInteger("-wndsz", "Window size", 128, 2, 256));

    // Wrong args or syntax error ?
    if (vok != cmdline.ParseCommandLine(arrcmd, argc, argv))
    {
        return verrmsg(1, "Error parsing command line");
    }

    std::string strfname = cmdline.GetFileName("-fname");

    if (strfname.empty())
    {
        std::cout << "Parse args issue" << std::endl;
        cmdline.ShowUsage(arrcmd);
        return verror(1);
    }

    //
    if(!VHFSYS::FileExist(strfname))
    {
        return verrmsg(3, "Wrong input file " + strfname);
    }

    fnameSource = strfname;

    std::string basefname = VHFSYS::ExtractFilename(strfname);

    fnameWND = basefname + "_wnd.svg";
    fnameDNAAbs = basefname + "_dna_abs.svg";
    fnameDNARel = basefname + "_dna_rel.svg";

    subsize = cmdline.GetInt("-wndsz");

    return vok;
}

/**
 * @brief Loads the source file and runs both wide-range scans.
 *
 * @return vok on success, or an error status on failure (source too
 * small, absolute or relative scan failed).
 */
verr VHAppCore::LoadAndScan()
{
    // Load source
    const std::vector<uint8_t> &arrsrc = datasource.LoadSource(fnameSource, subsize);

    if (arrsrc.size() < subsize)
        return verror(1);

    if (vok != wscanabs.scanabs(arrsrc))
        return verrmsg(1, "Wide Range (abs) analisys failed");

    if (vok != wscanrel.scanrel(arrsrc))
        return verrmsg(1, "Wide Range (rel) analisys failed");

    return vok;
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
    verr r = docwnd.GenerateDocument(datasource.data(), fnameWND, datasource.strSHA256(), datasource.filesize());
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
        fnameSource,
        fnameDNAAbs,
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
        fnameSource,
        fnameDNARel,
        datasource.strSHA256(),
        datasource.filesize());
    if (vok != r)
        return verrmsg(2, "Error generating docdna rel");

    return vok;
}
