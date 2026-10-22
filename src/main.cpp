
#include "dbg.hpp"

#include "vhfile.hpp"
#include "vhfsys.hpp"

#include "vhmemblk.hpp"

#include <string.h>
#include <vector>

#include "vhrect.hpp"

#include "vhsvg.hpp"

#include "global.hpp"
#include "draw.hpp"

#include "widerange/widerange.hpp"

// Generate SVG
#include "generatewnd/generatewnd.hpp"
#include "generatedna/generatedna.hpp"

// Command Line Parsing
#include "cmdline/cmdline.hpp"

/**
 *
 */
int main(int argc, char *argv[])
{
    sAppCfg appcfg;

    if (vok != ParseArgs(argc, argv, appcfg))
        return verror(1);

    // Load source
    VHMemBlock memblk;
    if (vok != LoadSource(appcfg.fnameSource, appcfg.subsize, &memblk))
        return verror(1);

    // WideRange Scan
    VHWideRangeScan wscan;
    std::vector<uint8_t> arrsrc(memblk.ptr(), memblk.ptr() + memblk.sz());

    if (vok != wscan.scan(arrsrc))
        return verrmsg(1, "Wide Range analisys failed");

    // Generate Wnd Doc / SVG
    {
        VHDocWnd docwnd;
        verr r = docwnd.GenerateDocument(arrsrc, appcfg.fnameWND);
        if (vok != r)
            return verrmsg(1, "Error generating docwnd");
    }

    // Generate DNA Doc / SVG
    {
        VHDocDNA docdna;
        verr r = docdna.GenerateDocument(arrsrc, appcfg.fnameDNA);
        if (vok != r)
            return verrmsg(2, "Error generating docdna");
    }

    return vok;
}
