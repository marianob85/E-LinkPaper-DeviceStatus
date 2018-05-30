#include <cstring>
#include "Painter.hpp"

using namespace std;

Paint::Paint( std::unique_ptr< uint8_t[] > image, int width, int height )
{
	this->m_rotate = Rotate::Rotate0;
	this->m_image  = move( image );
	/* 1 byte = 8 pixels, so the width should be the multiple of 8 */
	this->m_width  = width % 8 ? width + 8 - ( width % 8 ) : width;
	this->m_height = height;
}

Paint::Paint( Paint& paint )
{
	this->operator=( paint );
}

Paint::~Paint() {}

Paint& Paint::operator=( const Paint& right )
{
	m_width  = right.m_width;
	m_height = right.m_height;
	m_rotate = right.m_rotate;

	const auto size = m_width / 8 * m_height;
	m_image			= make_unique< uint8_t[] >( size );
	memcpy( m_image.get(), right.m_image.get(), size * sizeof( uint8_t ) );
	return *this;
}

/**
 *  @brief: clear the image
 */
void Paint::clear( bool colored )
{
	for( int x = 0; x < this->m_width; x++ )
	{
		for( int y = 0; y < this->m_height; y++ )
		{
			drawAbsolutePixel( x, y, colored );
		}
	}
}

/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 */
void Paint::drawAbsolutePixel( int x, int y, bool colored )
{
	if( x < 0 || x >= this->m_width || y < 0 || y >= this->m_height )
	{
		return;
	}
	if( IF_INVERT_COLOR )
	{
		if( colored )
		{
			rawImage()[ ( x + y * this->m_width ) / 8 ] |= static_cast< uint8_t >( 0x80 >> ( x % 8 ) );
		}
		else
		{
			rawImage()[ ( x + y * this->m_width ) / 8 ] &= static_cast< uint8_t >( ~( 0x80 >> ( x % 8 ) ) );
		}
	}
	else
	{
		if( colored )
		{
			rawImage()[ ( x + y * this->m_width ) / 8 ] &= static_cast< uint8_t >( ~( 0x80 >> ( x % 8 ) ) );
		}
		else
		{
			rawImage()[ ( x + y * this->m_width ) / 8 ] |= static_cast< uint8_t >( 0x80 >> ( x % 8 ) );
		}
	}
}

bool Paint::getAbsolutePixel( unsigned x, unsigned y ) const
{
	return rawImage()[ ( x + y * this->m_width ) / 8 ] & static_cast< uint8_t >( 0x80 >> ( x % 8 ) ) ? true : false;
}

int Paint::getWidth( void )
{
	return this->m_width;
}

int Paint::getHeight( void )
{
	return this->m_height;
}

Rotate Paint::getRotate( void )
{
	return this->m_rotate;
}

