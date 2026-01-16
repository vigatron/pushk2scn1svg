#include "misc.hpp"

verr verrmsg(int x, std::string strerr) {
    std::cout << strerr << std::endl;
    return x; }

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
