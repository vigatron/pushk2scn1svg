#include "draw.hpp"
#include "global.hpp"
#include "misc.hpp"

// No path, it's just a filename
static std::string extract_filename(const std::string& fullPath) {
	size_t last_slash = fullPath.find_last_of("/\\");
	if (last_slash == std::string::npos) { return fullPath; }
	return fullPath.substr(last_slash + 1); }


verr GenerateDocument( const VHMemBlock & memblk , stInParams * pInParams ) {

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


	// Data types ( left )
	VHRect		rect_types( rect_2X4.sx, rect_2X4.ey, rect_2X4.w/2, hh_ftr );
	rect_types.shrink(64, 16);

	// Draw props ( right ) empty
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
	std::string basefname = extract_filename(pInParams->fname);
	std::string fnameout =  basefname + "_scn.svg";
	svg.savetosvg( fnameout );

	return vok;
}