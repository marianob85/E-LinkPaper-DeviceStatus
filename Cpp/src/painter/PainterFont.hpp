#pragma once
#include <memory>
#include <FontDef.hpp>

class FontPainter
{
protected:
	using DrawPixel = std::function< void( unsigned, unsigned, bool ) >;

public:
	struct Size
	{
		unsigned width;
		unsigned height;
	};

protected:
	FontPainter( FontData fontData, DrawPixel drawPixel );
	virtual ~FontPainter() = default;

public:
	virtual Size drawChar( unsigned startX, unsigned startY, char character, bool color )	 = 0;
	virtual Size drawString( unsigned startX, unsigned startY, std::string text, bool color ) = 0;
	virtual Size getCharSize( char character )												  = 0;
	virtual Size getStringSize( std::string text )											  = 0;

protected:
	DrawPixel m_drawPixel;
	FontData m_fontData;
};