#include "vhsvgdraw.hpp"

#include "colors/colors.hpp"
#include "global.hpp"
#include "vhstrtools.hpp"

#include <string>
#include <vector>

namespace
{
uint32_t arrcolors[4] = {0xFFFFFF, 0x80E080, 0xFFFF00, 0xFF8080};
} // namespace

void VHSVGDraw::DrawCenterLine(VHSVG &svg, const VHSVGRect &rect)
{

    int ymid1 = rect.starty() + rect.height() / 2;
    int ymidu = ymid1 - rect.height() / 4;
    int ymaxu = ymid1 + rect.height() / 4;

    svg.line(rect.startx(), ymid1, rect.endx(), ymid1, 1, "gray", "10,10", "0.40");
    svg.line(rect.startx(), ymidu, rect.endx(), ymidu, 1, "gray", "4,4", "0.25");
    svg.line(rect.startx(), ymaxu, rect.endx(), ymaxu, 1, "gray", "4,4", "0.25");
}

void VHSVGDraw::DrawShadowBox(VHSVG &svg, const VHSVGRect &rect)
{

    int dx = 16, dy = 24;

    VHSVGRect rect_shadow = rect;
    rect_shadow.move(dx, dy);
    svg.rect(rect_shadow, 1, Colors::clrshd_f, Colors::clrshd_b);
    svg.rect(rect, 1, Colors::clrshd_w, Colors::clrshd_bb);
}

void VHSVGDraw::DrawScale(VHSVG &svg, const VHSVGRect &rect, int size)
{

    int n = 0;
    int s = size;
    int fnsize = 18;

    while (s > 256)
    {

        int dx = s * rect.width() / size;
        int dxt = dx + ((s == size) ? -64 : 6);
        int dy = rect.height();
        VHSVGRect r(rect.startx(), rect.starty(), dx, rect.height() + 1);

        std::string colf = "#AAAAAA";
        std::string colb = (n++ & 1) ? "#D0D0D0" : "#E0E0E0";
        std::string val = std::to_string(s);

        svg.rect(r, 1, colf, colb);
        svg.text(r.startx() + dxt, r.endy() - fnsize / 2, val, "verdana", fnsize, Colors::colortxt);
        s /= 2;
    }

    svg.text(rect.startx() + 6, rect.endy() - fnsize / 2, "0", "verdana", fnsize, Colors::colortxt);
}

// ----------------------------------------------------------------------------
// Section
// ----------------------------------------------------------------------------
void VHSVGDraw::draw_column_spread(
    VHSVG &svg,
    int idx,
    const VHSVGRect &rect,
    const std::vector<BlockProps> &arr)
{
    int ww = rect.width();
    int hh = rect.height();
    int sx = rect.startx();
    int ey = rect.endy();

    const BlockProps &s = arr[idx];

    int gfx_hmin = s._min * hh / 255;
    int gfx_hmax = s._max * hh / 255;
    int gfx_hdiff = gfx_hmax - gfx_hmin;
    svg.rect(sx, ey - gfx_hmax, ww, gfx_hdiff, 0, "", Colors::clrspr);

    int gfx_hwdt = s.wght * hh / 128; // 255;
    int gfx_hmid = gfx_hdiff / 2;
    int gfx_ymid = ey - gfx_hmin - gfx_hmid - gfx_hwdt / 2;
    int gfx_yund = ey + 128 - gfx_hwdt / 2;
    svg.rect(sx, gfx_ymid, ww, gfx_hwdt, 0, "", Colors::clrwdt);
}

// ----------------------------------------------------------------------------
// Draw background bar
// ----------------------------------------------------------------------------
void VHSVGDraw::draw_col_bck(VHSVG &svg, int idx, const VHSVGRect &rect)
{
    std::string color = (idx & 1) ? Colors::clrbck1 : Colors::clrbck2;
    svg.rect(rect, 0, color, color);
}

