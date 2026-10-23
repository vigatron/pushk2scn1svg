#include "draw.hpp"
#include "global.hpp"

#include "draw/colors.hpp"

// ----------------------------------------------------------------------------
void DrawShadowBox( const VHRect & rect ) {

	int dx = 16, dy = 24;

	VHRect rect_shadow = rect;
	rect_shadow.move(dx, dy);
	svg.rect(rect_shadow,	1, Colors::clrshd_f, Colors::clrshd_b  );
	svg.rect(rect,			1, Colors::clrshd_w, Colors::clrshd_bb ); }

// ----------------------------------------------------------------------------
void DrawScale( const VHRect & rect , int size) {

	int n 		= 0;
	int s 		= size;
	int fnsize 	= 18;

	while(s>256) {

		int dx 	= s * rect.w / size;
		int dxt	= dx + ( (s == size) ? - 64 : 6 );
		int dy = rect.h;
		VHRect r(rect.sx, rect.sy, dx, rect.h + 1);
		
		std::string colf = "#AAAAAA";
		std::string colb = (n++ & 1) ? "#D0D0D0" : "#E0E0E0";
		std::string val	 = std::to_string(s);

		svg.rect(r, 1, colf, colb);
		svg.text(r.sx + dxt, r.ey - fnsize/2, val, "verdana", fnsize, Colors::colortxt);
		s /= 2; }

	svg.text(rect.sx +  6, rect.ey - fnsize/2, "0", "verdana", fnsize, Colors::colortxt); }
