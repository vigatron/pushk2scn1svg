#include "draw.hpp"
#include "vhstrtools.hpp"
#include "global.hpp"

#include "structs/blockprops.hpp"

uint32_t arrcolors[4] = {0xFFFFFF, 0x80E080, 0xFFFF00, 0xFF8080};

// ----------------------------------------------------------------------------
void rgb_to_compo(uint32_t v, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (v >> 16) & 0xFF;
    *g = (v >> 8) & 0xFF;
    *b = (v >> 0) & 0xFF;
}

// ----------------------------------------------------------------------------
uint8_t scaleval(uint8_t v1, uint8_t v2, uint8_t pos, uint8_t range)
{
    uint8_t r;
    int delta = v2 - v1;
    r = v1 + (delta * pos / range);
    return r;
}

// ----------------------------------------------------------------------------
std::string hexval(int v)
{
    char sss[3] = {
        szhex()[(v >> 4) & 0xF],
        szhex()[(v >> 0) & 0xF], 0};
    return std::string(sss);
}

// ----------------------------------------------------------------------------
std::string strrgb(uint8_t r, uint8_t g, uint8_t b)
{
    return "#" + hexval(r) + hexval(g) + hexval(b);
}

// ----------------------------------------------------------------------------
std::string strrgb(uint32_t v)
{
    uint8_t r = (v >> 16) & 0xFF;
    uint8_t g = (v >> 8) & 0xFF;
    uint8_t b = (v >> 0) & 0xFF;
    return strrgb(r, g, b);
}

// ----------------------------------------------------------------------------
std::string lincolor(uint8_t vv)
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

// ----------------------------------------------------------------------------
static void draw_column_mask(int idx, const VHSVGRect &rect, const std::vector<BlockProps> &arr)
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

// ----------------------------------------------------------------------------
void DrawDensity(const VHSVGRect &rect, const std::vector<BlockProps> &arr, int blksize, int ws)
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
        draw_column_mask(i, rectcol, arr);
    }

    DrawCenterLine(rectgfx);

    for (int i = 0; i < arr.size(); i++)
    {
        VHSVGRect rectcol(rectdif.startx() + unitw * i + 1, rectdif.starty(), unitw, rectdif.height());
        int color = i * 255 / arr.size();

        std::string clr = lincolor(color);
        svg.rect(rectcol, 1, clr, clr);
        // draw_col_bck( i , rectcol );
    }

    DrawScale(rectscl, blksize);
}
