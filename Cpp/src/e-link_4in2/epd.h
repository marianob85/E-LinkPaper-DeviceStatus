/**
 *  @filename   :   epdif.h
 *  @brief      :   Header file of epdif.cpp providing EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
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

#include <stdint.h>

// Pin level definition
#define LOW 0
#define HIGH 1

class Epd
{
public:
	Epd( uint8_t channel ) : m_channel( channel ) {}
	virtual ~Epd() {}
	virtual bool init( uint8_t rstPin, uint8_t dcPin, uint8_t busyPin ) = 0;
	virtual void digitalWrite( uint8_t pin, uint8_t value )				= 0;
	virtual uint8_t digitalRead( uint8_t pin )							= 0;
	virtual void delayMs( unsigned int delaytime )						= 0;
	virtual void spiTransfer( uint8_t data )							= 0;

protected:
	uint8_t m_channel{ 0 };
};

class EpdBcm2835 : public Epd
{
public:
	EpdBcm2835( uint8_t channel );
	~EpdBcm2835() = default;

	bool init( uint8_t rstPin, uint8_t dcPin, uint8_t busyPin ) override;
	void digitalWrite( uint8_t pin, uint8_t value ) override;
	uint8_t digitalRead( uint8_t pin ) override;
	void delayMs( unsigned int delaytime ) override;
	void spiTransfer( uint8_t data ) override;
};

class EpdWiringPi : public Epd
{
public:
	EpdWiringPi( uint8_t channel );
	~EpdWiringPi() = default;

	bool init( uint8_t rstPin, uint8_t dcPin, uint8_t busyPin ) override;
	void digitalWrite( uint8_t pin, uint8_t value ) override;
	uint8_t digitalRead( uint8_t pin ) override;
	void delayMs( unsigned int delaytime ) override;
	void spiTransfer( uint8_t data ) override;
};
