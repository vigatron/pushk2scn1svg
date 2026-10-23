#include "draw.hpp"

#include "colors.hpp"
#include "global.hpp"

// ----------------------------------------------------------------------------
static void draw_column_spread(int idx, const VHSVGRect &rect, const std::vector<BlockProps> &arr)
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
void draw_col_bck(int idx, const VHSVGRect &rect)
{
    std::string color = (idx & 1) ? Colors::clrbck1 : Colors::clrbck2;
    svg.rect(rect, 0, color, color);
}

// ----------------------------------------------------------------------------
void DrawSection(const VHSVGRect &rect, const std::vector<BlockProps> &arr, int blksize, int ws)
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
        draw_col_bck(i, rectcol);
    }

    for (int i = 0; i < arr.size(); i++)
    {
        VHSVGRect rectcol(rect.startx() + unitw * i, rect.starty(), unitw, rectgfx.height());
        draw_column_spread(i, rectcol, arr);
    }

    DrawCenterLine(rectgfx);

    svg.rect(rectgfx, 1, "#AAAAAA");
    svg.rect(rectdif, 1, "#AAAAAA");
    svg.rect(rectscl, 1, "#AAAAAA");
    svg.rect(rect, 1, "#CCCCCC");

    std::string txt = "Размер окна: " + std::to_string(ws) + " байт";
    svg.text(rect.startx() + 14, rect.starty() - 20, txt, "verdana", 32, Colors::colortxtsh2);
    svg.text(rect.startx() + 12, rect.starty() - 22, txt, "verdana", 32, Colors::colortxt);

    DrawScale(rectscl, blksize);
}
