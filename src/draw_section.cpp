#include "draw.hpp"

#include "colors.hpp"
#include "global.hpp"

// ----------------------------------------------------------------------------
void draw_column(int idx, const VHRect & rect, const std::vector<stret> & arr ) {

    int ww = rect.w;
    int hh = rect.h;
    int sx = rect.sx;
    int ey = rect.ey;

    const stret & s = arr[idx];

    int gfx_hmin    = s.min * hh / 255;
    int gfx_hmax    = s.max * hh / 255;
    int gfx_hdiff   = gfx_hmax - gfx_hmin;
    svg.rect(sx, ey - gfx_hmax, ww, gfx_hdiff, 0, "", Colors::clrspr);

    int gfx_hwdt     = s.wght * hh / 128; // 255;
    int gfx_hmid    = gfx_hdiff / 2;
    int gfx_ymid    = ey - gfx_hmin - gfx_hmid - gfx_hwdt/2;
    int gfx_yund    = ey + 128 - gfx_hwdt/2;
    svg.rect(sx, gfx_ymid, ww, gfx_hwdt, 1, Colors::clrwdt, Colors::clrwdt ); }

// ----------------------------------------------------------------------------
void draw_col_bck( int idx, const VHRect & rect ) {
	// Draw background bar
	std::string color = (idx & 1) ? Colors::clrbck1 : Colors::clrbck2;
	svg.rect(rect, 0, color, color); }

// ----------------------------------------------------------------------------
void DrawSection( const VHRect & rect, const std::vector<stret> & arr , int blksize, int ws ) {

	int ww = rect.w;
	int h1 = rect.h * 0.8,	h2 = rect.h * 0.1,	h3 = rect.h * 0.1;
	int y1 = rect.sy,		y2 = y1 + h1,		y3 = y2 + h2;

	VHRect rectgfx(rect.sx , y1, ww, h1);	// Quantization
	VHRect rectdif(rect.sx , y2, ww, h2);	// Delta
	VHRect rectscl(rect.sx , y3, ww, h3);	// Scale

	float 	unitw 	= rect.w * 1.0 / arr.size();

	for(int i=0; i < arr.size(); i++) {
		VHRect rectcol(rect.sx + unitw * i, rect.sy, unitw, rectgfx.h);
		draw_col_bck( i , rectcol ); }

	for(int i=0; i < arr.size(); i++) {
		VHRect rectcol(rect.sx + unitw * i, rect.sy, unitw, rectgfx.h);
		draw_column(i, rectcol, arr); }
	
	int ymid 	= rectgfx.sy + rectgfx.h/2;
	int ymidu	= ymid - rectgfx.h / 4;
	int ymaxu	= ymid + rectgfx.h / 4;

	svg.line( rectgfx.sx, ymid  , rectgfx.ex, ymid  , 1, "gray", "10,10", "0.40" );
	svg.line( rectgfx.sx, ymidu , rectgfx.ex, ymidu , 1, "gray", "4,4"  , "0.25" );
	svg.line( rectgfx.sx, ymaxu , rectgfx.ex, ymaxu , 1, "gray", "4,4"  , "0.25" );


	svg.rect( rectgfx, 	1, "#AAAAAA");
	svg.rect( rectdif, 	1, "#AAAAAA");
	svg.rect( rectscl, 	1, "#AAAAAA");
	svg.rect( rect, 	1, "#CCCCCC");

	std::string txt = "Размер окна: " + std::to_string(ws) + " байт";
	svg.text( rect.sx + 14, rect.sy - 20, txt, "verdana", 32, Colors::colortxtsh2 );
	svg.text( rect.sx + 12, rect.sy - 22, txt, "verdana", 32, Colors::colortxt );

	DrawScale( rectscl, blksize ); }
