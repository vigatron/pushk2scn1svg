#include "draw.hpp"
#include "global.hpp"

#include "draw/colors.hpp"

// ----------------------------------------------------------------------------
void DrawShadowBox(const VHSVGRect &rect)
{

    int dx = 16, dy = 24;

    VHSVGRect rect_shadow = rect;
    rect_shadow.move(dx, dy);
    svg.rect(rect_shadow, 1, Colors::clrshd_f, Colors::clrshd_b);
    svg.rect(rect, 1, Colors::clrshd_w, Colors::clrshd_bb);
}

// ----------------------------------------------------------------------------
void DrawScale(const VHSVGRect &rect, int size)
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
