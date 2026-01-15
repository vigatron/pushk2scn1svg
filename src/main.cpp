#include "dbg.hpp"
#include "vhfile.hpp"
#include "vhmemblk.hpp"

#include <string.h>
#include <vector>
#include <filesystem>

#include "args.hpp"
#include "vhrect.hpp"
#include "svg.hpp"


// TODO : Scale Kb

using namespace std;

DBG     dbg;
VHSVG   svg;


struct stret { uint8_t min; uint8_t max; uint8_t wght; };


verr verrmsg(int x, std::string strerr) {
    std::cout << strerr << std::endl;
    return x; }

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

// ----------------------------------------------------------------------------
void scanblk(uint8_t *ptr, stret * r, int subsize ) {

    int min = 256;
    int max = -1;

    uint8_t spc[256];
    memset(spc, 0, sizeof(uint8_t) * 256);

    for( int i=0; i < subsize; i++ ) {
        if( *ptr > max ) max = *ptr; 
        if( *ptr < min ) min = *ptr;
        spc[*ptr] = 1; ptr++; }

    int cnt = 0;
    for(int i=0; i<256;i++) { if(spc[i]) cnt++; }
    r->min  = min; r->max  = max; r->wght = cnt; }

// ----------------------------------------------------------------------------
std::vector<stret> GenSamplesArray( const VHMemBlock & mblk , int subsize ) {
	std::vector<stret> r;

    // Process block
    int blkcnt = mblk.sz() / subsize;
    for(int i=0; i<blkcnt;i++) {
        stret s;
        scanblk( mblk() + i * subsize, &s, subsize);
        r.push_back(s); }

	return r; }

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

    // Draw background
    std::string color = (idx & 1) ? clrbck1 : clrbck2;
    svg.rect(rect.sx, rect.sy, rect.w, hh, 0, color, color);

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

void GenerateSection( const VHRect & rect, const std::vector<stret> & arr ) {

	int ww = rect.w;

	int h1 = rect.h * 0.7,	h2 = rect.h * 0.2,	h3 = rect.h * 0.1;
	int y1 = rect.sy,		y2 = y1 + h1,		y3 = y2 + h2;

	VHRect rectgfx(rect.sx , y1, ww, h1);
	VHRect rectdif(rect.sx , y2, ww, h2);
	VHRect rectscl(rect.sx , y3, ww, h3);

	float 	unitw 	= rect.w * 1.0 / arr.size();
	int		cx 		= rect.sx;
	int		cy 		= rect.sy;

    for(int i=0; i < arr.size(); i++) {
        VHRect rectcol(cx + unitw * i, cy, unitw, rectgfx.h);
        draw_column(i, rectcol, arr); }

	{ const VHRect *p = & rectgfx;	svg.rect( p->sx, p->sy, p->w, p->h, 1, "#AAAAAA"); }
	{ const VHRect *p = & rectdif;	svg.rect( p->sx, p->sy, p->w, p->h, 1, "#AAAAAA"); }
	{ const VHRect *p = & rectscl;	svg.rect( p->sx, p->sy, p->w, p->h, 1, "#AAAAAA"); }

	{ const VHRect *p = & rect;		svg.rect( p->sx, p->sy, p->w, p->h, 1, "#CCCCCC"); }

}

// ----------------------------------------------------------------------------
void GenerateContent(const VHMemBlock & memblk , const VHRect & rect ) {

	int ww = rect.w / 2;
	int hh = rect.h / 4;

	for(int j=0; j < 4; j++) {
		for(int i=0; i < 2; i++) {
			VHRect rect_section(rect.sx + i*ww, rect.sy + j*hh, ww, hh);
			rect_section.shrink( 40, 40 );
			const std::vector<stret> & arr = GenSamplesArray( memblk, 64 * (j+1) );
			GenerateSection( rect_section, arr ); } }
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
    GenerateContent( memblk, rect_paper_area );
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
