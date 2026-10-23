#include "vhsvgrect.hpp"

/** */
VHSVGRect::VHSVGRect() {}

/** */
VHSVGRect::VHSVGRect(const VHSVGRect &src)
{
    *this = src;
}

/** */
VHSVGRect::VHSVGRect(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    set(x, y, w, h);
}

/** */
VHSVGRect::~VHSVGRect() {}

/** */
VHSVGRect &VHSVGRect::operator=(const VHSVGRect &src)
{
    if (this != &src)
    {
        this->sx = src.sx;
        this->sy = src.sy;
        this->ex = src.ex;
        this->ey = src.ey;
        this->w = src.w;
        this->h = src.h;
    }
    return *this;
}

/** */
void VHSVGRect::set(int16_t x, int16_t y, int16_t w, int16_t h)
{
    sx = x;
    sy = y;
    this->w = w;
    this->h = h;
    ex = x + w;
    ey = y + h;
}

/** */
void VHSVGRect::setpos(int16_t x, int16_t y)
{
    this->sx = x;
    this->sy = y;
    this->ex = x + w;
    this->ey = y + h;
}

/** */
void VHSVGRect::setsiz(uint16_t w, uint16_t h)
{
    this->w = w;
    this->h = h;
    this->ex = sx + w;
    this->ey = sy + h;
}

/** */
void VHSVGRect::move(int16_t dx, int16_t dy)
{
    setpos(sx + dx, sy + dy);
}

/** */
void VHSVGRect::extdxdy(uint16_t dd)
{
    sx -= dd;
    sy -= dd;
    ex += dd;
    ey += dd;
    w += dd + dd;
    h += dd + dd;
}

/** */
void VHSVGRect::shrink(int dx, int dy)
{
    set(sx + dx, sy + dy, w - dx * 2, h - dy * 2);
}

/** */
const uint16_t VHSVGRect::width() const
{
    return w;
}

/** */
const uint16_t VHSVGRect::height() const
{
    return h;
}

/** */
const int16_t VHSVGRect::startx() const { 
    return sx; }

/** */
const int16_t VHSVGRect::endx() const { 
    return ex;
}

/** */
const int16_t VHSVGRect::starty() const { 
    return sy;
}

/** */
const int16_t VHSVGRect::endy() const {
    return ey;
}
