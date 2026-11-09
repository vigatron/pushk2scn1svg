#include "cfg/appcfgstruct.hpp"

#include "argsparser.hpp"
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
