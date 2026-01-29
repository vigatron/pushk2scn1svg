#pragma once

#include "vhplatform.hpp"

class BlockProps {

	public:

		BlockProps() { }

		// ----------------------------------------------------------------------------
		void scan( uint8_t *ptr, int subsize ) {

			int min = 256;
			int max = -1;

			memset(spc, 0, sizeof(uint8_t) * 256);

			for( int i=0; i < subsize; i++ ) {
				uint8_t v = *ptr;
				if( v > max ) max = v; 
				if( v < min ) min = v;
				if(spc[v] < 255) spc[v]++;
				ptr++; }

			wght = 0;
			for(int i=0; i<256;i++) {
				if(spc[i]) if(wght<255) wght++; }
			
			_min = min; _max = max; }

	uint8_t _min;
	uint8_t _max;

	uint8_t spc[256];
	uint8_t wght;

};
