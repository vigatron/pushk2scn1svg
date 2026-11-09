#include "vhplatform.hpp"

#include "vhsvg.hpp"

#include "global.hpp"
#include "draw/draw.hpp"

// Application Core (command line parsing, source loading and SVG generation inside VHAppCore)
#include "cfg/appcfgstruct.hpp"

/**
 *
 */
int main(int argc, char *argv[])
{
    VHAppCore appcore;

    if (vok != appcore.ParseArgs(argc, argv))
        return verror(1);

    if (vok != appcore.LoadAndScan())
        return verror(1);

    if (vok != appcore.GenerateWnd())
        return verror(1);

    if (vok != appcore.GenerateDNAAbs())
        return verror(1);

    if (vok != appcore.GenerateDNARel())
        return verror(1);

    return vok;
}
