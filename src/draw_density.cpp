#include "draw.hpp"
#include "strtools.hpp"
#include "global.hpp"

// white	#FFFFFF
// green	#80E080
// yellow	#FFFF80
// red		#FFA0A0

uint32_t	arrcolors[4] = { 0xFFFFFF, 0x80E080, 0xFFFF00, 0xFF8080 };


// ----------------------------------------------------------------------------
void rgb_to_compo( uint32_t v, uint8_t * r, uint8_t * g, uint8_t * b) {
	*r = (v >>16) & 0xFF; 	*g = (v >> 8) & 0xFF;	*b = (v >> 0) & 0xFF; }

// ----------------------------------------------------------------------------
uint8_t scaleval( uint8_t v1, uint8_t v2, uint8_t pos, uint8_t range) {
	uint8_t r;
	int delta = v2 - v1;
	r = v1 + (delta * pos / range);
	return r; }

// ----------------------------------------------------------------------------
std::string hexval( int v ) {
	char sss[3] = { 
		szhex()[ (v >> 4) & 0xF ],
		szhex()[ (v >> 0) & 0xF ], 0 };
	return std::string(sss); }

// ----------------------------------------------------------------------------
std::string strrgb(uint8_t r, uint8_t g, uint8_t b) {
	return "#" + hexval(r) + hexval(g) + hexval(b); }

// ----------------------------------------------------------------------------
std::string strrgb( uint32_t v) {
	uint8_t r = (v >>16) & 0xFF;
	uint8_t g = (v >> 8) & 0xFF;
	uint8_t b = (v >> 0) & 0xFF;
	return strrgb( r, g, b ); }

// ----------------------------------------------------------------------------
std::string lincolor(uint8_t val) {
	uint8_t		pos = val / 86; // /3
	uint8_t		dv  = val % 86;
	uint8_t		rf, gf, bf;
	uint8_t		rt, gt, bt;
	uint32_t	rgbf = arrcolors[pos];
	uint32_t	rgbt = arrcolors[pos+1];

	rgb_to_compo ( rgbf, & rf, & gf, & bf );
	rgb_to_compo ( rgbt, & rt, & gt, & bt );

	uint8_t		r = scaleval( rf, rt, dv, 86);
	uint8_t		g = scaleval( gf, gt, dv, 86);
	uint8_t		b = scaleval( bf, bt, dv, 86);

	return strrgb( r , g , b ); }

// ----------------------------------------------------------------------------
void	DrawDensity		( const VHRect & rect , const std::vector<stret> & arr , int blksize, int ws ) {

	int ww = rect.w;
	int h1 = rect.h * 0.8,	h2 = rect.h * 0.1,	h3 = rect.h * 0.1;
	int y1 = rect.sy,		y2 = y1 + h1,		y3 = y2 + h2;

	VHRect rectgfx(rect.sx , y1, ww, h1);	// Quantization
	VHRect rectdif(rect.sx , y2, ww, h2);	// Delta
	VHRect rectscl(rect.sx , y3, ww, h3);	// Scale

	float 	unitw 	= rectgfx.w * 1.0 / arr.size();


	int ymid 	= rectgfx.sy + rectgfx.h/2;
	int ymidu	= ymid - rectgfx.h / 4;
	int ymaxu	= ymid + rectgfx.h / 4;

	svg.line( rectgfx.sx, ymid  , rectgfx.ex, ymid  , 1, "gray", "10,10", "0.40" );
	svg.line( rectgfx.sx, ymidu , rectgfx.ex, ymidu , 1, "gray", "4,4"  , "0.25" );
	svg.line( rectgfx.sx, ymaxu , rectgfx.ex, ymaxu , 1, "gray", "4,4"  , "0.25" );



	for(int i=0; i < arr.size(); i++) {
		VHRect rectcol(rectdif.sx + unitw * i + 1, rectdif.sy, unitw, rectdif.h);
		int color = i * 255 / arr.size();

		std::string clr = lincolor(color);
		svg.rect(rectcol, 1, clr, clr);
		// draw_col_bck( i , rectcol );
	}
	
	DrawScale( rectscl, blksize );
}
