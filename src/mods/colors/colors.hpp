#pragma once

#include <cstdint>
#include <string>

// ----------------------------------------------------------------------------
// Colors MAP
// ----------------------------------------------------------------------------
class Colors {

	public:

	static std::string		clrbck1;
	static std::string		clrbck2;

	static std::string		clrspr;

	static std::string		clrmin;
	static std::string		clrminl;

	static std::string		clrmax;
	static std::string		clrmaxl;

	static std::string		clrwdt;

	static std::string		clrshd_w;
	static std::string		clrshd_bb;
	static std::string		clrshd_f;
	static std::string		clrshd_b;

	// DNA Document
	static std::string		clrheadertxt;
	static std::string		clrempty;

	static std::string		clrcellalt1;
	static std::string		clrcellalt2;

	// Common lines / paper
	static std::string		clrline;
	static std::string		clrpaper;

	// Scale / Section frames
	static std::string		clrframe;
	static std::string		clrscalealt1;
	static std::string		clrscalealt2;
	static std::string		clrouterframe;

	// Markers / selection
	static std::string		clrmarker;
	static std::string		clrseltext;
	static std::string		clrselfill;

	// Density gradient (white -> green -> yellow -> red)
	static const uint32_t	colordensity[4];

	// Scale
	static std::string		colortxt;
	static std::string		colortxtsh;
	static std::string		colortxtsh2;

};
