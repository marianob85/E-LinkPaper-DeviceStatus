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

#include "fonts.h"

class Paint
{
public:
	Paint( std::unique_ptr< uint8_t[] > image, int width, int height );
	Paint( Paint& paint );
	Paint( Paint&& ) = default;

	~Paint();

	Paint& operator=( const Paint& right );

	void clear( int colored );
	int getWidth( void );
	int getHeight( void );
	Rotate getRotate( void );
	void setRotate( Rotate rotate );
	void drawAbsolutePixel( int x, int y, int colored );
	bool getAbsolutePixel( unsigned x, unsigned y ) const;
	void drawPixel( int x, int y, int colored );
	void drawCharAt( int x, int y, char ascii_char, const sFONT* font, int colored );
	void drawStringAt( int x, int y, const char* text, const sFONT* font, int colored );
	void drawLine( int x0, int y0, int x1, int y1, int colored );
	void drawHorizontalLine( int x, int y, int width, int colored );
	void drawVerticalLine( int x, int y, int height, int colored );
	void drawRectangle( int x0, int y0, int x1, int y1, int colored );
	void drawFilledRectangle( int x0, int y0, int x1, int y1, int colored );
	void drawCircle( int x, int y, int radius, int colored );
	void drawFilledCircle( int x, int y, int radius, int colored );
	std::pair< unsigned, unsigned > getStringSize( const char* text, const sFONT* font ) const;
	uint8_t* rawImage() const;
	bool merge( unsigned offsetX, unsigned offsetY, const Paint& painter );

private:
	std::unique_ptr< uint8_t[] > m_image;
	int m_width{ 0 };
	int m_height{ 0 };
	Rotate m_rotate{ Rotate::Rotate0 };
};
