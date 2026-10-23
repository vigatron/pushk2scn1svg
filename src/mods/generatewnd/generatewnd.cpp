#include "generatewnd.hpp"

#include "draw/draw.hpp"
#include "global.hpp"

#include "vhfsys.hpp"

/**
 *
 */
verr VHDocWnd::GenerateDocument(
    const std::vector<uint8_t> &arrsrc,
    const std::string &fnamewnd)
{

    // SVG Defines
    int SVG_W = 3200, SVG_H = 2400;
    int SVG_Border = 100;

    VHRect rect_full_area(0, 0, SVG_W, SVG_H);

    int hh_hdr = 200;
    int hh_ftr = 200;

    // Paper area including border
    VHRect rect_paper_area = rect_full_area;
    rect_paper_area.shrink(SVG_Border, SVG_Border);

    VHRect rect_2X4 = rect_paper_area;
    rect_2X4.shrink(0, (hh_hdr + hh_ftr) / 2);
    rect_2X4.setpos(rect_paper_area.sx, rect_paper_area.sy + hh_hdr);

    // Data types ( left )
    VHRect rect_types(rect_2X4.sx, rect_2X4.ey, rect_2X4.w / 2, hh_ftr);
    rect_types.shrink(64, 16);

    // Draw props ( right ) empty
    VHRect rect_props = rect_types;
    rect_props.move(rect_2X4.w / 2, 0);

    // SVG Generation
    svg.begin(SVG_W, SVG_H);
    svg.rect(0, 0, SVG_W, SVG_H, 0, "white", "white");
    // svg.rect( rect_paper_area, 1, "#AAAAAA" );

    Draw2X4Content(arrsrc, rect_2X4);

    DrawTypes(rect_types);
    svg.end();

    // Save results
    svg.savetosvg(fnamewnd);

    return vok;
}

/**
 *
 */
void VHDocWnd::Draw2X4Content(const std::vector<uint8_t> &arrsrc, const VHRect &rect)
{

    int ww = rect.w / 2;
    int hh = rect.h / 4;

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 2; i++)
        {

            int chunksize = 64 << j;
            const std::vector<BlockProps> &arrprops = GenSamplesArray(arrsrc, chunksize);

            VHRect rects(rect.sx + i * ww, rect.sy + j * hh, ww, hh);
            rects.shrink(64, 64);

            DrawShadowBox(rects);

            if (!i)
                DrawSection(rects, arrprops, arrsrc.size(), chunksize);
            else
                DrawDensity(rects, arrprops, arrsrc.size(), chunksize);
        }
    }
}

/**
 *
 */
std::vector<BlockProps> VHDocWnd::GenSamplesArray(const std::vector<uint8_t> &arrsrc, int subsize)
{
    std::vector<BlockProps> r;

    // Process block
    size_t blkcnt = arrsrc.size() / subsize;
    for (int i = 0; i < blkcnt; i++)
    {
        BlockProps blk;
        blk.scan(arrsrc.data() + i * subsize, subsize);
        r.push_back(blk);
    }

    return r;
}
