#include "svgtools/svgtools.hpp"

#include <cctype>
#include <fstream>
#include <ios>

/**
 * @brief Escapes text for use in XML text nodes and attributes.
 *
 * @param text Source text.
 * @return Escaped text safe for XML output.
 */
std::string VHSvgTools::EscapeXml(const std::string &text)
{
    std::string result;
    result.reserve(text.size());

    for (const char ch : text)
    {
        const unsigned char code = static_cast<unsigned char>(ch);

        switch (ch)
        {
        case '&':
            result += "&amp;";
            break;
        case '<':
            result += "&lt;";
            break;
        case '>':
            result += "&gt;";
            break;
        case '"':
            result += "&quot;";
            break;
        case '\'':
            result += "&apos;";
            break;
        default:
            // XML 1.0 forbids most control characters.
            if (code < 0x20 && ch != '\t' && ch != '\n' && ch != '\r')
                result += '?';
            else
                result += ch;
            break;
        }
    }

    return result;
}

/**
 * @brief Saves an SVG document and verifies the written file.
 *
 * @param svg Document to save.
 * @param fname Output file path.
 * @return vok on success, or an error status if the file cannot be
 * created, is empty or is truncated.
 */
verr VHSvgTools::Save(VHSVG &svg, const std::string &fname)
{
    // VHSVG::savetosvg() ignores open/write failures and may leave a stale
    // file in place. Truncate the target first so a failed save cannot be
    // mistaken for the previous successful document.
    {
        std::ofstream probe(fname, std::ios::binary | std::ios::trunc);
        if (!probe)
            return verror(1);

        probe.close();
        if (probe.fail())
            return verror(1);
    }

    svg.savetosvg(fname);

    std::ifstream in(fname, std::ios::binary | std::ios::ate);
    if (!in)
        return verror(2);

    const std::streamoff size = in.tellg();
    if (size <= 0)
        return verror(2);

    // savetosvg() terminates the document with "</svg>\n"; inspect only the
    // tail instead of reading the whole file back.
    const std::streamoff tail = (size < 16) ? size : 16;
    in.seekg(size - tail, std::ios::beg);
    if (!in)
        return verror(3);

    std::string buf(static_cast<size_t>(tail), '\0');
    in.read(buf.data(), tail);
    if (!in)
        return verror(3);

    static const std::string closing = "</svg>";
    const size_t pos = buf.find(closing);
    if (pos == std::string::npos)
        return verror(4);

    for (size_t i = pos + closing.size(); i < buf.size(); ++i)
    {
        if (!std::isspace(static_cast<unsigned char>(buf[i])))
            return verror(5);
    }

    return vok;
}
