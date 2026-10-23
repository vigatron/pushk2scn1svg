#pragma once

#include <stdint.h>

class VHSVGRect
{

public:

    VHSVGRect();
    VHSVGRect(const VHSVGRect &src);
    VHSVGRect(int16_t x, int16_t y, uint16_t w, uint16_t h);
    ~VHSVGRect();
    VHSVGRect &operator=(const VHSVGRect &src);

    void set(int16_t x, int16_t y, int16_t w, int16_t h);
    void setpos(int16_t x, int16_t y);
    void setsiz(uint16_t w, uint16_t h);

    void move(int16_t dx, int16_t dy);
    void extdxdy(uint16_t dd = 1);
    void shrink(int dx, int dy);

    const int16_t startx() const;
    const int16_t endx() const;

    const int16_t starty() const;
    const int16_t endy() const;

    const uint16_t width() const;
    const uint16_t height() const;

private:
    int16_t sx;
    int16_t sy;
    uint16_t w;
    uint16_t h;
    int16_t ex;
    int16_t ey;
};
