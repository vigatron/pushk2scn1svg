#include "vhplatform.hpp"

// Application Core (command line parsing, source loading and SVG generation inside VHAppCore)
#include "vhappcore/vhappcore.hpp"

/**
 *
 */
int main(int argc, char *argv[])
{
    VHAppCore appcore;

    if (vok != appcore.appcfg.ParseArgs(argc, argv))
        return verror(1);

    if (vok != appcore.LoadSource())
        return verror(3);

    if (vok != appcore.ScanAbs())
        return verror(4);

    if (vok != appcore.ScanRel())
        return verror(5);

    if (vok != appcore.GenerateWnd())
        return verror(6);

    if (vok != appcore.GenerateDNAAbs())
        return verror(7);

    if (vok != appcore.GenerateDNARel())
        return verror(8);

    return vok;
}
