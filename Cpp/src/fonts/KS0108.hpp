#pragma once

#include <FontDef.hpp>
#include <PainterFont.hpp>

extern FontData fontGeorgia12;

class FontPainterKS0108 : public FontPainter
{
public:
	FontPainterKS0108( FontData fontData, DrawPixel drawPixel );

	virtual Size drawChar( unsigned startX, unsigned startY, char character, bool color ) override;
};
