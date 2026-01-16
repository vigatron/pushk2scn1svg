#include "draw.hpp"
#include "strtools.hpp"
#include "global.hpp"

std::string hexval( int v ) {

	char sss[3] = { 
		szhex()[ (v >> 4) & 0xF ],
		szhex()[ (v >> 0) & 0xF ], 0 };
	return std::string(sss); }

std::string strrgb(uint8_t r, uint8_t g, uint8_t b) {
	return "#" + hexval(r) + hexval(g) + hexval(b); }

void	DrawDensity		( const VHRect & rect , const std::vector<stret> & arr , int blksize, int ws ) {

	float 	unitw 	= rect.w * 1.0 / arr.size();

	for(int i=0; i < arr.size(); i++) {
		VHRect rectcol(rect.sx + unitw * i, rect.sy, unitw, rect.h);
		int color = i * 255 / arr.size();

		std::string clr = strrgb(0, 255, color );
		svg.rect(rectcol, 1, clr, clr);
		// draw_col_bck( i , rectcol );
	}
	
}
