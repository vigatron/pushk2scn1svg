#include "vhappcorecfg/vhappcorecfg.hpp"

#include <charconv>
#include <exception>
#include <filesystem>
#include <string_view>
#include <system_error>

#include "app_version.hpp"
#include "argsparser.hpp"
#include "datasource/vhsrcdataarray.hpp"
#include "vhfsys.hpp"


using namespace VHArgsParser;

/**
 * @brief Checks that a token is a plain decimal integer.
 *
 * The vhlibargs parser uses std::stoi(), which accepts trailing garbage
 * ("2x" becomes 2) and throws on non-numeric input. This pre-check uses
 * std::from_chars() so malformed values are rejected without exceptions.
 *
 * @param text Token to check.
 * @return true if the whole token is a decimal integer.
 */
static bool IsStrictInteger(const std::string &text)
{
    if (text.empty())
        return false;

    int value = 0;
    const char *begin = text.data();
    const char *end = begin + text.size();
    const std::from_chars_result res = std::from_chars(begin, end, value);
    return res.ec == std::errc() && res.ptr == end;
}

/**
 * @brief Validates the -wndsz value before the generic parser sees it.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line arguments.
 * @param badvalue Receives the rejected token on failure.
 * @return true if -wndsz is absent, has no value, or its value is a plain
 * decimal integer; false otherwise.
 */
static bool ValidateWindowSize(int argc, char *argv[], std::string &badvalue)
{
    for (int i = 1; i + 1 < argc; ++i)
    {
        if (std::string_view(argv[i]) != "-wndsz")
            continue;

        badvalue = argv[i + 1];
        return IsStrictInteger(badvalue);
    }

    return true;
}

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

    arrcmd.push_back(VHArgTypeString("-fname", "", "Source file"));
    arrcmd.push_back(VHArgTypeInteger("-wndsz", "Window size", 128, 2, 256));
    arrcmd.push_back(VHArgTypeString("-outfolder", "", "Output Files folder"));

    // No arguments: show usage once and fail. VHCmdLineParser would also
    // print usage and return vok, which would make ParseArgs print it again.
    if (argc < 2)
    {
        cmdline.ShowUsage(arrcmd);
        return verror(1);
    }

    // std::stoi inside vhlibargs accepts trailing characters and throws on
    // non-numeric input; validate the only integer option up front.
    std::string badwndsz;
    if (!ValidateWindowSize(argc, argv, badwndsz))
    {
        std::cout << "Invalid value for -wndsz: " << badwndsz
                  << " (expected integer 2..256)" << std::endl;
        return verror(1);
    }

    // Wrong args or syntax error ?
    try
    {
        if (vok != cmdline.ParseCommandLine(arrcmd, argc, argv))
        {
            return verrmsg(1, "Error parsing command line");
        }
    }
    catch (const std::exception &)
    {
        return verrmsg(1, "Error parsing command line");
    }

    std::string strfname = cmdline.GetString("-fname");

    if (strfname.empty())
    {
        std::cout << "Parse args issue" << std::endl;
        cmdline.ShowUsage(arrcmd);
        return verror(1);
    }

    // A source must be an existing regular file; directories, devices and
    // other special files are rejected before any output folder is created.
    std::error_code ec;
    if (!std::filesystem::is_regular_file(strfname, ec) || ec)
    {
        return verrmsg(3, "Wrong input file " + strfname);
    }

    const std::uintmax_t srcsize = std::filesystem::file_size(strfname, ec);
    if (ec)
    {
        return verrmsg(3, "Wrong input file " + strfname);
    }

    if (srcsize > VHSrcDataArray::MAX_SOURCE_SIZE)
    {
        return verrmsg(5, "Source file too large " + strfname + " (max " +
            std::to_string(VHSrcDataArray::MAX_SOURCE_SIZE) + " bytes)");
    }

    std::string stroutfolder = cmdline.GetString("-outfolder");

    if (vok != EnsureOutputFolder(stroutfolder))
    {
        return verrmsg(4, "Can't create output folder " + stroutfolder);
    }

    fnameSource = strfname;
    dirOutput = stroutfolder;

    std::string basefname = std::filesystem::path(strfname).filename().string();

    fnameWND = MakeOutputPath(dirOutput, basefname + "_wnd.svg");
    fnameDNAAbs = MakeOutputPath(dirOutput, basefname + "_dna_abs.svg");
    fnameDNARel = MakeOutputPath(dirOutput, basefname + "_dna_rel.svg");

    subsize = static_cast<uint16_t>(cmdline.GetInt("-wndsz"));

    return vok;
}