void VHSVGDraw::DrawSection(
    VHSVG &svg,
    const VHSVGRect &rect,
    const std::vector<BlockProps> &arr,
    int blksize,
    int ws)
{

    int ww = rect.width();
    int h1 = rect.height() * 0.8;
    int h2 = rect.height() * 0.1;
    int h3 = rect.height() * 0.1;
    int y1 = rect.starty();
    int y2 = y1 + h1;
    int y3 = y2 + h2;

    VHSVGRect rectgfx(rect.startx(), y1, ww, h1); // Quantization
    VHSVGRect rectdif(rect.startx(), y2, ww, h2); // Delta
    VHSVGRect rectscl(rect.startx(), y3, ww, h3); // Scale

    float unitw = rect.width() * 1.0 / arr.size();

    for (int i = 0; i < arr.size(); i++)
    {
        VHSVGRect rectcol(rect.startx() + unitw * i, rect.starty(), unitw, rectgfx.height());
        draw_col_bck(svg, i, rectcol);
    }

    for (int i = 0; i < arr.size(); i++)
    {
        VHSVGRect rectcol(rect.startx() + unitw * i, rect.starty(), unitw, rectgfx.height());
        draw_column_spread(svg, i, rectcol, arr);
    }

    DrawCenterLine(svg, rectgfx);

    svg.rect(rectgfx, 1, "#AAAAAA");
    svg.rect(rectdif, 1, "#AAAAAA");
    svg.rect(rectscl, 1, "#AAAAAA");
    svg.rect(rect, 1, "#CCCCCC");

    std::string txt = "Размер окна: " + std::to_string(ws) + " байт";
    svg.text(rect.startx() + 14, rect.starty() - 20, txt, "verdana", 32, Colors::colortxtsh2);
    svg.text(rect.startx() + 12, rect.starty() - 22, txt, "verdana", 32, Colors::colortxt);

    DrawScale(svg, rectscl, blksize);
}

// ----------------------------------------------------------------------------
// Density
// ----------------------------------------------------------------------------
void VHSVGDraw::rgb_to_compo(uint32_t v, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (v >> 16) & 0xFF;
    *g = (v >> 8) & 0xFF;
    *b = (v >> 0) & 0xFF;
}

uint8_t VHSVGDraw::scaleval(uint8_t v1, uint8_t v2, uint8_t pos, uint8_t range)
{
    uint8_t r;
    int delta = v2 - v1;
    r = v1 + (delta * pos / range);
    return r;
}

std::string VHSVGDraw::hexval(int v)
{
    char sss[3] = {
        VHSVGTextTools::szhex()[(v >> 4) & 0xF],
        VHSVGTextTools::szhex()[(v >> 0) & 0xF], 0};
    return std::string(sss);
}

std::string VHSVGDraw::strrgb(uint8_t r, uint8_t g, uint8_t b)
{
    return "#" + hexval(r) + hexval(g) + hexval(b);
}

std::string VHSVGDraw::strrgb(uint32_t v)
{
    uint8_t r = (v >> 16) & 0xFF;
    uint8_t g = (v >> 8) & 0xFF;
    uint8_t b = (v >> 0) & 0xFF;
    return strrgb(r, g, b);
}

std::string VHSVGDraw::lincolor(uint8_t vv)
{
    uint8_t val = 64 + vv * 2 / 3;
    uint8_t pos = val / 86; // /3
    uint8_t dv = val % 86;
    uint8_t rf, gf, bf;
    uint8_t rt, gt, bt;
    uint32_t rgbf = arrcolors[pos];
    uint32_t rgbt = arrcolors[pos + 1];

    rgb_to_compo(rgbf, &rf, &gf, &bf);
    rgb_to_compo(rgbt, &rt, &gt, &bt);

    uint8_t r = scaleval(rf, rt, dv, 86);
    uint8_t g = scaleval(gf, gt, dv, 86);
    uint8_t b = scaleval(bf, bt, dv, 86);

    return strrgb(r, g, b);
}

void VHSVGDraw::draw_column_mask(
    VHSVG &svg,
    int idx,
    const VHSVGRect &rect,
    const std::vector<BlockProps> &arr)
{

    int ww = rect.width();
    int hh = rect.height();
    int sx = rect.startx();
    int ey = rect.endy();

    const BlockProps &blk = arr[idx];

    int gfx_hmin = blk._min * hh / 255;
    int gfx_hmax = blk._max * hh / 255;
    int gfx_hdiff = gfx_hmax - gfx_hmin;

    int range = blk._max - blk._min;
    std::string barcolor = lincolor(range);
    svg.rect(sx, ey - gfx_hmax, ww, gfx_hdiff, 0, "", barcolor);

    for (int i = 0; i < 256; i++)
    { // blk.wght
        if (blk.spc[i])
        {
            int val = blk.spc[i];
            int posy = rect.endy() - i * hh / 255;
            svg.line(rect.startx(), posy, rect.endx(), posy, 1, "red");
        }
    }

    // int gfx_hmin    = s._min * hh / 255;
    // int gfx_hmax    = s._max * hh / 255;
    // int gfx_hdiff   = gfx_hmax - gfx_hmin;
    // svg.rect(sx, ey - gfx_hmax, ww, gfx_hdiff, 0, "", Colors::clrspr);

    // int gfx_hwdt     = s.wght * hh / 128; // 255;
    // int gfx_hmid    = gfx_hdiff / 2;
    // int gfx_ymid    = ey - gfx_hmin - gfx_hmid - gfx_hwdt/2;
    // int gfx_yund    = ey + 128 - gfx_hwdt/2;
    // svg.rect(sx, gfx_ymid, ww, gfx_hwdt, 0, "", Colors::clrwdt );
}

