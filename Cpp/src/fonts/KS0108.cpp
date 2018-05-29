#include <cmath>
#include <cstdint>
#include "KS0108.hpp"

using namespace std;

// start: ------------------- FontPainterKS0108 -------------------

FontPainterKS0108::FontPainterKS0108( FontData fontData, DrawPixel drawPixel ) : FontPainter( fontData, drawPixel ) {}

FontPainter::Size FontPainterKS0108::drawChar( unsigned startX, unsigned startY, char character, bool color )
{
	const auto chacterIndex = static_cast< uint8_t >( character ) - static_cast< uint8_t >( m_fontData.startCharacter );

	// Height should be always around to bytes
	const auto heightBytes   = static_cast< int >( ceil( float( m_fontData.height ) / 8 ) );
	const auto startByte	 = ( chacterIndex * ( heightBytes * 8 * m_fontData.width + 8 ) ) / 8;
	const CharData* charData = reinterpret_cast< const CharData* >( m_fontData.chars + startByte );

	const unsigned short* dataPtr = charData->data;
	for( size_t x = 0; x < charData->width; ++x, dataPtr += heightBytes )
		for( size_t y = 0; y < m_fontData.height; ++y )
			if( 0x01 & ( *( dataPtr + ( y / 8 ) ) >> y % 8 ) )
				m_drawPixel( startX + x, startY + y, !color );

	return { charData->width, m_fontData.height };
}

FontPainter::Size FontPainterKS0108::drawString( unsigned startX, unsigned startY, std::string text, bool color )
{
	return {};
}

// end: --------------------- FontPainterKS0108 -------------------
