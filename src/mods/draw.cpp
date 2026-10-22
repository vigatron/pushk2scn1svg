#include "draw.hpp"
#include "global.hpp"


/**
 *
 */
void DrawCenterLine(const VHRect &rect)
{

    int ymid = rect.sy + rect.h / 2;
    int ymidu = ymid - rect.h / 4;
    int ymaxu = ymid + rect.h / 4;

    svg.line(rect.sx, ymid, rect.ex, ymid, 1, "gray", "10,10", "0.40");
    svg.line(rect.sx, ymidu, rect.ex, ymidu, 1, "gray", "4,4", "0.25");
    svg.line(rect.sx, ymaxu, rect.ex, ymaxu, 1, "gray", "4,4", "0.25");
}

