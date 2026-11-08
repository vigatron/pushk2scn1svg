#include "vhplatform.hpp"


#include "mem/vhmemblk.hpp"

#include "vhsvg.hpp"

#include "global.hpp"
#include "draw/draw.hpp"

#include "datasource/vhsrcdataarray.hpp"

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
    VHAppCore appcore;

    if (vok != ParseArgs(argc, argv, appcore))
        return verror(1);

    // Load source
    VHMemBlock memblk;
    if (vok != VHSrcDataArray::LoadSource(appcore.fnameSource, appcore.subsize, &memblk))
        return verror(1);

    // Convert source to std::vector
    std::vector<uint8_t> arrsrc(memblk.ptr(), memblk.ptr() + memblk.sz());

    if (vok != appcore.wscanabs.scanabs(arrsrc))
        return verrmsg(1, "Wide Range (abs) analisys failed");

    if (vok != appcore.wscanrel.scanrel(arrsrc))
        return verrmsg(1, "Wide Range (rel) analisys failed");

    // Generate Wnd Doc / SVG
    {
        VHDocWnd docwnd;
        verr r = docwnd.GenerateDocument(arrsrc, appcore.fnameWND);
        if (vok != r)
            return verrmsg(1, "Error generating docwnd");
    }

    // Generate DNA Doc Abs / SVG
    {
        VHDocDNA docdna;
        verr r = docdna.GenerateDocument(appcore.wscanabs, appcore.fnameDNAAbs);
        if (vok != r)
            return verrmsg(2, "Error generating docdna abs");
    }

    // Generate DNA Doc Rel / SVG
    {
        VHDocDNA docdna;
        verr r = docdna.GenerateDocument(appcore.wscanrel, appcore.fnameDNARel);
        if (vok != r)
            return verrmsg(2, "Error generating docdna rel");
    }

    return vok;
}
