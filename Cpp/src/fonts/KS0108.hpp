#pragma once

#include <FontDef.hpp>
#include <PainterFont.hpp>

extern FontData fontGeorgia12;

class FontPainterKS0108 : public FontPainter
{
private:
#pragma pack( push, 1 )
	struct CharData
	{
		const unsigned short width;
		const unsigned short data[ 1 ];
	};
#pragma pack( pop )

public:
	FontPainterKS0108( FontData fontData, DrawPixel drawPixel );

	// start: ------------------- FontPainter -------------------
	virtual Size drawChar( unsigned startX, unsigned startY, char character, bool color ) override;
	virtual Size drawString( unsigned startX, unsigned startY, std::string text, bool color ) override;
	// end: --------------------- FontPainter -------------------
};
