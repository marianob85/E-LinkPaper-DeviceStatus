#include <cstring>
#include "Painter.hpp"

using namespace std;

Paint::Paint( size_t width, size_t height )
{
	m_rotate = Rotate::Rotate0;
	/* 1 byte = 8 pixels, so the width should be the multiple of 8 */
	m_width  = width % 8 ? width + 8 - ( width % 8 ) : width;
	m_height = height;
}

Paint::~Paint() {}

/**
 *  @brief: clear the image
 */
void Paint::clear( Color color )
{
	for( size_t x = 0; x < m_width; x++ )
	{
		for( size_t y = 0; y < m_height; y++ )
		{
			drawAbsolutePixel( x, y, color );
		}
	}
}

size_t Paint::getWidth( void )
{
	return m_width;
}

size_t Paint::getHeight( void )
{
	return m_height;
}

Rotate Paint::getRotate( void )
{
	return m_rotate;
}

void Paint::setRotate( Rotate rotate )
{
	m_rotate = rotate;
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void Paint::drawPixel( size_t x, size_t y, Color color )
{
	size_t point_temp;
	if( m_rotate == Rotate::Rotate0 )
	{
		if( x < 0 || x >= m_width || y < 0 || y >= m_height )
		{
			return;
		}
		drawAbsolutePixel( x, y, color );
	}
	else if( m_rotate == Rotate::Rotate90 )
	{
		if( x < 0 || x >= m_height || y < 0 || y >= m_width )
		{
			return;
		}
		point_temp = x;
		x		   = m_width - y;
		y		   = point_temp;
		drawAbsolutePixel( x, y, color );
	}
	else if( m_rotate == Rotate::Rotate180 )
	{
		if( x < 0 || x >= m_width || y < 0 || y >= m_height )
		{
			return;
		}
		x = m_width - x;
		y = m_height - y;
		drawAbsolutePixel( x, y, color );
	}
	else if( m_rotate == Rotate::Rotate270 )
	{
		if( x < 0 || x >= m_height || y < 0 || y >= m_width )
		{
			return;
		}
		point_temp = x;
		x		   = y;
		y		   = m_height - point_temp;
		drawAbsolutePixel( x, y, color );
	}
}

bool Paint::merge( size_t offsetX, size_t offsetY, std::unique_ptr< Paint > painter )
{
	for( size_t x = 0; x < m_width - offsetX; x++ )
		for( size_t y = 0; y < m_height - offsetY; y++ )
			drawAbsolutePixel( offsetX + x, offsetY + y, painter->getAbsolutePixel( x, y ) );

	return true;
}

Paint::operator const uint8_t*() const
{
	return m_image.data();
}

/**
 *  @brief: this draws a line on the frame buffer
 */
void Paint::drawLine( size_t _x0, size_t _y0, size_t _x1, size_t _y1, Color color )
{
	int x0 = static_cast< int >( _x0 );
	int x1 = static_cast< int >( _x1 );
	int y0 = static_cast< int >( _y0 );
	int y1 = static_cast< int >( _y1 );

	/* Bresenham algorithm */
	int dx  = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
	int sx  = x0 < x1 ? 1 : -1;
	int dy  = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
	int sy  = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	while( ( x0 != x1 ) && ( y0 != y1 ) )
	{
		drawPixel( x0, y0, color );
		if( 2 * err >= dy )
		{
			err += dy;
			x0 += sx;
		}
		if( 2 * err <= dx )
		{
			err += dx;
			y0 += sy;
		}
	}
}

/**
 *  @brief: this draws a horizontal line on the frame buffer
 */
void Paint::drawHorizontalLine( size_t x, size_t y, size_t line_width, Color color )
{
	for( size_t i = x; i < x + line_width; i++ )
		drawPixel( i, y, color );
}

/**
 *  @brief: this draws a vertical line on the frame buffer
 */
void Paint::drawVerticalLine( size_t x, size_t y, size_t line_height, Color color )
{
	for( size_t i = y; i < y + line_height; i++ )
		drawPixel( x, i, color );
}

/**
 *  @brief: this draws a rectangle
 */
void Paint::drawRectangle( size_t _x0, size_t _y0, size_t _x1, size_t _y1, Color color )
{
	int x0 = static_cast< int >( _x0 );
	int x1 = static_cast< int >( _x1 );
	int y0 = static_cast< int >( _y0 );
	int y1 = static_cast< int >( _y1 );

	size_t min_x, min_y, max_x, max_y;
	min_x = x1 > x0 ? x0 : x1;
	max_x = x1 > x0 ? x1 : x0;
	min_y = y1 > y0 ? y0 : y1;
	max_y = y1 > y0 ? y1 : y0;

	drawHorizontalLine( min_x, min_y, max_x - min_x + 1, color );
	drawHorizontalLine( min_x, max_y, max_x - min_x + 1, color );
	drawVerticalLine( min_x, min_y, max_y - min_y + 1, color );
	drawVerticalLine( max_x, min_y, max_y - min_y + 1, color );
}

/**
 *  @brief: this draws a filled rectangle
 */
void Paint::drawFilledRectangle( size_t _x0, size_t _y0, size_t _x1, size_t _y1, Color color )
{
	int x0 = static_cast< int >( _x0 );
	int x1 = static_cast< int >( _x1 );
	int y0 = static_cast< int >( _y0 );
	int y1 = static_cast< int >( _y1 );

	size_t min_x, min_y, max_x, max_y;
	min_x = x1 > x0 ? x0 : x1;
	max_x = x1 > x0 ? x1 : x0;
	min_y = y1 > y0 ? y0 : y1;
	max_y = y1 > y0 ? y1 : y0;

	for( size_t i = min_x; i <= max_x; i++ )
		drawVerticalLine( i, min_y, max_y - min_y + 1, color );
}

/**
 *  @brief: this draws a circle
 */
void Paint::drawCircle( size_t _x, size_t _y, size_t _radius, Color color )
{
	int y	  = static_cast< int >( _y );
	int x	  = static_cast< int >( _x );
	int radius = static_cast< int >( _radius );

	/* Bresenham algorithm */
	int x_pos = -radius;
	int y_pos = 0;
	int err   = 2 - 2 * radius;
	int e2;

	do
	{
		drawPixel( x - x_pos, y + y_pos, color );
		drawPixel( x + x_pos, y + y_pos, color );
		drawPixel( x + x_pos, y - y_pos, color );
		drawPixel( x - x_pos, y - y_pos, color );
		e2 = err;
		if( e2 <= y_pos )
		{
			err += ++y_pos * 2 + 1;
			if( -x_pos == y_pos && e2 <= x_pos )
			{
				e2 = 0;
			}
		}
		if( e2 > x_pos )
		{
			err += ++x_pos * 2 + 1;
		}
	} while( x_pos <= 0 );
}

/**
 *  @brief: this draws a filled circle
 */
void Paint::drawFilledCircle( size_t _x, size_t _y, size_t _radius, Color color )
{
	int y	  = static_cast< int >( _y );
	int x	  = static_cast< int >( _x );
	int radius = static_cast< int >( _radius );
	/* Bresenham algorithm */
	int x_pos = -radius;
	int y_pos = 0;
	int err   = 2 - 2 * radius;
	int e2;

	do
	{
		drawPixel( x - x_pos, y + y_pos, color );
		drawPixel( x + x_pos, y + y_pos, color );
		drawPixel( x + x_pos, y - y_pos, color );
		drawPixel( x - x_pos, y - y_pos, color );
		drawHorizontalLine( x + x_pos, y + y_pos, 2 * ( -x_pos ) + 1, color );
		drawHorizontalLine( x + x_pos, y - y_pos, 2 * ( -x_pos ) + 1, color );
		e2 = err;
		if( e2 <= y_pos )
		{
			err += ++y_pos * 2 + 1;
			if( -x_pos == y_pos && e2 <= x_pos )
			{
				e2 = 0;
			}
		}
		if( e2 > x_pos )
		{
			err += ++x_pos * 2 + 1;
		}
	} while( x_pos <= 0 );
}

// start: ------------------- Paint2Colors -------------------

Paint2Colors::Paint2Colors( size_t width, size_t height ) : Paint( width, height )
{
	m_imageSize = m_width / 8 * m_height;
	m_image.reserve( m_imageSize );
	m_image.resize( m_imageSize );
}

Paint2Colors::~Paint2Colors() {}

void Paint2Colors::drawAbsolutePixel( size_t x, size_t y, Color color )
{
	if( x < 0 || x >= m_width || y < 0 || y >= m_height )
		return;

	switch( color )
	{
	case Color::White:
		m_image[ ( x + y * m_width ) / 8 ] |= static_cast< uint8_t >( 0x80 >> ( x % 8 ) );
		break;
	default:
		m_image[ ( x + y * m_width ) / 8 ] &= static_cast< uint8_t >( ~( 0x80 >> ( x % 8 ) ) );
	}
}

Color Paint2Colors::getAbsolutePixel( size_t x, size_t y ) const
{
	if( x < 0 || x >= m_width || y < 0 || y >= m_height )
		return Color::White;

	return m_image[ ( x + y * m_width ) / 8 ] & static_cast< uint8_t >( 0x80 >> ( x % 8 ) ) ? Color::White :
																							  Color::Black;
}

// end: --------------------- Paint2Colors -------------------

// start: ------------------- Paint3Colors -------------------

Paint3Colors::Paint3Colors( size_t width, size_t height ) : Paint( width, height )
{
	m_imageSize = m_width / 2 * m_height;
	m_image.reserve( m_imageSize );
	m_image.resize( m_imageSize );
}

Paint3Colors::~Paint3Colors() {}

void Paint3Colors::drawAbsolutePixel( size_t x, size_t y, Color color )
{
	if( x < 0 || x >= m_width || y < 0 || y >= m_height )
	{
		return;
	}
	switch( color )
	{
	case Color::Black:
		m_image[ ( x + y * m_width ) / 2 ] &= static_cast< uint8_t >( ~( 0xF0 >> ( x % 2 * 4 ) ) );
		break;
	case Color::White:
		m_image[ ( x + y * m_width ) / 2 ] &= static_cast< uint8_t >( ~( 0xF0 >> ( x % 2 * 4 ) ) );
		m_image[ ( x + y * m_width ) / 2 ] ^= static_cast< uint8_t >( 0x30 >> ( x % 2 * 4 ) );
		break;
	case Color::Color1:
	default:
		m_image[ ( x + y * m_width ) / 2 ] &= static_cast< uint8_t >( ~( 0xF0 >> ( x % 2 * 4 ) ) );
		m_image[ ( x + y * m_width ) / 2 ] ^= static_cast< uint8_t >( 0x40 >> ( x % 2 * 4 ) );
		break;

		/* do nothing */
		break;
	}
}

Color Paint3Colors::getAbsolutePixel( size_t x, size_t y ) const
{
	// About the image data: 4 bits = 1 pixel, doesn’t support Gray scale (can only display black and white). 0000
	// (binary) stands for a black pixel, 0011 (binary) stands for a white pixel, 0100 (binary) stands for a red pixel.
	// https://www.waveshare.com/wiki/7.5inch_e-Paper_HAT_(B)
	return Color::White;
}

// end: --------------------- Paint3Colors -------------------
