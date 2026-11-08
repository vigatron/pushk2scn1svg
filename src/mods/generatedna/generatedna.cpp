#include "generatedna.hpp"

#include "widerange/widerange.hpp"

// TODO: Header / fname / size / MD5
// TODO: Colors set

// Abs Map / Rel Map
// Creation Date & Time

void VHDocDNA::GenerateHeader(
    const VHSVGRect &rect,
    const std::string &filename,
    size_t filesize,
    const std::string &strsha256)
{

    //
    int startx = rect.startx();
    int starty = rect.starty();
    int width = rect.width();
    int height = rect.height();

    //
    std::string colorhdr = "black";
    std::string fonthdr = "Arial";
    int fontsize = 16;

    int startxx = startx + 160;

    svg.rect(startx, starty, width, height, 1, colorhdr, "#F0F0F0");

    starty += 40;
    startx += 40;

    // File Name
    svg.text(startx, starty, "File Name", fonthdr, fontsize, colorhdr);
    svg.text(startxx, starty, filename, fonthdr, fontsize, colorhdr);

    starty += 20;

    // File Size
    svg.text(startx, starty, "File Size", fonthdr, fontsize, colorhdr);
    svg.text(startxx, starty, std::to_string(filesize), fonthdr, fontsize, colorhdr);

    starty += 20;

    // File SHA256
    svg.text(startx, starty, "SHA256", fonthdr, fontsize, colorhdr);
    svg.text(startxx, starty, strsha256, fonthdr, fontsize, colorhdr);
}

//
verr VHDocDNA::GenerateDocument(
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
    rect_paper_area.shrink(doc_content_dist_w, doc_content_dist_h);

    // SVG Generation
    svg.begin(rectabs.width(), rectabs.height());

    // Visible area
    svg.rect(rectabs, 1, "gray", "white");

    svg.rect(rect_paper_area, 1, "red");


    //
    VHSVGRect recthdr;
    VHSVGRect rectbody;
    VHSVGRect rectftr;

    GenerateLayout(rect_paper_area, recthdr, rectbody, rectftr);

    svg.rect(recthdr,  1, "orange");
    svg.rect(rectbody, 1, "blue");
    svg.rect(rectftr,  1, "green");

    //
    GenerateHeader(recthdr, fname, filesize, strsha256);

    // Items
    Gen256(svg, range);

    // SVG Done
    svg.end();

    // Save results
    svg.savetosvg(fnamedna);

    return vok;
}

//
void VHDocDNA::Gen256(VHSVG &svg, const VHWideRangeScan &wscan)
{

    // Nothing to generate
    if (!wscan.width())
        return;

    const uint16_t cellw = 1600;
    const uint16_t cellh = 6;
    const uint16_t fonth = cellh - 2;

    const uint16_t cellh_step = 1;

    VHSVGRect itemrect(100, 400, cellw, cellh);

    for (uint16_t s = 0; s < 256; s++)
    {
        VHSVGRect itemrect_back(itemrect);
        itemrect_back.shrink(0, 1);

        //
        const VHKeyFingerPrint &obj = wscan[s];
        bool empty = obj.empty();

        std::string color_text = empty ? "#EEEEEE" : "gray";
        std::string color_bord = empty ? "none" : "#EEEEEE";
        std::string color_back = empty ? "none" : "#EEEEEE";

        svg.rect(itemrect_back, 1, color_bord, color_back);

        //
        for (uint16_t pos = obj.getStartPos(); pos < obj.getEndPos() + 1; pos++)
        {
            if (obj.status(pos))
            {
                size_t dx = pos * cellw / wscan.width();
                uint16_t linex = itemrect.startx() + dx;
                std::string color = (dx % 2) ? "#8888FF" : "#6666FF";
                svg.line(linex, itemrect.starty() + 1, linex, itemrect.endy() - 1, 1, color);
            }
        }

        // Show number
        std::string strnum = "#" + std::to_string(s);
        int txtx = itemrect.startx() - cellh * 3;
        int txty = itemrect.starty() + cellh - (cellh - fonth + 1);
        svg.text(txtx, txty, strnum, "verdna", fonth, color_text);

        // Move to the next
        itemrect.move(0, cellh + cellh_step);
    }
}

/**
 * Calculate Layout
 */
void VHDocDNA::GenerateLayout(
    const VHSVGRect &rect,
    VHSVGRect & recthdr,
    VHSVGRect & rectbody,
    VHSVGRect & rectftr)
{

    recthdr = rect;
    recthdr.setsiz(rect.width(), 120);

    rectbody = rect;
    rectbody.setsiz(rect.width(), rect.height() - 2*120);
    rectbody.move(0, 120);

    rectftr = rect;
    rectftr.setsiz(rect.width(), 120);
    rectftr.move(0, rect.height() - 120);

}
