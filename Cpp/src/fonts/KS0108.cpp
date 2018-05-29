#include <cmath>
#include <cstdint>
#include "KS0108.hpp"

using namespace std;

// start: ------------------- FontPainterKS0108 -------------------

FontPainterKS0108::FontPainterKS0108( FontData fontData, DrawPixel drawPixel ) : FontPainter( fontData, drawPixel ) {}

FontPainter::Size FontPainterKS0108::drawChar( unsigned startX, unsigned startY, char character, bool color )
{
	return drawChar( startX, startY, character, color, m_drawPixel );
}

FontPainter::Size FontPainterKS0108::drawChar(
	unsigned startX, unsigned startY, char character, bool color, DrawPixel drawPixel )
{
	const auto chacterIndex = static_cast< uint8_t >( character ) - static_cast< uint8_t >( m_fontData.startCharacter );

	// Height should be always around to bytes
	const auto heightBytes   = static_cast< int >( ceil( float( m_fontData.height ) / 8 ) );
	const auto startByte	 = ( chacterIndex * ( heightBytes * 8 * m_fontData.width + 8 ) ) / 8;
	const CharData* charData = reinterpret_cast< const CharData* >( m_fontData.chars + startByte );

	if( drawPixel )
	{
		const unsigned short* dataPtr = charData->data;
		for( size_t x = 0; x < charData->width; ++x, dataPtr += heightBytes )
			for( size_t y = 0; y < m_fontData.height; ++y )
				if( 0x01 & ( *( dataPtr + ( y / 8 ) ) >> y % 8 ) )
					drawPixel( startX + x, startY + y, !color );
	}

	return { charData->width, m_fontData.height };
}

FontPainter::Size FontPainterKS0108::drawString( unsigned startX, unsigned startY, std::string text, bool color )
{
	return drawString( startX, startY, text, color, m_drawPixel );
}

FontPainter::Size FontPainterKS0108::drawString(
	unsigned startX, unsigned startY, std::string text, bool color, DrawPixel drawPixel )
{
	FontPainter::Size totalSize{ 0, 0 };
	size_t nextCharXPosition{ 0 };
	for( const auto& character : text )
	{
		auto size = drawChar( startX + nextCharXPosition, startY, character, color, drawPixel );
		nextCharXPosition += size.width + m_fontData.space;

		totalSize.height = max( totalSize.height, size.height );
		totalSize.width  = nextCharXPosition - m_fontData.space;
	}
	return totalSize;
}

FontPainter::Size FontPainterKS0108::getCharSize( char character )
{
	return drawChar( 0, 0, character, false, DrawPixel() );
}

FontPainter::Size FontPainterKS0108::getStringSize( std::string text )
{
	return drawString( 0, 0, text, false, DrawPixel() );
}
}

// end: --------------------- FontPainterKS0108 -------------------
