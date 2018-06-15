#pragma once
#include <memory>
#include <FontDef.hpp>

class FontPainter
{
protected:
	using DrawPixel = std::function< void( size_t, size_t, bool ) >;

public:
	struct Size
	{
		size_t width;
		size_t height;
	};

protected:
	FontPainter( FontData fontData, DrawPixel drawPixel );
	virtual ~FontPainter() = default;

public:
	virtual Size drawChar( size_t startX, size_t startY, char character, bool color )	 = 0;
	virtual Size drawString( size_t startX, size_t startY, std::string text, bool color ) = 0;
	virtual Size getCharSize( char character )											  = 0;
	virtual Size getStringSize( std::string text )										  = 0;

protected:
	DrawPixel m_drawPixel;
	FontData m_fontData;
};