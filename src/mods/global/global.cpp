#include "global.hpp"
#include "misc.hpp"

DBG     dbg;
VHSVG   svg;

// ----------------------------------------------------------------------------
verr LoadSource(std::string fname, int minsize, VHMemBlock * r) {

    VHFile f(fname);

    int fsize = f.flen();
    if( fsize < minsize) {
        std::string msg = "Minimal size less than " + std::to_string(minsize);
        dbg.txtnl(msg.c_str()); return 3; }
    
    if(! r->LoadFromFile(fname.c_str())) {
        std::string msg = "Cant read file " + std::to_string(minsize);
        dbg.txtnl(msg.c_str()); return 4; }

	return vok; }

// ----------------------------------------------------------------------------
void DumpSamples( const std::vector<BlockProps> & arr ) {
	int cnt = 0;
	for( const BlockProps & s : arr) {
		dbg.itxt    ("BLK #%3d    ", cnt++ );
		dbg.i3txt   ("min=%4d  max=%4d  width=%d ", s._min, s._max, s.wght );
		dbg.nl(); } }
