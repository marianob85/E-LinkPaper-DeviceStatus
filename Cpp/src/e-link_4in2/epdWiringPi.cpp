/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
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

#include "epd.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>

// start: ------------------- EpdWiringPi -------------------

EpdWiringPi::EpdWiringPi( uint8_t channel ) : Epd( channel ) {}

bool EpdWiringPi::init( uint8_t rstPin, uint8_t dcPin, uint8_t busyPin )
{
	if(::wiringPiSetupGpio() < 0 )
	{ // using Broadcom GPIO pin mapping
		return false;
	}
	::pinMode( rstPin, OUTPUT );
	::pinMode( dcPin, OUTPUT );
	::pinMode( busyPin, INPUT );
	::wiringPiSPISetup( m_channel, 2000000 );
	return true;
}

void EpdWiringPi::digitalWrite( uint8_t pin, uint8_t value )
{
	::digitalWrite( pin, value );
}

uint8_t EpdWiringPi::digitalRead( uint8_t pin )
{
	return static_cast< uint8_t >(::digitalRead( pin ) );
}

void EpdWiringPi::delayMs( unsigned int delaytime )
{
	::delay( delaytime );
}

void EpdWiringPi::spiTransfer( uint8_t data )
{
	::wiringPiSPIDataRW( m_channel, &data, 1 );
}

// end: --------------------- EpdWiringPi -------------------
