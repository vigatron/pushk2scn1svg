#include "draw.hpp"
#include "global.hpp"
#include "misc.hpp"

// ----------------------------------------------------------------------------
void draw_column(int idx, const VHRect & rect, const std::vector<stret> & arr ) {

    int ww = rect.w;
    int hh = rect.h;
    int sx = rect.sx;
    int ey = rect.ey;

    // Draw background bar
    std::string color = (idx & 1) ? Colors::clrbck1 : Colors::clrbck2;
    svg.rect(rect, 0, color, color);

    const stret & s = arr[idx];

    int gfx_hmin    = s.min * hh / 255;
    int gfx_hmax    = s.max * hh / 255;
    int gfx_hdiff   = gfx_hmax - gfx_hmin;
    svg.rect(sx, ey - gfx_hmax, ww, gfx_hdiff, 0, "", Colors::clrspr);

    int gfx_hwdt     = s.wght * hh / 128; // 255;
    int gfx_hmid    = gfx_hdiff / 2;
    int gfx_ymid    = ey - gfx_hmin - gfx_hmid - gfx_hwdt/2;
    int gfx_yund    = ey + 128 - gfx_hwdt/2;
    svg.rect(sx, gfx_ymid, ww, gfx_hwdt, 1, Colors::clrwdt, Colors::clrwdt );

    // svg.rect(sx, gfx_yund, ww, gfx_hwdt, 1, clrwdt, clrwdt );
}

// ----------------------------------------------------------------------------
void DrawSection( const VHRect & rect, const std::vector<stret> & arr , int blksize, int ws ) {

	int ww = rect.w;

	int h1 = rect.h * 0.6,	h2 = rect.h * 0.2,	h3 = rect.h * 0.2;
	int y1 = rect.sy,		y2 = y1 + h1,		y3 = y2 + h2;

	VHRect rectgfx(rect.sx , y1, ww, h1);	// Quantization
	VHRect rectdif(rect.sx , y2, ww, h2);	// Delta
	VHRect rectscl(rect.sx , y3, ww, h3);	// Scale
	DrawScale( rectscl, blksize );

	float 	unitw 	= rect.w * 1.0 / arr.size();
	int		cx 		= rect.sx;
	int		cy 		= rect.sy;

    for(int i=0; i < arr.size(); i++) {
        VHRect rectcol(cx + unitw * i, cy, unitw, rectgfx.h);
        draw_column(i, rectcol, arr); }

	svg.rect( rectgfx, 	1, "#AAAAAA");
	svg.rect( rectdif, 	1, "#AAAAAA");
	svg.rect( rectscl, 	1, "#AAAAAA");
	svg.rect( rect, 	1, "#CCCCCC");

	std::string txt = "Размер окна: " + std::to_string(ws);
	svg.text( rect.sx + 12, rect.sy - 16, txt, "verdana", 32, Colors::colortxt );
}

// ----------------------------------------------------------------------------
void Draw2X4Content(const VHMemBlock & memblk , const VHRect & rect ) {

	int ww = rect.w / 2;
	int hh = rect.h / 4;

	for(int j=0; j < 4; j++) {
		for(int i=0; i < 2; i++) {

			int chunksize = 64 * (j+1);

			VHRect rect_section(rect.sx + i*ww, rect.sy + j*hh, ww, hh);
			rect_section.shrink( 64, 64 );
			const std::vector<stret> & arr = GenSamplesArray( memblk, chunksize );

			DrawShadowBox(rect_section);
			if(!i) DrawSection( rect_section, arr, memblk.sz(), chunksize );
		}
	}
}


verr GenerateDocument( const VHMemBlock & memblk ) {

    // SVG Defines
	int         SVG_W 		= 3200, SVG_H = 2000;
	int         SVG_Border 	= 100;

	VHRect      rect_full_area( 0, 0, SVG_W, SVG_H );
	VHRect      rect_paper_area = rect_full_area;

	rect_paper_area.shrink( SVG_Border, SVG_Border );

	// SVG Generation
    svg.begin( SVG_W, SVG_H );
    svg.rect(0,0, SVG_W, SVG_H, 0, "white", "white");
    Draw2X4Content( memblk, rect_paper_area );
    svg.end();

    // Save results
    std::string     fnameout = "result";
    svg.savetosvg( fnameout + ".svg");

	return vok;
}