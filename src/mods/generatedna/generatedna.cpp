#include "generatedna.hpp"

#include "widerange/widerange.hpp"

verr VHDocDNA::GenerateDocument(
    std::vector<uint8_t> arrsrc,
    const std::string &fnamedna)
{

    // WideRange Scan
    VHWideRangeScan wscan;

    if (vok != wscan.scan(arrsrc))
        return verrmsg(1, "Wide Range analisys failed");

    VHSVGRect rectabs(0, 0, 4000, 6000);

    // SVG Paper area (including border)
    VHSVGRect rect_paper_area(rectabs);
    rect_paper_area.shrink(200, 200);

    // === SVG GENERATION ===
    VHSVG svg;

    // SVG Generation
    svg.begin(rectabs.width(), rectabs.height());

    // Visible area
    svg.rect(rectabs, 1, "gray", "white");

    // Items
    Gen256(svg, wscan);

    // SVG Done
    svg.end();

    // Save results
    svg.savetosvg(fnamedna);

    return vok;
}

void VHDocDNA::Gen256(VHSVG &svg, const VHWideRangeScan &wscan)
{

    // Nothing to generate
    if(!wscan.width())
        return;


    const uint16_t cellw = 3800;
    const uint16_t cellh = 16;

    const uint16_t fonth = 14;
    const uint16_t cellh_step = 4;

    VHSVGRect itemrect(100, 100, cellw, cellh);

    for (uint16_t s = 0; s < 256; s++)
    {
        //
        const VHKeyFingerPrint & obj = wscan[s];

        std::string color_frnt = obj.empty() ? "#CCCCCC" : "gray";
        std::string color_back = obj.empty() ? "#F8F8F8" : "#EEEEEE";

        svg.rect(itemrect, 1, color_frnt, color_back);

        //

        for(uint16_t pos = obj.getStartPos(); pos < obj.getEndPos() + 1; pos++)
        {
            if(obj.status(pos))
            {
                size_t dx = pos * cellw / wscan.width();
                uint16_t linex = itemrect.startx() + dx;
                std::string color = (dx % 2) ? "#8888FF" : "#6666FF";
                svg.line(linex, itemrect.starty() + 1, linex, itemrect.endy() - 1, 1, color);
            }
        }

        // Show number
        int txtx = itemrect.startx() + 4;
        int txty = itemrect.starty() + cellh - (cellh - fonth + 1);
        svg.text(txtx, txty, std::to_string(s), "verdna", fonth, color_frnt);

        // Move to the next
        itemrect.move(0, cellh + cellh_step);
    }
}
