#include "dbg.hpp"
#include "vhfile.hpp"
#include "vhmemblk.hpp"

#include <string.h>
#include <vector>
#include <filesystem>

#include "args.hpp"
#include "vhrect.hpp"
#include "vhlibsvg/src/vhsvg.hpp"

#include "misc.hpp"
#include "global.hpp"
#include "draw.hpp"

// vhlibplatform
// vhlibfsys
// vhlibargs

// ----------------------------------------------------------------------------
static verr ParseArgs(int argc, char *argv[], stInParams *pInParams)
{

    VHArgsParser argsparser;

    argsparser.setappname("pack81scansvg");

    argsparser.addopt("s", "Block size");

    if (vok != argsparser.ParseArgs(argc, argv))
    {
        std::cout << "Parse args issue" << std::endl;
        argsparser.Usage();
        return verror(1);
    }

    if (!argsparser.single().size())
    {
        dbg.txtnl("USAGE: pack81scansvg <filename>");
        return verror(2);
    }

    pInParams->fname = argsparser.single()[0];
    if (!std::filesystem::exists(pInParams->fname) ||
        std::filesystem::is_directory(pInParams->fname))
    {
        std::string msg = "Wrong input file " + pInParams->fname;
        dbg.txtnl(msg.c_str());
        return verror(3);
    }

    pInParams->subsize = argsparser.getopt("s", 8);

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

    verr r = GenerateDocument(memblk, &sinParams);
    return (r == vok) ? 0 : 1;
}
