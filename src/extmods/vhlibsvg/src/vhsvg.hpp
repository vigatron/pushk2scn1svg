#pragma once

#include "vhplatform.hpp"

#include "vhsvgparam.hpp"

#include "vhstrtools.hpp"
#include "vhsvgrect.hpp"


class VHSVG
{

public:

    //
    void append(const std::string &content);

    //
    void append(const std::vector<std::string> &content);

    //
    void savetosvg(std::string fname);

    //
    std::string svg_tline(std::string pfx, std::string content, bool close = true);

    //
    void line(int x1, int y1, int x2, int y2, int wdt, std::string col, std::string dot = "", std::string opa = "");

    //
    void circ(int cx, int cy, int r, int wdt, std::string colf, std::string colb = "");

    //
    void arc(int x, int y, int rad, int wdt, std::string colf);

    //
    void path(int mx, int my, int ax, int ay, int fx, int fy, int ca, int wdt, std::string colf, std::string dot = "");

    //
    void rect(int x, int y, int width, int height, int wdt, std::string colf, std::string colb = "", int rr = 0);

    //
    void rect(const VHSVGRect &r, int th, std::string colf, std::string colb = "");

    //
    void text(int x, int y, std::string text, std::string fontfam, int fontsz, std::string col);

    //
    void begin(int svg_width, int svg_height);

    //
    void end();

    //
    static const std::string stdstr_svg;

private:
    std::vector<std::string> svg_content;
    const std::string spc = " ";
};
