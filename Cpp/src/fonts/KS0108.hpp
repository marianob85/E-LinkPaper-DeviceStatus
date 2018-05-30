#pragma once

#include <FontDef.hpp>
#include <PainterFont.hpp>

class FontPainterKS0108 : public FontPainter
{
private:
	struct CharData
	{
		const unsigned short width;
		const unsigned short data[ 1 ];
	};

public:
	FontPainterKS0108( FontData fontData, DrawPixel drawPixel );

	// start: ------------------- FontPainter -------------------
	virtual Size drawChar( unsigned startX, unsigned startY, char character, bool color ) override;
	virtual Size drawString( unsigned startX, unsigned startY, std::string text, bool color ) override;
	virtual Size getCharSize( char character ) override;
	virtual Size getStringSize( std::string text ) override;
	// end: --------------------- FontPainter -------------------

protected:
	Size drawChar( unsigned startX, unsigned startY, char character, bool color, DrawPixel drawPixel );
	Size drawString( unsigned startX, unsigned startY, std::string text, bool color, DrawPixel drawPixel );
};


extern FontData fontGeorgia12;
extern FontData liberationMono8;
extern FontData liberationMono9;
extern FontData liberationMono9Bold;
extern FontData liberationMono10;
extern FontData liberationMono10Bold;
extern FontData liberationMono11Bold;
extern FontData liberationMono12;