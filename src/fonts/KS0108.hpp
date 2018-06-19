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
	virtual Size drawChar( size_t startX, size_t startY, char character, Color color ) override;
	virtual Size drawString( size_t startX, size_t startY, std::string text, Color color ) override;
	virtual Size getCharSize( char character ) override;
	virtual Size getStringSize( std::string text ) override;
	// end: --------------------- FontPainter -------------------

protected:
	Size drawChar( size_t startX, size_t startY, char character, Color color, DrawPixel drawPixel );
	Size drawString( size_t startX, size_t startY, std::string text, Color color, DrawPixel drawPixel );
};

extern FontData fontGeorgia12;
extern FontData liberationMono8;
extern FontData liberationMono9;
extern FontData liberationMono9Bold;
extern FontData liberationMono10;
extern FontData liberationMono10Bold;
extern FontData liberationMono11Bold;
extern FontData liberationMono12;
extern FontData fontDigital_7_36Bold;
extern FontData fontArial28;
extern FontData fontEbrima28;
