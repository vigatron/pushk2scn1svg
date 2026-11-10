#pragma once

#include "vhplatform.hpp"
#include "vhsvg.hpp"

#include "widerange/widerange.hpp"

class VHDocDNA
{
public:
    /**
     * @brief Generates a DNA document SVG from a wide range scan.
     *
     * @param caption Document caption.
     * @param range Wide range scan to visualize.
     * @param fname Source file name.
     * @param fnamedna Output SVG file path.
     * @param strsha256 Source file SHA256 hash.
     * @param filesize Source file size in bytes.
     * @return vok on success, or an error status on failure.
     */
    verr GenerateDocument(
        const std::string &caption,
        const VHWideRangeScan &range,
        const std::string &fname,
        const std::string &fnamedna,
        const std::string &strsha256,
        size_t filesize);

private:

    // DOC Size: Width
    static const uint16_t doc_size_w = 2800;

    // DOC Size: Height
    static const uint16_t doc_size_h = 2600;

    // Content area margin: Width
    static const uint16_t doc_content_margin_w = 240;

    // Content area margin: Height
    static const uint16_t doc_content_margin_h = 200;

    // === SVG GENERATION ===
    VHSVG svg;

    /**
     * @brief Calculates header, body, and footer layout rectangles.
     *
     * @param rect Content area rectangle.
     * @param recthdr Output header rectangle.
     * @param rectbody Output body rectangle.
     * @param rectftr Output footer rectangle.
     */
    void GenerateLayout(
        const VHSVGRect &rect,
        VHSVGRect &recthdr,
        VHSVGRect &rectbody,
        VHSVGRect &rectftr);

    /**
     * @brief Generates the document header.
     *
     * @param caption Document caption.
     * @param rect Header area rectangle.
     */
    void GenerateHeader(const std::string &caption, const VHSVGRect &rect);

    /**
     * @brief Generates the document footer with file metadata and timestamp.
     *
     * @param rect Footer area rectangle.
     * @param filename Source file name (path is stripped for display).
     * @param filesize Source file size in bytes.
     * @param strsha256 Source file SHA256 hash.
     * @return void
     */
    void GenerateFooter(
        const VHSVGRect &rect,
        const std::string &filename,
        size_t filesize,
        const std::string &strsha256);

    /**
     * @brief Generates 256-row byte fingerprint statistics visualization.
     *
     * @param svg SVG document to draw into.
     * @param rect Drawing area rectangle.
     * @param wscan Wide range scan data.
     */
    void Gen256Stat(
        VHSVG &svg,
        const VHSVGRect &rect,
        const VHWideRangeScan &wscan);

};
