#include <cstring>
#include <functional>
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
	for( int x = 0; x < static_cast< int >( m_width ); x++ )
	{
		for( int y = 0; y < static_cast< int >( m_height ); y++ )
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
void Paint::drawPixel( int x, int y, Color color )
{
	size_t point_temp;
	if( m_rotate == Rotate::Rotate0 )
	{
		if( x < 0 || x >= static_cast< int >( m_width ) || y < 0 || y >= static_cast< int >( m_height ) )
		{
			return;
		}
		drawAbsolutePixel( x, y, color );
	}
	else if( m_rotate == Rotate::Rotate90 )
	{
		if( x < 0 || x >= static_cast< int >( m_height ) || y < 0 || y >= static_cast< int >( m_width ) )
		{
			return;
		}
		point_temp = x;
		x		   = static_cast< int >( m_width ) - y;
		y		   = point_temp;
		drawAbsolutePixel( x, y, color );
	}
	else if( m_rotate == Rotate::Rotate180 )
	{
		if( x < 0 || x >= static_cast< int >( m_width ) || y < 0 || y >= static_cast< int >( m_height ) )
		{
			return;
		}
		x = static_cast< int >( m_width ) - x;
		y = static_cast< int >( m_height ) - y;
		drawAbsolutePixel( x, y, color );
	}
	else if( m_rotate == Rotate::Rotate270 )
	{
		if( x < 0 || x >= static_cast< int >( m_height ) || y < 0 || y >= static_cast< int >( m_width ) )
		{
			return;
		}
		point_temp = x;
		x		   = y;
		y		   = static_cast< int >( m_height ) - point_temp;
		drawAbsolutePixel( x, y, color );
	}
}

bool Paint::merge( int offsetX, int offsetY, std::unique_ptr< Paint > painter )
{
	for( int x = 0; x < static_cast< int >( m_width ) - offsetX; x++ )
		for( int y = 0; y < static_cast< int >( m_height ) - offsetY; y++ )
			drawAbsolutePixel( offsetX + x, offsetY + y, painter->getAbsolutePixel( x, y ) );

	return true;
}

bool Paint::merge( int offsetX, int offsetY, std::unique_ptr< PaintMerger > mergeObject )
{
	for( int x = 0; x < static_cast< int >( mergeObject->width() ); x++ )
		for( int y = 0; y < static_cast< int >( mergeObject->height() ); y++ )
			drawAbsolutePixel( offsetX + x, offsetY + y, mergeObject->getAbsolutePixel( x, y ) );

	return true;
}

Paint::operator const uint8_t*() const
{
	return m_image.data();
}

/**
 *  @brief: this draws a line on the frame buffer
 */
void Paint::drawLine( int _x0, int _y0, int _x1, int _y1, Color color )
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
void Paint::drawHorizontalLine( int x, int y, size_t line_width, Color color )
{
	for( int i = x; i < x + static_cast< int >( line_width ); i++ )
		drawPixel( i, y, color );
}

/**
 *  @brief: this draws a vertical line on the frame buffer
 */
void Paint::drawVerticalLine( int x, int y, size_t line_height, Color color )
{
	for( int i = y; i < y + static_cast< int >( line_height ); i++ )
		drawPixel( x, i, color );
}

/**
 *  @brief: this draws a rectangle
 */
void Paint::drawRectangle( int _x0, int _y0, int _x1, int _y1, Color color )
{
	int x0 = _x0;
	int x1 = _x1;
	int y0 = _y0;
	int y1 = _y1;

	int min_x, min_y, max_x, max_y;
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
void Paint::drawFilledRectangle( int _x0, int _y0, int _x1, int _y1, Color color )
{
	int x0 = static_cast< int >( _x0 );
	int x1 = static_cast< int >( _x1 );
	int y0 = static_cast< int >( _y0 );
	int y1 = static_cast< int >( _y1 );

	int min_x, min_y, max_x, max_y;
	min_x = x1 > x0 ? x0 : x1;
	max_x = x1 > x0 ? x1 : x0;
	min_y = y1 > y0 ? y0 : y1;
	max_y = y1 > y0 ? y1 : y0;

	for( int i = min_x; i <= max_x; i++ )
		drawVerticalLine( i, min_y, max_y - min_y + 1, color );
}

/**
 *  @brief: this draws a circle
 */
void Paint::drawCircle( int _x, int _y, size_t _radius, Color color )
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
void Paint::drawFilledCircle( int _x, int _y, size_t _radius, Color color )
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

void Paint2Colors::drawAbsolutePixel( int x, int y, Color color )
{
	if( x < 0 || x >= static_cast< int >( m_width ) || y < 0 || y >= static_cast< int >( m_height ) )
		return;

	switch( color )
	{
	case Color::White:
		m_image[ ( x + y * static_cast< int >( m_width ) ) / 8 ] |= static_cast< uint8_t >( 0x80 >> ( x % 8 ) );
		break;
	default:
		m_image[ ( x + y * static_cast< int >( m_width ) ) / 8 ] &= static_cast< uint8_t >( ~( 0x80 >> ( x % 8 ) ) );
	}
}

Color Paint2Colors::getAbsolutePixel( int x, int y ) const
{
	if( x < 0 || x >= static_cast< int >( m_width ) || y < 0 || y >= static_cast< int >( m_height ) )
		return Color::White;

	return m_image[ ( x + y * static_cast< int >( m_width ) ) / 8 ] & static_cast< uint8_t >( 0x80 >> ( x % 8 ) ) ?
		Color::White :
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

void Paint3Colors::drawAbsolutePixel( int x, int y, Color color )
{
	if( x < 0 || x >= static_cast< int >( m_width ) || y < 0 || y >= static_cast< int >( m_height ) )
	{
		return;
	}
	switch( color )
	{
	case Color::Black:
		m_image[ ( x + y * static_cast< int >( m_width ) ) / 2 ]
			&= static_cast< uint8_t >( ~( 0xF0 >> ( x % 2 * 4 ) ) );
		break;
	case Color::White:
		m_image[ ( x + y * static_cast< int >( m_width ) ) / 2 ]
			&= static_cast< uint8_t >( ~( 0xF0 >> ( x % 2 * 4 ) ) );
		m_image[ ( x + y * static_cast< int >( m_width ) ) / 2 ] ^= static_cast< uint8_t >( 0x30 >> ( x % 2 * 4 ) );
		break;
	case Color::Color1:
	default:
		m_image[ ( x + y * static_cast< int >( m_width ) ) / 2 ]
			&= static_cast< uint8_t >( ~( 0xF0 >> ( x % 2 * 4 ) ) );
		m_image[ ( x + y * static_cast< int >( m_width ) ) / 2 ] ^= static_cast< uint8_t >( 0x40 >> ( x % 2 * 4 ) );
		break;

		/* do nothing */
		break;
	}
}

Color Paint3Colors::getAbsolutePixel( int x, int y ) const
{
	// About the image data: 4 bits = 1 pixel, doesn\92t support Gray scale (can only display black and white). 0000
	// (binary) stands for a black pixel, 0011 (binary) stands for a white pixel, 0100 (binary) stands for a red pixel.
	// https://www.waveshare.com/wiki/7.5inch_e-Paper_HAT_(B)

	if( x < 0 || x >= static_cast< int >( m_width ) || y < 0 || y >= static_cast< int >( m_height ) )
		return Color::White;

	auto color = m_image[ ( x + y * static_cast< int >( m_width ) ) / 2 ] >> ( ( x % 2 ) ? 0 : 4 ) & 0b1111;

	switch( color )
	{
	case 0b0:
		return Color::Black;
	case 0b0011:
		return Color::White;
	case 0b0100:
		return Color::Color1;
	}

	return Color::White;
}

// end: --------------------- Paint3Colors -------------------
