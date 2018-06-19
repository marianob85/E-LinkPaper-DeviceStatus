#include <cmath>
#include "Image.hpp"

// start: ------------------- ImageXX -------------------
ImageXX::ImageXX( Image image ) : m_image( image ) {}

Color ImageXX::getAbsolutePixel( size_t x, size_t y )
{
	const auto widthBytes = static_cast< int >( ceil( float( m_image.width ) / 8 ) );

	return m_image.data[ ( x + y * ( widthBytes * 8 ) ) / 8 ] & static_cast< uint8_t >( 0x80 >> ( x % 8 ) ) ?
		Color::White :
		Color::Black;
}

size_t ImageXX::width() const
{
	return m_image.width;
}

size_t ImageXX::height() const
{
	return m_image.height;
}

// end: --------------------- ImageXX -------------------
