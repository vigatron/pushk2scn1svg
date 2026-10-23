#include "draw.hpp"
#include "global.hpp"


/**
 *
 */
void DrawCenterLine(VHSVG &svg, const VHSVGRect &rect)
{

    int ymid1 = rect.starty() + rect.height() / 2;
    int ymidu = ymid1 - rect.height() / 4;
    int ymaxu = ymid1 + rect.height() / 4;

    svg.line(rect.startx(), ymid1, rect.endx(), ymid1, 1, "gray", "10,10", "0.40");
    svg.line(rect.startx(), ymidu, rect.endx(), ymidu, 1, "gray", "4,4", "0.25");
    svg.line(rect.startx(), ymaxu, rect.endx(), ymaxu, 1, "gray", "4,4", "0.25");
}

