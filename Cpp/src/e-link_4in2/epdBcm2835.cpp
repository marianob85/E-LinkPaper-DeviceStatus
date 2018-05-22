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
#include <bcm2835.h>

// start: ------------------- EpdBcm2835 -------------------

EpdBcm2835::EpdBcm2835( uint8_t channel ) : Epd( channel ) {}

void EpdBcm2835::digitalWrite( uint8_t pin, uint8_t value )
{
	bcm2835_gpio_write( pin, value );
}

uint8_t EpdBcm2835::digitalRead( uint8_t pin )
{
	return bcm2835_gpio_lev( pin );
}

void EpdBcm2835::delayMs( unsigned int delaytime )
{
	bcm2835_delay( delaytime );
}

void EpdBcm2835::spiTransfer( uint8_t data )
{
	bcm2835_spi_transfern( ( char* )&data, 1 );
}

bool EpdBcm2835::init( uint8_t rstPin, uint8_t dcPin, uint8_t busyPin )
{
	if( !bcm2835_init() )
	{
		return false;
	}
	bcm2835_gpio_fsel( rstPin, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_fsel( dcPin, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_fsel( busyPin, BCM2835_GPIO_FSEL_INPT );

	bcm2835_spi_begin(); // Start spi interface, set spi pin for the reuse function
	bcm2835_spi_setBitOrder( BCM2835_SPI_BIT_ORDER_MSBFIRST );	// High first transmission
	bcm2835_spi_setDataMode( BCM2835_SPI_MODE0 );				  // spi mode 0
	bcm2835_spi_setClockDivider( BCM2835_SPI_CLOCK_DIVIDER_128 ); // Frequency
	bcm2835_spi_chipSelect( m_channel );						  // set CE0
	bcm2835_spi_setChipSelectPolarity( m_channel, LOW );		  // enable cs0
	return true;
}

// end: --------------------- EpdBcm2835 -------------------
