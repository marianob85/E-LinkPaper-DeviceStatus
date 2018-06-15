/**
 *  @filename   :   epd4in2.h
 *  @brief      :   Header file for e-paper library epd4in2.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
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
#include "epd.hpp"

class Epd4in2 : public Epd
{
public:
	Epd4in2( std::unique_ptr< EpdInterface > epd,
			 uint8_t resetPin,
			 uint8_t dcPin,
			 uint8_t csPin,
			 uint8_t bussyPin,
			 int width,
			 int height );
	~Epd4in2();

	// start: ------------------- Epd -------------------
	bool init( void ) override;
	void waitUntilIdle( void ) override;
	void reset( void ) override;
	void setLut( void ) override;
	void displayFrame( const uint8_t* frame_buffer ) override;
	void sleep( void ) override;
	// end: --------------------- Epd -------------------
};
