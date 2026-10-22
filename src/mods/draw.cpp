#include "draw.hpp"
#include "global.hpp"
#include "misc.hpp"


// svg.rect(sx, gfx_yund, ww, gfx_hwdt, 1, clrwdt, clrwdt );

void DrawCenterLine( const VHRect & rect ) {

	int ymid 	= rect.sy + rect.h/2;
	int ymidu	= ymid - rect.h / 4;
	int ymaxu	= ymid + rect.h / 4;

	svg.line( rect.sx, ymid  , rect.ex, ymid  , 1, "gray", "10,10", "0.40" );
	svg.line( rect.sx, ymidu , rect.ex, ymidu , 1, "gray", "4,4"  , "0.25" );
	svg.line( rect.sx, ymaxu , rect.ex, ymaxu , 1, "gray", "4,4"  , "0.25" );
}

// ----------------------------------------------------------------------------
void Draw2X4Content(const VHMemBlock & memblk , const VHRect & rect ) {

	int ww = rect.w / 2;
	int hh = rect.h / 4;

	for(int j=0; j < 4; j++) {
		for(int i=0; i < 2; i++) {

			int chunksize = 64 << j;
			const std::vector<BlockProps> & arr = GenSamplesArray( memblk, chunksize );

			VHRect rects(rect.sx + i*ww, rect.sy + j*hh, ww, hh);
			rects.shrink( 64, 64 );

			DrawShadowBox(rects);
			if(!i)	DrawSection( rects, arr, memblk.sz(), chunksize );
			else 	DrawDensity( rects, arr, memblk.sz(), chunksize );
		}
	}
}


