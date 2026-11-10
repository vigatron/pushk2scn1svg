#pragma once

#include <string>

#include "vhplatform.hpp"
#include "vhsvg.hpp"

/**
 * @brief Helpers for producing XML-safe and verifiable SVG output.
 */
class VHSvgTools
{
public:
    /**
     * @brief Escapes text for use in XML text nodes and attributes.
     *
     * Replaces `&`, `<`, `>`, `"` and `'` with their XML entities and
     * substitutes XML-forbidden control characters with `?`.
     *
     * @param text Source text.
     * @return Escaped text safe for XML output.
     */
    static std::string EscapeXml(const std::string &text);

    /**
     * @brief Saves an SVG document and verifies the written file.
     *
     * VHSVG::savetosvg() returns void and does not report write errors.
     * This wrapper checks that the output file can be created, saves the
     * document, then verifies that the file is non-empty and ends with the
     * closing `</svg>` tag.
     *
     * @param svg Document to save.
     * @param fname Output file path.
     * @return vok on success, or an error status if the file cannot be
     * created, is empty or is truncated.
     */
    static verr Save(VHSVG &svg, const std::string &fname);
};
