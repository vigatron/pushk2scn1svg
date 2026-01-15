#pragma once

#include <stdint.h>

class VHRect {

    public:

		VHRect	( ) { }
		VHRect	( const VHRect & src) { *this = src; }
		VHRect	(int16_t x, int16_t y, int16_t w, int16_t h) {  set(x, y, w, h); }
		~VHRect	( ) { }

		VHRect & operator=( const VHRect & src ) {
			if(this != &src) {
				this->sx = src.sx; this->sy = src.sy;
				this->ex = src.ex; this->ey = src.ey;
				this->w  = src.w ; this->h  = src.h ; }
			return * this; }

		// void operator=(const VHRect & rect)     { set(rect.sx, rect.sy, rect.w, rect.h); }

		void set(int16_t x, int16_t y, int16_t w, int16_t h)
			{ sx = x; sy = y; this->w =  w; this->h = h; ex = x+w; ey = y+h; }

		void setpos		(int16_t x, int16_t y)		{ this->sx = x; this->sy = y; this->ex = x+w; this->ey = y+h; }
		void setsiz		(uint16_t w, uint16_t h)	{ this->w = w;  this->h = h; this->ex = sx+w; this->ey = sy+h; }
		void move		(int16_t dx, int16_t dy)	{ setpos(sx + dx, sy + dy); }
		void extdxdy	(uint16_t dd = 1)			{ sx -= dd; sy -= dd; ex += dd; ey += dd; w += dd + dd; h += dd + dd; }
		void shrink		(int dx, int dy)			{ set( sx + dx, sy + dy, w - dx*2, h - dy*2 ); }

		int16_t     sx; int16_t     sy;
		uint16_t    w;  uint16_t    h;
		int16_t     ex; int16_t     ey;
};
