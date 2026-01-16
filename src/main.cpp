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
#include "global.hpp"
#include "draw.hpp"


struct stInParams {
	std::string		fname;
	int				subsize;
};

// ----------------------------------------------------------------------------
static verr ParseArgs( int argc , char * argv[] , stInParams * pInParams ) {

    VHArgsParser argsparser;

    argsparser.setappname("pack81scansvg");

    argsparser.addopt( "s", "Block size");

    if( vok != argsparser.ParseArgs(argc, argv)) {
        std::cout << "Parse args issue" << std::endl;
        argsparser.Usage();
        return verror(1); }

    if( ! argsparser.single().size() ) {
        dbg.txtnl("USAGE: pack81scansvg <filename>");
        return verror(2); }

    pInParams->fname = argsparser.single()[0];
    if(!std::filesystem::exists(pInParams->fname) ||
		std::filesystem::is_directory(pInParams->fname)) { 
        std::string msg = "Wrong input file " + pInParams->fname;
        dbg.txtnl(msg.c_str());
        return verror(3); }

    pInParams->subsize = argsparser.getopt("s", 8);

	return vok; }

// ----------------------------------------------------------------------------
int main ( int argc, char * argv[] ) {

	stInParams	sinParams;
	if(vok != ParseArgs( argc , argv , & sinParams))
		return verror(1);

	// Load source
	VHMemBlock memblk;
	if( vok != LoadSource( sinParams.fname , sinParams.subsize , & memblk ) )
		return verror(1);

	verr r = GenerateDocument( memblk );
	return (r == vok) ? 0 : 1; }

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
