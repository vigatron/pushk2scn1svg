#include "generatewnd.hpp"

#include "colors/colors.hpp"
#include "global.hpp"
#include "svgtools/svgtools.hpp"

/**
 * @brief Builds the document layout, renders content, and saves the SVG file.
 *
 * @param arrsrc Source data buffer to analyze and visualize.
 * @param fnamewnd Output SVG file path.
 * @param strsha256 Source file SHA256 hash.
 * @param filesize Source file size in bytes.
 * @return vok on success, or an error status on failure.
 */
verr VHDocWnd::GenerateDocument(
    const std::vector<uint8_t> &arrsrc,
    const std::string &fnamewnd,
    const std::string &strsha256,
    size_t filesize) const
{
    // Full document area
    VHSVGRect rect_full_area(0, 0, kPaperWidth, kPaperHeight);

    // Paper area (excluding border)
    VHSVGRect rect_paper_area = rect_full_area;
    rect_paper_area.shrink(kPaperBorderX, kPaperBorderY);

    // 2x4 content area between header and footer
    VHSVGRect rect_2X4 = rect_paper_area;
    rect_2X4.shrink(0, (kHeaderHeight + kFooterHeight) / 2);
    rect_2X4.setpos(rect_paper_area.startx(), rect_paper_area.starty() + kHeaderHeight);

    // Data types legend (left, footer area)
    VHSVGRect rect_types(rect_2X4.startx(), rect_2X4.endy(), rect_2X4.width() / kGridCols, kFooterHeight);
    rect_types.shrink(kTypesPaddingX, kTypesPaddingY);

    // Draw props (right) - reserved
    VHSVGRect rect_props = rect_types;
    rect_props.move(rect_2X4.width() / kGridCols, 0);
    (void)rect_props;

    // === SVG GENERATION ===
    VHSVG svg;
    svg.begin(rect_full_area.width(), rect_full_area.height());

    svg.rect(0, 0, rect_full_area.width(), rect_full_area.height(), 0, Colors::clrline, Colors::clrpaper);

    Draw2X4Content(svg, arrsrc, rect_2X4);
    VHSVGDraw::DrawTypes(svg, rect_types);

    svg.end();

    return VHSvgTools::Save(svg, fnamewnd);
}

/**
 * @brief Draws the 2x4 content grid (Section/Density per row).
 *
 * @param svg Target SVG document.
 * @param arrsrc Source data buffer.
 * @param rect Bounding rectangle for the grid area.
 */
void VHDocWnd::Draw2X4Content(VHSVG &svg, const std::vector<uint8_t> &arrsrc, const VHSVGRect &rect) const
{
    const int ww = rect.width() / kGridCols;
    const int hh = rect.height() / kGridRows;

    for (int j = 0; j < kGridRows; ++j)
    {
        const size_t chunksize = kBaseChunkSize << static_cast<size_t>(j);
        const std::vector<BlockProps> arrprops = GenSamplesArray(arrsrc, chunksize);

        for (int i = 0; i < kGridCols; ++i)
        {
            VHSVGRect rects(rect.startx() + i * ww, rect.starty() + j * hh, ww, hh);
            rects.shrink(kCellPadding, kCellPadding);

            VHSVGDraw::DrawShadowBox(svg, rects);

            if (i == 0)
                VHSVGDraw::DrawSection(svg, rects, arrprops, arrsrc.size(), static_cast<int>(chunksize));
            else
                VHSVGDraw::DrawDensity(svg, rects, arrprops, arrsrc.size(), static_cast<int>(chunksize));
        }
    }
}

/**
 * @brief Splits source data into fixed-size blocks and scans properties for each.
 *
 * @param arrsrc Source data buffer.
 * @param subsize Block size in bytes; must be greater than 0.
 * @return Vector of scanned block properties (may be empty if subsize is 0 or data is smaller than one block).
 */
std::vector<BlockProps> VHDocWnd::GenSamplesArray(const std::vector<uint8_t> &arrsrc, size_t subsize)
{
    std::vector<BlockProps> r;
    if (subsize == 0)
        return r;

    const size_t blkcnt = arrsrc.size() / subsize;
    r.reserve(blkcnt);

    for (size_t i = 0; i < blkcnt; ++i)
    {
        BlockProps blk;
        blk.scan(arrsrc.data() + i * subsize, static_cast<int>(subsize));
        r.push_back(std::move(blk));
    }

    return r;
}
