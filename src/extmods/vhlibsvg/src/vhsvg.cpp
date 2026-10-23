#include "vhsvg.hpp"

#include <iostream>
#include <fstream>

//
const std::string VHSVG::stdstr_svg = "http://www.w3.org/2000/svg";

/** */
void VHSVG::append(const std::string &content)
{
    svg_content.push_back(content);
}

/** */
void VHSVG::append(const std::vector<std::string> &content)
{
    svg_content.insert(svg_content.end(), content.begin(), content.end());
}

/** */
void VHSVG::savetosvg(std::string fname)
{

    // Create and open a text file for writing
    std::ofstream output_file(fname);

    // Write each string followed by a newline
    if (output_file.is_open())
    {
        for (const std::string &line : svg_content)
        {
            output_file << line << std::endl;
        }
        output_file.close(); // Close the file
        std::cout << "Data successfully saved to " << fname << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to open file for writing " << fname << std::endl;
    }
}

/** */
std::string VHSVG::svg_tline(std::string pfx, std::string content, bool close)
{
    return "<" + pfx + " " + content + (close ? "/" : "") + ">";
}

/** */
void VHSVG::line(
    int x1, int y1,
    int x2, int y2,
    int wdt, std::string col, std::string dot, std::string opa)
{
    std::string txt = "";
    txt += VHSVGParam("x1", x1);
    txt += VHSVGParam("y1", y1);
    txt += VHSVGParam("x2", x2);
    txt += VHSVGParam("y2", y2);
    if (wdt)
        txt += VHSVGParam("stroke-width", wdt);
    if (!col.empty())
        txt += VHSVGParam("stroke", col);
    if (!dot.empty())
    {
        txt += VHSVGParam("stroke-linecap", "round");
        txt += VHSVGParam("stroke-dasharray", dot);
    }
    if (!opa.empty())
    {
        txt += VHSVGParam("stroke-opacity", opa);
    }
    append(svg_tline("line", txt));
}

/** */
void VHSVG::circ(int cx, int cy, int r, int wdt, std::string colf, std::string colb)
{
    std::string txt = "";
    txt += VHSVGParam("cx", cx);
    txt += VHSVGParam("cy", cy);
    txt += VHSVGParam("r", r);
    if (wdt)
        txt += VHSVGParam("stroke-width", wdt);
    if (!colf.empty())
        txt += VHSVGParam("stroke", colf);
    if (!colb.empty())
        txt += VHSVGParam("fill", colb);
    append(svg_tline("circle", txt));
}

/** */
void VHSVG::arc(int x, int y, int rad, int wdt, std::string colf)
{
    std::string strsx = std::to_string(x - rad);
    std::string strsy = std::to_string(y);
    std::string strex = std::to_string(x + rad);
    std::string strey = std::to_string(y);

    std::vector<std::string> dparams = {
        "M", strsx, strsy,
        "A", std::to_string(rad), std::to_string(rad),
        "0", // x-axis-rotation: Rotation of the ellipse.
        "0", // large-arc-flag: 0 for the smaller arc, 1 for the larger arc.
        "0", // sweep-flag: 0 for clockwise, 1 for counter-clockwise.
        strex, strey};

    std::string par_d = VHSVGParam("d", join(dparams, " "));
    std::string par_fill = VHSVGParam("fill", "none");
    std::string par_strk = VHSVGParam("stroke", colf);
    std::string par_strkc = VHSVGParam("stroke-width", wdt);

    std::vector<std::string> pparams = {par_d, par_fill, par_strk, par_strkc};
    append(svg_tline("path", join(pparams, " ")));
}

/** */
void VHSVG::path(int mx, int my, int ax, int ay, int fx, int fy, int ca, int wdt, std::string colf, std::string dot)
{
    std::string strsx = std::to_string(mx);
    std::string strsy = std::to_string(my);

    std::string strax = std::to_string(ax);
    std::string stray = std::to_string(ay);
    std::string strfx = std::to_string(fx);
    std::string strfy = std::to_string(fy);

    std::string strca = std::to_string(ca);
    std::vector<std::string> dparams = {"M", strsx, strsy, "A", strax, stray, "0", "0", strca, strfx, strfy};

    std::string par_d = VHSVGParam("d", join(dparams, " "));
    std::string par_fill = VHSVGParam("fill", "none");
    std::string par_strk = VHSVGParam("stroke", colf);
    std::string par_strkc = VHSVGParam("stroke-width", wdt);

    std::vector<std::string> pparams = {par_d, par_fill, par_strk, par_strkc};
    if (dot.size())
    {
        std::string par_strdot = VHSVGParam("stroke-dasharray", dot);
        pparams.push_back(par_strdot);
    }
    append(svg_tline("path", join(pparams, " ")));
}

/** */
void VHSVG::rect(int x, int y, int width, int height, int wdt, std::string colf, std::string colb, int rr)
{
    std::string txt = "";
    txt += VHSVGParam("x", x);
    txt += VHSVGParam("y", y);
    txt += VHSVGParam("width", width);
    txt += VHSVGParam("height", height);
    if (rr)
    {
        txt += VHSVGParam("rx", rr);
        txt += VHSVGParam("ry", rr);
    }
    if (wdt)
        txt += VHSVGParam("stroke-width", wdt);
    if (!colf.empty())
        txt += VHSVGParam("stroke", colf);
    txt += VHSVGParam("fill", colb.empty() ? "none" : colb);
    append(svg_tline("rect", txt));
}

/** */
void VHSVG::rect(const VHSVGRect &r, int th, std::string colf, std::string colb)
{
    rect(r.startx(), r.starty(), r.width(), r.height(), 1, colf, colb);
}

/** */
void VHSVG::text(int x, int y, std::string text, std::string fontfam, int fontsz, std::string col)
{
    std::string txt = "";
    txt += VHSVGParam("x", x);
    txt += VHSVGParam("y", y);
    txt += VHSVGParam("font-family", fontfam);
    txt += VHSVGParam("font-size", std::to_string(fontsz) + "px");
    // if(wdt) txt += svg_param("stroke-width", wdt);
    // if(!colf.empty()) txt += svg_param("stroke", colf);
    if (!col.empty())
        txt += VHSVGParam("fill", col);
    append(svg_tline("text", txt, false) + text + "</text>");
}

/** */
void VHSVG::begin(int svg_width, int svg_height)
{
    std::string txt = "";
    txt += VHSVGParam("width", svg_width);
    txt += VHSVGParam("height", svg_height);
    txt += VHSVGParam("xmlns", stdstr_svg);
    append(svg_tline("svg", txt, false));
}

/** */
void VHSVG::end()
{
    append("</svg>");
}
