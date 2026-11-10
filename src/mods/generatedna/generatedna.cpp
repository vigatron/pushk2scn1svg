#include "generatedna.hpp"

#include "colors/colors.hpp"
#include "svgtools/svgtools.hpp"
#include "widerange/widerange.hpp"

#include "vhfsys.hpp"

#include <ctime>

// Abs Map / Rel Map
// Colors are defined in the Colors class (src/mods/colors/colors.hpp)

//
void VHDocDNA::GenerateHeader(const std::string &caption, const VHSVGRect &rect)
{
    svg.text(
        rect.startx() + 64,
        rect.starty() + 72,
        VHSvgTools::EscapeXml(caption),
        "Arial",
        48,
        Colors::clrheadertxt);

    svg.rect(rect, 1, Colors::clrline);
}

/**
 * @brief Generates the document footer with file metadata and timestamp.
 *
 * @param rect Footer area rectangle.
 * @param filename Source file name (path is stripped for display).
 * @param filesize Source file size in bytes.
 * @param strsha256 Source file SHA256 hash.
 * @return void
 */
void VHDocDNA::GenerateFooter(
    const VHSVGRect &rect,
    const std::string &filename,
    size_t filesize,
    const std::string &strsha256)
{
    // File Name (display only the file name without path)
    const std::string strfname = VHSvgTools::EscapeXml(VHFSYS::ExtractFilename(filename));

    //
    int startx = rect.startx();
    int starty = rect.starty();
    int width = rect.width();
    int height = rect.height();
    int stepdy = 20;

    //
    std::string colorhdr = Colors::clrheadertxt;
    std::string fonthdr = "Arial";
    int fontsize = 16;

    int startxx = startx + 160;

    svg.rect(startx, starty, width, height, 1, colorhdr, Colors::clrbck1);

    startx += 40;
    starty += 32;

    // File Name
    svg.text(startx, starty, "File Name", fonthdr, fontsize, colorhdr);
    svg.text(startxx, starty, strfname, fonthdr, fontsize, colorhdr);

    starty += stepdy;

    // File Size
    svg.text(startx, starty, "File Size", fonthdr, fontsize, colorhdr);
    svg.text(startxx, starty, std::to_string(filesize), fonthdr, fontsize, colorhdr);

    starty += stepdy;

    // File SHA256
    svg.text(startx, starty, "SHA256", fonthdr, fontsize, colorhdr);
    svg.text(startxx, starty, strsha256, fonthdr, fontsize, colorhdr);

    starty += stepdy;

    // * Document creation Date Time *
    // Date Time Format: DD-MMM-YYYY HH:MM (month as text)
    std::time_t now = std::time(nullptr);
    std::tm local_tm{};
#if defined(_WIN32)
    localtime_s(&local_tm, &now);
#else
    localtime_r(&now, &local_tm);
#endif
    char datetime_buf[32] = {};
    std::strftime(datetime_buf, sizeof(datetime_buf), "%d-%b-%Y %H:%M", &local_tm);

    svg.text(startx, starty, "DateTime", fonthdr, fontsize, colorhdr);
    svg.text(startxx, starty, datetime_buf, fonthdr, fontsize, colorhdr);
}

//
verr VHDocDNA::GenerateDocument(
    const std::string &caption,
    const VHWideRangeScan &range,
    const std::string &fname,
    const std::string &fnamedna,
    const std::string &strsha256,
    size_t filesize)
{
    // SVG Area
    VHSVGRect rectabs(0, 0, doc_size_w, doc_size_h);

    // SVG Paper area (including border)
    VHSVGRect rect_paper_area(rectabs);
    rect_paper_area.shrink(doc_content_margin_w, doc_content_margin_h);

    // SVG Generation
    svg.begin(rectabs.width(), rectabs.height());

    // Visible area
    svg.rect(rectabs, 1, Colors::clrline, Colors::clrpaper);

    svg.rect(rect_paper_area, 1, Colors::clrline);

    //
    VHSVGRect recthdr;
    VHSVGRect rectbody;
    VHSVGRect rectftr;

    GenerateLayout(rect_paper_area, recthdr, rectbody, rectftr);

    //
    GenerateHeader(caption, recthdr);
    GenerateFooter(rectftr, fname, filesize, strsha256);

    // Items
    Gen256Stat(svg, rectbody, range);

    // SVG Done
    svg.end();

    // Save results
    return VHSvgTools::Save(svg, fnamedna);
}

//
void VHDocDNA::Gen256Stat(
    VHSVG &svg,
    const VHSVGRect &rect,
    const VHWideRangeScan &wscan)
{

    // Nothing to generate
    if (!wscan.width())
        return;

    // Absolute scans store absolute positions, relative scans store offsets
    // from the first occurrence; every query below must use the same space.
    const bool relmode = wscan.relative();

    VHSVGRect innerrect(rect);
    innerrect.shrink(64, 64);

    const uint16_t cellw = innerrect.width();
    const uint16_t cellh = 6;
    const uint16_t fonth = cellh - 2;

    const uint16_t cellh_step = 1;

    VHSVGRect itemrect(innerrect.startx(), innerrect.starty(), cellw, cellh);

    for (uint16_t s = 0; s < 256; s++)
    {
        VHSVGRect itemrect_back(itemrect);
        itemrect_back.shrink(0, 1);

        //
        const VHKeyFingerPrint &obj = wscan[s];
        const bool empty = obj.empty();

        std::string color_text = empty ? Colors::clrempty : Colors::clrline;
        std::string color_bord = empty ? "none" : Colors::clrempty;
        std::string color_back = Colors::clrempty;

        svg.rect(itemrect_back, 1, color_bord, color_back);

        //
        if (!empty)
        {
            const size_t first = relmode ? 0u : static_cast<size_t>(obj.getStartPos());
            const size_t last = relmode ? static_cast<size_t>(obj.getSpan())
                                        : static_cast<size_t>(obj.getEndPos());

            for (size_t idx = first; idx <= last; idx++)
            {
                if (!obj.status(static_cast<uint16_t>(idx)))
                    continue;

                const size_t dx = idx * cellw / wscan.width();
                const int linex = itemrect.startx() + static_cast<int>(dx);
                const std::string color = (dx % 2) ? Colors::clrcellalt1 : Colors::clrcellalt2;
                svg.line(linex, itemrect.starty() + 1, linex, itemrect.endy() - 1, 1, color);
            }
        }

        // Show number
        std::string strnum = "#" + std::to_string(s);
        int txtx = itemrect.startx() - cellh * 3;
        int txty = itemrect.starty() + cellh - (cellh - fonth + 1);
        svg.text(txtx, txty, strnum, "verdana", fonth, color_text);

        // Move to the next
        itemrect.move(0, cellh + cellh_step);
    }
}

/**
 * Calculate Layout
 */
void VHDocDNA::GenerateLayout(
    const VHSVGRect &rect,
    VHSVGRect &recthdr,
    VHSVGRect &rectbody,
    VHSVGRect &rectftr)
{

    recthdr = rect;
    recthdr.setsiz(rect.width(), 120);

    rectbody = rect;
    rectbody.setsiz(rect.width(), rect.height() - 2 * 120);
    rectbody.move(0, 120);

    rectftr = rect;
    rectftr.setsiz(rect.width(), 120);
    rectftr.move(0, rect.height() - 120);
}
