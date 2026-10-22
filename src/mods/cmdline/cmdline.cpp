#include "cmdline.hpp"

#include "vhfile.hpp"
#include "vhfsys.hpp"


using namespace VHArgsParser;

/**
 *
 */
verr ParseArgs(int argc, char *argv[], sAppCfg  & appcfg)
{

    VHArgsParser::VHCmdLineParser cmdline;
    verr parse_status;

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

    appcfg.fnameSource = strfname;

    std::string basefname = VHFSYS::ExtractFilename(strfname);

    appcfg.fnameWND = basefname + "_wnd.svg";
    appcfg.fnameDNA = basefname + "_dna.svg";

    appcfg.subsize = cmdline.GetInt("-wndsz");

    return vok;
}