void Paint::setRotate( Rotate rotate )
{
	this->m_rotate = rotate;
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void Paint::drawPixel( int x, int y, bool colored )
{
	int point_temp;
	if( this->m_rotate == Rotate::Rotate0 )
	{
		if( x < 0 || x >= this->m_width || y < 0 || y >= this->m_height )
		{
			return;
		}
		drawAbsolutePixel( x, y, colored );
	}
	else if( this->m_rotate == Rotate::Rotate90 )
	{
		if( x < 0 || x >= this->m_height || y < 0 || y >= this->m_width )
		{
			return;
		}
		point_temp = x;
		x		   = this->m_width - y;
		y		   = point_temp;
		drawAbsolutePixel( x, y, colored );
	}
	else if( this->m_rotate == Rotate::Rotate180 )
	{
		if( x < 0 || x >= this->m_width || y < 0 || y >= this->m_height )
		{
			return;
		}
		x = this->m_width - x;
		y = this->m_height - y;
		drawAbsolutePixel( x, y, colored );
	}
	else if( this->m_rotate == Rotate::Rotate270 )
	{
		if( x < 0 || x >= this->m_height || y < 0 || y >= this->m_width )
		{
			return;
		}
		point_temp = x;
		x		   = y;
		y		   = this->m_height - point_temp;
		drawAbsolutePixel( x, y, colored );
	}
}

uint8_t* Paint::rawImage() const
{
	return ( uint8_t* )m_image.get();
}

bool Paint::merge( unsigned offsetX, unsigned offsetY, const Paint& painter )
{
	for( unsigned x = 0; x < this->m_width - offsetX; x++ )
		for( unsigned y = 0; y < this->m_height - offsetY; y++ )
			drawAbsolutePixel( offsetX + x, offsetY + y, painter.getAbsolutePixel( x, y ) );

	return true;
}

/**
 *  @brief: this draws a line on the frame buffer
 */
void Paint::drawLine( int x0, int y0, int x1, int y1, bool colored )
{
	/* Bresenham algorithm */
	int dx  = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
	int sx  = x0 < x1 ? 1 : -1;
	int dy  = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
	int sy  = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	while( ( x0 != x1 ) && ( y0 != y1 ) )
	{
		drawPixel( x0, y0, colored );
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
void Paint::drawHorizontalLine( int x, int y, int line_width, bool colored )
{
	int i;
	for( i = x; i < x + line_width; i++ )
	{
		drawPixel( i, y, colored );
	}
}

/**
 *  @brief: this draws a vertical line on the frame buffer
 */
void Paint::drawVerticalLine( int x, int y, int line_height, bool colored )
{
	int i;
	for( i = y; i < y + line_height; i++ )
	{
		drawPixel( x, i, colored );
	}
}

/**
 *  @brief: this draws a rectangle
 */
void Paint::drawRectangle( int x0, int y0, int x1, int y1, bool colored )
{
	int min_x, min_y, max_x, max_y;
	min_x = x1 > x0 ? x0 : x1;
	max_x = x1 > x0 ? x1 : x0;
	min_y = y1 > y0 ? y0 : y1;
	max_y = y1 > y0 ? y1 : y0;

	drawHorizontalLine( min_x, min_y, max_x - min_x + 1, colored );
	drawHorizontalLine( min_x, max_y, max_x - min_x + 1, colored );
	drawVerticalLine( min_x, min_y, max_y - min_y + 1, colored );
	drawVerticalLine( max_x, min_y, max_y - min_y + 1, colored );
}

/**
 *  @brief: this draws a filled rectangle
 */
void Paint::drawFilledRectangle( int x0, int y0, int x1, int y1, bool colored )
{
	int min_x, min_y, max_x, max_y;
	int i;
	min_x = x1 > x0 ? x0 : x1;
	max_x = x1 > x0 ? x1 : x0;
	min_y = y1 > y0 ? y0 : y1;
	max_y = y1 > y0 ? y1 : y0;

	for( i = min_x; i <= max_x; i++ )
	{
		drawVerticalLine( i, min_y, max_y - min_y + 1, colored );
	}
}

/**
 *  @brief: this draws a circle
 */
void Paint::drawCircle( int x, int y, int radius, bool colored )
{
	/* Bresenham algorithm */
	int x_pos = -radius;
	int y_pos = 0;
	int err   = 2 - 2 * radius;
	int e2;

	do
	{
		drawPixel( x - x_pos, y + y_pos, colored );
		drawPixel( x + x_pos, y + y_pos, colored );
		drawPixel( x + x_pos, y - y_pos, colored );
		drawPixel( x - x_pos, y - y_pos, colored );
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
void Paint::drawFilledCircle( int x, int y, int radius, bool colored )
{
	/* Bresenham algorithm */
	int x_pos = -radius;
	int y_pos = 0;
	int err   = 2 - 2 * radius;
	int e2;

	do
	{
		drawPixel( x - x_pos, y + y_pos, colored );
		drawPixel( x + x_pos, y + y_pos, colored );
		drawPixel( x + x_pos, y - y_pos, colored );
		drawPixel( x - x_pos, y - y_pos, colored );
		drawHorizontalLine( x + x_pos, y + y_pos, 2 * ( -x_pos ) + 1, colored );
		drawHorizontalLine( x + x_pos, y - y_pos, 2 * ( -x_pos ) + 1, colored );
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

/* END OF FILE */
