/**
 *  @filename   :   epdpaint.cpp
 *  @brief      :   Paint tools
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <cstring>
#include "epdpaint.h"

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
void Paint::clear( int colored )
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
void Paint::drawAbsolutePixel( int x, int y, int colored )
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
void Paint::drawPixel( int x, int y, int colored )
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

/**
 *  @brief: this draws a charactor on the frame buffer but not refresh
 */
void Paint::drawCharAt( int x, int y, char ascii_char, const sFONT* font, int colored )
{
	int i, j;
	unsigned int char_offset = ( ascii_char - ' ' ) * font->Height * ( font->Width / 8 + ( font->Width % 8 ? 1 : 0 ) );
	const unsigned char* ptr = &font->table[ char_offset ];

	for( j = 0; j < font->Height; j++ )
	{
		for( i = 0; i < font->Width; i++ )
		{
			if( *ptr & ( 0x80 >> ( i % 8 ) ) )
			{
				drawPixel( x + i, y + j, colored );
			}
			if( i % 8 == 7 )
			{
				ptr++;
			}
		}
		if( font->Width % 8 != 0 )
		{
			ptr++;
		}
	}
}

/**
 *  @brief: this displays a string on the frame buffer but not refresh
 */
void Paint::drawStringAt( int x, int y, const char* text, const sFONT* font, int colored )
{
	int refcolumn = x;

	/* Send the string character by character on EPD */
	while( *text != 0 )
	{
		/* Display one character on EPD */
		drawCharAt( refcolumn, y, *text, font, colored );
		/* Decrement the column position by 16 */
		refcolumn += font->Width;
		/* Point on the next character */
		text++;
	}
}

std::pair< unsigned, unsigned > Paint::getStringSize( const char* text, const sFONT* font ) const
{
	unsigned refcolumn = 0;

	/* Send the string character by character on EPD */
	while( *text++ != 0 )
	{
		/* Decrement the column position by 16 */
		refcolumn += font->Width;
		/* Point on the next character */
	}

	// font->Height
	return { refcolumn, font->Height };
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
void Paint::drawLine( int x0, int y0, int x1, int y1, int colored )
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
void Paint::drawHorizontalLine( int x, int y, int line_width, int colored )
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
void Paint::drawVerticalLine( int x, int y, int line_height, int colored )
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
void Paint::drawRectangle( int x0, int y0, int x1, int y1, int colored )
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
void Paint::drawFilledRectangle( int x0, int y0, int x1, int y1, int colored )
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
void Paint::drawCircle( int x, int y, int radius, int colored )
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
void Paint::drawFilledCircle( int x, int y, int radius, int colored )
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
