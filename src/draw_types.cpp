#include "draw.hpp"
#include "global.hpp"

/*
[ ] sint8_t			[ ] float16			[ ] 	[ ] 5:5:5
[ ] uint8_t			[ ] double32		[ ] 	[ ] 6:5:5
[ ] sint16_t		[ ] double64		[ ] 	[ ] 5:6:5
[ ] uint16_t							[ ] 	[ ] 5:5:6
*/

std::vector<std::string> arrtypes = {
	"sint8_t" ,
	"uint8_t" ,
	"",
	"",

	"sint16_t (BE)" ,
	"sint16_t (LE)" ,
	"uint16_t (BE)" ,
	"uint16_t (LE)" ,

	"sint32_t (BE)" ,
	"sint32_t (LE)" ,
	"uint32_t (BE)" ,
	"uint32_t (LE)" ,

	"float16",
	"double32",
	"double64",
	"",

	"5:5:5",
	"5:5:6",
	"5:6:5",
	"6:5:5",

	"4:4:4:4",
	"  4:4:8",
	"  8:8:8",
	"8:8:8:8",

};


std::string gettype(int i, int j) {
	int n = i * 4 + j;
	return arrtypes[n]; }

void DrawTypes( const VHRect & rect ) {

	// svg.rect(rect, 1, "red");

	int cols = 6;
	int rows = 4;

	for(int j = 0; j < rows; j++) {
		for( int i = 0; i < cols; i++) {

			int ww = rect.w / cols;
			int hh = rect.h / rows;

			int x = rect.sx + ww * i;
			int y = rect.sy + hh * j;

			std::string strt = gettype(i, j);

			svg.text( x +  0, y + 22, "[ X ]", "verdana", 26, Colors::colortxt );
			svg.text( x + 64, y + 22, strt	, "verdana", 26, Colors::colortxt );
		}
	}
}