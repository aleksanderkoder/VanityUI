#pragma once
#include "Elements.h"

// Utility functions

void ComputeDimensionsOfText(TTF_Font* font, int textLength, int& widthRef, int& heightRef) {
	// Compute width and height 
	int minx, maxx, miny, maxy;
	TTF_GlyphMetrics(font, 65, &minx, &maxx, &miny, &maxy, nullptr);
	widthRef = (maxx - minx) * textLength;
	heightRef = (maxy - miny);
}