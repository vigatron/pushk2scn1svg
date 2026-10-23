#include "vhplatform.hpp"


#include "dbg.hpp"

#include "vhfile.hpp"
#include "vhfsys.hpp"

#include "mem/vhmemblk.hpp"

#include "vhrect.hpp"

#include "vhsvg.hpp"

#include "global.hpp"
#include "draw/draw.hpp"


// Generate SVG
#include "generatewnd/generatewnd.hpp"
#include "generatedna/generatedna.hpp"

// Command Line Parsing
#include "cmdline/cmdline.hpp"

// ----------------------------------------------------------------------------
verr LoadSource(std::string fname, int minsize, VHMemBlock *r)
{

    VHFile f(fname);

    int fsize = f.flen();
    if (fsize < minsize)
    {
        std::string msg = "Minimal size less than " + std::to_string(minsize);
        dbg.txtnl(msg.c_str());
        return verror(3);
    }

    if (!r->LoadFromFile(fname.c_str()))
    {
        std::string msg = "Cant read file " + std::to_string(minsize);
        dbg.txtnl(msg.c_str());
        return verror(4);
    }

    return vok;
}


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

    // Convert source to std::vector
    std::vector<uint8_t> arrsrc(memblk.ptr(), memblk.ptr() + memblk.sz());

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
