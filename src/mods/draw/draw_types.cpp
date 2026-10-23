#include "draw.hpp"
#include "global.hpp"

#include "draw/colors.hpp"

std::vector<std::string> arrtypes = {
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

std::string gettype(int i, int j)
{
    int n = i * 4 + j;
    return arrtypes[n];
}

void DrawTypes(const VHSVGRect &rect)
{

    int fnsize = 18;
    std::string fnfam = "noto";

    int cols = 7;
    int rows = 4;
    int ww = rect.width() / cols;
    int hh = rect.height() / rows;

    DrawShadowBox(rect);

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