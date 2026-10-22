
#include "dbg.hpp"

#include "vhfile.hpp"
#include "vhfsys.hpp"

#include "vhmemblk.hpp"

#include <string.h>
#include <vector>

#include "vhrect.hpp"

#include "vhsvg.hpp"

#include "misc.hpp"
#include "global.hpp"
#include "draw.hpp"

#include "widerange/widerange.hpp"

#include "argsparser.hpp"

using namespace VHArgsParser;

// ----------------------------------------------------------------------------
static verr ParseArgs(int argc, char *argv[], stInParams *pInParams)
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

    pInParams->fname = strfname;
    pInParams->subsize = cmdline.GetInt("-wndsz");

    return vok;
}

// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{

    stInParams sinParams;
    if (vok != ParseArgs(argc, argv, &sinParams))
        return verror(1);

    // Load source
    VHMemBlock memblk;
    if (vok != LoadSource(sinParams.fname, sinParams.subsize, &memblk))
        return verror(1);

    // WideRange Scan
    VHWideRangeScan wscan;
    std::vector<uint8_t> arrsrc(memblk.ptr(), memblk.ptr() + memblk.sz());
    if (vok != wscan.scan(arrsrc))
        return verrmsg(1, "Wide Range analisys failed");

    // Generate SVG
    verr r = GenerateDocument(memblk, &sinParams);
    return (r == vok) ? 0 : 1;
}
