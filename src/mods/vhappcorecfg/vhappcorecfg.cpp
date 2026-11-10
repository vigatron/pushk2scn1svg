#include "vhappcorecfg/vhappcorecfg.hpp"

#include <filesystem>

#include "app_version.hpp"
#include "argsparser.hpp"
#include "vhfile.hpp"
#include "vhfsys.hpp"


using namespace VHArgsParser;

/**
 * @brief Joins an output folder and an output file name.
 *
 * An empty folder means the current working directory, so the file name
 * is returned unchanged. Otherwise the name is appended to the folder
 * using std::filesystem path rules.
 *
 * @param dir Output folder (may be empty).
 * @param fname Output file name.
 * @return Path to the output file.
 */
static std::string MakeOutputPath(const std::string &dir, const std::string &fname)
{
    if (dir.empty())
        return fname;

    return (std::filesystem::path(dir) / fname).string();
}

/**
 * @brief Ensures that the output folder exists, creating it when necessary.
 *
 * Uses VHFSYS::CreateNestedDir, which creates all missing parent
 * directories. Exceptions thrown by std::filesystem are converted into
 * an error status.
 *
 * @param dir Output folder (may be empty).
 * @return vok if the folder exists or was created, or an error status.
 */
static verr EnsureOutputFolder(const std::string &dir)
{
    if (dir.empty())
        return vok;

    try
    {
        return VHFSYS::CreateNestedDir(dir);
    }
    catch (const std::filesystem::filesystem_error &)
    {
        return verror(1);
    }
}

/**
 * @brief Parses command-line arguments and fills the configuration.
 *
 * @param argc Number of arguments passed to the program.
 * @param argv Array of arguments passed to the program.
 * @return vok on success, or an error status on failure.
 */
verr VHAppCoreCfg::ParseArgs(int argc, char *argv[])
{

    VHArgsParser::VHCmdLineParser cmdline;

    cmdline.setAppName("pushk2scn1svg");
    cmdline.setAppDesc(std::string("scan filter tool. V") + APP_VERSION + "\n");

    std::vector<VHArgTypeBase> arrcmd;

    arrcmd.push_back(VHArgTypeFileName("-fname", "", "Source file"));
    arrcmd.push_back(VHArgTypeInteger("-wndsz", "Window size", 128, 2, 256));
    arrcmd.push_back(VHArgTypeString("-outfolder", "", "Output Files folder"));

    // No arguments: show usage once and fail. VHCmdLineParser would also
    // print usage and return vok, which would make ParseArgs print it again.
    if (argc < 2)
    {
        cmdline.ShowUsage(arrcmd);
        return verror(1);
    }

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

    std::string stroutfolder = cmdline.GetString("-outfolder");

    if (vok != EnsureOutputFolder(stroutfolder))
    {
        return verrmsg(4, "Can't create output folder " + stroutfolder);
    }

    fnameSource = strfname;
    dirOutput = stroutfolder;

    std::string basefname = VHFSYS::ExtractFilename(strfname);

    fnameWND = MakeOutputPath(dirOutput, basefname + "_wnd.svg");
    fnameDNAAbs = MakeOutputPath(dirOutput, basefname + "_dna_abs.svg");
    fnameDNARel = MakeOutputPath(dirOutput, basefname + "_dna_rel.svg");

    subsize = cmdline.GetInt("-wndsz");

    return vok;
}