void VHSVGDraw::DrawDensity(
    VHSVG &svg,
    const VHSVGRect &rect,
    const std::vector<BlockProps> &arr,
    int blksize,
    int ws)
{

    int ww = rect.width();

    int h1 = rect.height() * 0.8;
    int h2 = rect.height() * 0.1;
    int h3 = rect.height() * 0.1;

    int y1 = rect.starty();
    int y2 = y1 + h1;
    int y3 = y2 + h2;

    VHSVGRect rectgfx(rect.startx(), y1, ww, h1); // Quantization
    VHSVGRect rectdif(rect.startx(), y2, ww, h2); // Delta
    VHSVGRect rectscl(rect.startx(), y3, ww, h3); // Scale

    float unitw = rectgfx.width() * 1.0f / arr.size();

    for (int i = 0; i < arr.size(); i++)
    {
        VHSVGRect rectcol(rect.startx() + unitw * i, rect.starty(), unitw, rectgfx.height());
        draw_column_mask(svg, i, rectcol, arr);
    }

    DrawCenterLine(svg, rectgfx);

    for (int i = 0; i < arr.size(); i++)
    {
        VHSVGRect rectcol(rectdif.startx() + unitw * i + 1, rectdif.starty(), unitw, rectdif.height());
        int color = i * 255 / arr.size();

        std::string clr = lincolor(color);
        svg.rect(rectcol, 1, clr, clr);
        // draw_col_bck( i , rectcol );
    }

    DrawScale(svg, rectscl, blksize);
}

// ----------------------------------------------------------------------------
// Types
// ----------------------------------------------------------------------------
const std::vector<std::string> &VHSVGDraw::types()
{
    static const std::vector<std::string> arrtypes = {
        "sint8_t",
        "uint8_t",
        "sint8_t x2",
        "uint8_t x2",

        "sint16_t (BE)",
        "sint16_t (LE)",
        "uint16_t (BE)",
        "uint16_t (LE)",

        "sint16_t (BE) x2",
        "sint16_t (LE) x2",
        "uint16_t (BE) x2",
        "uint16_t (LE) x2",

        "sint32_t (BE)",
        "sint32_t (LE)",
        "uint32_t (BE)",
        "uint32_t (LE)",

        "float16",
        "float16   x2",
        "double32",
        "double64",

        "1:5:5:5",
        "5:5:6",
        "5:6:5",
        "6:5:5",

        "4:4:4:4",
        "4:4:8",
        "8:8:8",
        "8:8:8:8",

    };
    return arrtypes;
}

std::string VHSVGDraw::gettype(int i, int j)
{
    int n = i * 4 + j;
    return types()[n];
}

void VHSVGDraw::DrawTypes(VHSVG &svg, const VHSVGRect &rect)
{

    int fnsize = 18;
    std::string fnfam = "noto";

    int cols = 7;
    int rows = 4;
    int ww = rect.width() / cols;
    int hh = rect.height() / rows;

    DrawShadowBox(svg, rect);

    svg.rect(rect, 1, "gray");

    for (int j = 0; j < rows; j++)
    {

        VHSVGRect rect_bck(rect.startx(), rect.starty() + j * hh, rect.width(), hh);
        std::string col = !(j & 1) ? Colors::clrbck1 : Colors::clrbck2;
        svg.rect(rect_bck, 0, col, col);

        for (int i = 0; i < cols; i++)
        {

            int x = rect.startx() + ww * i;
            int y = rect.starty() + hh * j;
            int yt = y + hh / 2 + fnsize / 4;
            bool sel = (!i) && (j == 1);

            std::string strt = gettype(i, j);
            std::string colr = sel ? "green" : Colors::colortxt;

            if (sel)
            {
                svg.rect(x, y, ww, hh, 1, "none", "#D0FFD0");
                svg.text(x + 10, yt, "[ X ]", fnfam, fnsize, colr);
            }
            else
            {
                svg.text(x + 10, yt, "[ _ ]", fnfam, fnsize, colr);
            }

            if (strt.size())
            {
                svg.text(x + 64, yt, strt, fnfam, fnsize, colr);
            }
        }
    }
}
