#include "dbg.hpp"
#include "vhfile.hpp"
#include "vhmemblk.hpp"

#include <string.h>
#include <vector>
#include <filesystem>

#include "args.hpp"
#include "vhrect.hpp"
#include "svg.hpp"

#include "misc.hpp"


// TODO : Scale Kb

using namespace std;

DBG     dbg;
VHSVG   svg;

/*
[ ] sint8_t			[ ] float16			[ ] 	[ ] 5:5:5
[ ] uint8_t			[ ] double32		[ ] 	[ ] 6:5:5
[ ] sint16_t		[ ] double64		[ ] 	[ ] 5:6:5
[ ] uint16_t							[ ] 	[ ] 5:5:6
*/


// ----------------------------------------------------------------------------
// Colors MAP
// ----------------------------------------------------------------------------

std::string     clrbck1 = "#F8F8F8";
std::string     clrbck2 = "#FBFBFB";

std::string     clrspr  = "#C0D0FF";

std::string     clrmin  = "#2020FF";
std::string     clrminl = "#D0D0FF";

std::string     clrmax  = "#2020FF";
std::string     clrmaxl = "#D0D0FF";

std::string     clrwdt  = "#6090FF";


std::string clrshd_w 	= "#E0E0E0";
std::string clrshd_bb	= "#F8F8F8";
std::string clrshd_f 	= "#DADADA";
std::string clrshd_b 	= "#F0F0F0";


// ----------------------------------------------------------------------------
void DumpSamples( const std::vector<stret> & arr ) {
	int cnt = 0;
	for( const stret & s : arr) {
		dbg.itxt    ("BLK #%3d    ", cnt++ );
		dbg.i3txt   ("min=%4d  max=%4d  width=%d ", s.min, s.max, s.wght );
		dbg.nl(); } }


// ----------------------------------------------------------------------------
void draw_column(int idx, const VHRect & rect, const std::vector<stret> & arr ) {

    int ww = rect.w;
    int hh = rect.h;
    int sx = rect.sx;
    int ey = rect.ey;

    // Draw background bar
    std::string color = (idx & 1) ? clrbck1 : clrbck2;
    svg.rect(rect, 0, color, color);

    const stret & s = arr[idx];

    int gfx_hmin    = s.min * hh / 255;
    int gfx_hmax    = s.max * hh / 255;
    int gfx_hdiff   = gfx_hmax - gfx_hmin;
    svg.rect(sx, ey - gfx_hmax, ww, gfx_hdiff, 0, "", clrspr);

    int gfx_hwdt     = s.wght * hh / 128; // 255;
    int gfx_hmid    = gfx_hdiff / 2;
    int gfx_ymid    = ey - gfx_hmin - gfx_hmid - gfx_hwdt/2;
    int gfx_yund    = ey + 128 - gfx_hwdt/2;
    svg.rect(sx, gfx_ymid, ww, gfx_hwdt, 1, clrwdt, clrwdt );

    // svg.rect(sx, gfx_yund, ww, gfx_hwdt, 1, clrwdt, clrwdt );
}

// ----------------------------------------------------------------------------
void DrawShadowBox( const VHRect & rect ) {

	int dx = 32, dy = 32;

	VHRect rect_shadow = rect;
	rect_shadow.move(dx, dy);
	svg.rect(rect_shadow,	1, clrshd_f, clrshd_b  );
	svg.rect(rect,			1, clrshd_w, clrshd_bb ); }

std::string colortxt = "#777777";

// ----------------------------------------------------------------------------
void DrawScale( const VHRect & rect , int size) {

	int n 		= 0;
	int s 		= size;
	int fnsize 	= 18;

	svg.text(rect.sx +  6, rect.ey - fnsize/2, "0", "verdana", fnsize, colortxt);

	while(s>256) {

		int dx 	= s * rect.w / size;
		int dxt	= dx + ( (s == size) ? - 64 : 6 );
		int dy = rect.h / 2;
		VHRect r(rect.sx, rect.sy +dy, dx, rect.h / 2);
		
		std::string colf = "#AAAAAA";
		std::string colb = (n++ & 1) ? "#D0D0D0" : "#E0E0E0";
		std::string val	 = std::to_string(s);

		svg.rect(r, 1, colf, colb);
		svg.text(r.sx + dxt, r.ey - fnsize/2, val, "verdana", fnsize, colortxt);
		s /= 2; }

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
	svg.text( rect.sx + 12, rect.sy - 16, txt, "verdana", 32, colortxt );
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

// ----------------------------------------------------------------------------
int main ( int argc, char * argv[] ) {

    VHArgsParser argsparser;

    argsparser.setappname("pack81scansvg");

    argsparser.addopt(  "s",    "Block size");

    if( vok != argsparser.ParseArgs(argc, argv)) {
        std::cout << "Parse args issue" << std::endl;
        argsparser.Usage();
        return 1; }

    if( ! argsparser.single().size() ) {
        dbg.txtnl("USAGE: pack81scansvg <filename>");
        return 1; }

    string fname = argsparser.single()[0];
    if(!std::filesystem::exists(fname) || std::filesystem::is_directory(fname)) { 
        string msg = "Wrong input file " + fname;
        dbg.txtnl(msg.c_str());
        return 2; }

    int subsize = argsparser.getopt("s", 8);

    // Load source
    VHFile f(fname);

    int fsize = f.flen();
    if( fsize < subsize) {
        string msg = "Minimal size less than " + to_string(subsize);
        dbg.txtnl(msg.c_str()); return 3; }
    
    VHMemBlock memblk;
    if(!memblk.LoadFromFile(fname.c_str())) {
        string msg = "Cant read file " + to_string(subsize);
        dbg.txtnl(msg.c_str()); return 4; }


    // Generate SVG
	int         SVG_W 		= 3200, SVG_H = 2000;
	int         SVG_Border 	= 100;

	VHRect      rect_full_area( 0, 0, SVG_W, SVG_H );

	VHRect      rect_paper_area = rect_full_area;
	rect_paper_area.shrink( SVG_Border, SVG_Border );


    svg.begin( SVG_W, SVG_H );
    svg.rect(0,0, SVG_W, SVG_H, 0, "white", "white");
    Draw2X4Content( memblk, rect_paper_area );
    svg.end();

    // Save results
    std::string     fnameout = "result";
    svg.savetosvg( fnameout + ".svg");

    return 0; }

// VHRect      svg_cont_area( svg_draw_area.sx, svg_draw_area.sy, svg_draw_area.w, svg_draw_area.h/4 );
// ( SVG_Border, SVG_Border, SVG_W - SVG_Border * 2, SVG_H - SVG_Border * 2 );


// svg.line(sx, ymin, ex, ymin, 1, clrmin );
// svg.line(sx, ymax, ex, ymax, 1, clrmax );

// if(i) {
//     int xprev       = cx + unitw * (i-1);
//     int yprevmin    = y+256-arr[i-1].min;
//     int yprevmax    = y+256-arr[i-1].max;
//     svg.line(xprev + unitw/2, yprevmin, sx + unitw/2, ymin, 1, clrminl);
//     svg.line(xprev + unitw/2, yprevmax, sx + unitw/2, ymax, 1, clrmaxl);
// }
