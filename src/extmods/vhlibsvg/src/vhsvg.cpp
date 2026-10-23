#include "vhsvg.hpp"

//
const std::string VHSVG::stdstr_svg = "http://www.w3.org/2000/svg";


void VHSVG::append(const std::string &content)
{
    svg_content.push_back(content);
}

//
void VHSVG::append(const std::vector<std::string> &content)
{
    svg_content.insert(svg_content.end(), content.begin(), content.end());
}

