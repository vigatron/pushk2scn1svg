#include "draw.hpp"
#include "global.hpp"
#include "misc.hpp"


// svg.rect(sx, gfx_yund, ww, gfx_hwdt, 1, clrwdt, clrwdt );

// ----------------------------------------------------------------------------
void Draw2X4Content(const VHMemBlock & memblk , const VHRect & rect ) {

	int ww = rect.w / 2;
	int hh = rect.h / 4;

	for(int j=0; j < 4; j++) {
		for(int i=0; i < 2; i++) {

			int chunksize = 32 << j; // * (j+1) * (j+1);
			const std::vector<stret> & arr = GenSamplesArray( memblk, chunksize );

			VHRect rects(rect.sx + i*ww, rect.sy + j*hh, ww, hh);
			rects.shrink( 64, 64 );

			DrawShadowBox(rects);
			if(!i)	DrawSection( rects, arr, memblk.sz(), chunksize );
			else 	DrawDensity( rects, arr, memblk.sz(), chunksize );
		}
	}
}


verr GenerateDocument( const VHMemBlock & memblk ) {

    // SVG Defines
	int         SVG_W 		= 3200, SVG_H = 2400;
	int         SVG_Border 	= 100;

	VHRect      rect_full_area( 0, 0, SVG_W, SVG_H );

	int 		hh_hdr = 200;
	int 		hh_ftr = 200;

	// Paper area including border
	VHRect      rect_paper_area = rect_full_area;
	rect_paper_area.shrink	( SVG_Border, SVG_Border );

	VHRect		rect_2X4 = rect_paper_area;
	rect_2X4.shrink	( 0, ( hh_hdr + hh_ftr )  / 2 );
	rect_2X4.setpos	( rect_paper_area.sx, rect_paper_area.sy + hh_hdr );

	VHRect		rect_types( rect_2X4.sx, rect_2X4.ey, rect_2X4.w/2, hh_ftr );
	rect_types.shrink(64, 16);

	VHRect		rect_props = rect_types;
	rect_props.move( rect_2X4.w/2, 0);

	// SVG Generation
	svg.begin( SVG_W, SVG_H );
	svg.rect(0,0, SVG_W, SVG_H, 0, "white", "white");
	// svg.rect( rect_paper_area, 1, "#AAAAAA" );
	Draw2X4Content( memblk, rect_2X4 );
	DrawTypes(rect_types);
	DrawProps(rect_props);
	svg.end();

	// Save results
	std::string fnameout = "result";
	svg.savetosvg( fnameout + ".svg");

	return vok;
}