/**
 *  @filename   :   epdpaint.h
 *  @brief      :   Header file for epdpaint.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 28 2017
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
#pragma once
#include <memory>
#include <PainterFont.hpp>
#include <FontDef.hpp>

// Display orientation
enum class Rotate
{
	Rotate0,
	Rotate90,
	Rotate180,
	Rotate270,
};

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR 0

class Paint
{
	friend class FontPainter;

public:
	Paint( std::unique_ptr< uint8_t[] > image, int width, int height );
	Paint( Paint& paint );
	Paint( Paint&& ) = default;
	~Paint();

	Paint& operator=( const Paint& right );

	void clear( bool colored );
	int getWidth( void );
	int getHeight( void );
	Rotate getRotate( void );
	void setRotate( Rotate rotate );
	void drawAbsolutePixel( int x, int y, bool colored );
	bool getAbsolutePixel( unsigned x, unsigned y ) const;
	void drawPixel( int x, int y, bool colored );
	void drawLine( int x0, int y0, int x1, int y1, bool colored );
	void drawHorizontalLine( int x, int y, int width, bool colored );
	void drawVerticalLine( int x, int y, int height, bool colored );
	void drawRectangle( int x0, int y0, int x1, int y1, bool colored );
	void drawFilledRectangle( int x0, int y0, int x1, int y1, bool colored );
	void drawCircle( int x, int y, int radius, bool colored );
	void drawFilledCircle( int x, int y, int radius, bool colored );
	bool merge( unsigned offsetX, unsigned offsetY, const Paint& painter );

	uint8_t* rawImage() const;

	template< class Fonter >
	std::shared_ptr< FontPainter > createFonter( FontData fontData )
	{
		return std::make_shared< Fonter >(
			fontData, std::bind( &Paint::drawPixel, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	}

private:
	std::unique_ptr< uint8_t[] > m_image;
	int m_width{ 0 };
	int m_height{ 0 };
	Rotate m_rotate{ Rotate::Rotate0 };
};
