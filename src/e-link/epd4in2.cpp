/**
 *  @filename   :   Epd4in24in2.cpp
 *  @brief      :   Implements for e-paper library
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
#include <stdlib.h>
#include "epd4in2.h"

#define LOW 0
#define HIGH 1

// EPD4IN2 commands
#define PANEL_SETTING 0x00
#define POWER_SETTING 0x01
#define POWER_OFF 0x02
#define POWER_OFF_SEQUENCE_SETTING 0x03
#define POWER_ON 0x04
#define POWER_ON_MEASURE 0x05
#define BOOSTER_SOFT_START 0x06
#define DEEP_SLEEP 0x07
#define DATA_START_TRANSMISSION_1 0x10
#define DATA_STOP 0x11
#define DISPLAY_REFRESH 0x12
#define DATA_START_TRANSMISSION_2 0x13
#define LUT_FOR_VCOM 0x20
#define LUT_WHITE_TO_WHITE 0x21
#define LUT_BLACK_TO_WHITE 0x22
#define LUT_WHITE_TO_BLACK 0x23
#define LUT_BLACK_TO_BLACK 0x24
#define PLL_CONTROL 0x30
#define TEMPERATURE_SENSOR_COMMAND 0x40
#define TEMPERATURE_SENSOR_SELECTION 0x41
#define TEMPERATURE_SENSOR_WRITE 0x42
#define TEMPERATURE_SENSOR_READ 0x43
#define VCOM_AND_DATA_INTERVAL_SETTING 0x50
#define LOW_POWER_DETECTION 0x51
#define TCON_SETTING 0x60
#define RESOLUTION_SETTING 0x61
#define GSST_SETTING 0x65
#define GET_STATUS 0x71
#define AUTO_MEASUREMENT_VCOM 0x80
#define READ_VCOM_VALUE 0x81
#define VCM_DC_SETTING 0x82
#define PARTIAL_WINDOW 0x90
#define PARTIAL_IN 0x91
#define PARTIAL_OUT 0x92
#define PROGRAM_MODE 0xA0
#define ACTIVE_PROGRAMMING 0xA1
#define READ_OTP 0xA2
#define POWER_SAVING 0xE3

using namespace std;

const unsigned char lut_vcom0[] = {
	0x00, 0x17, 0x00, 0x00, 0x00, 0x02, 0x00, 0x17, 0x17, 0x00, 0x00, 0x02, 0x00, 0x0A, 0x01,
	0x00, 0x00, 0x01, 0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};
const unsigned char lut_ww[] = {
	0x40, 0x17, 0x00, 0x00, 0x00, 0x02, 0x90, 0x17, 0x17, 0x00, 0x00, 0x02, 0x40, 0x0A,
	0x01, 0x00, 0x00, 0x01, 0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};
const unsigned char lut_bw[] = {
	0x40, 0x17, 0x00, 0x00, 0x00, 0x02, 0x90, 0x17, 0x17, 0x00, 0x00, 0x02, 0x40, 0x0A,
	0x01, 0x00, 0x00, 0x01, 0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};

const unsigned char lut_bb[] = {
	0x80, 0x17, 0x00, 0x00, 0x00, 0x02, 0x90, 0x17, 0x17, 0x00, 0x00, 0x02, 0x80, 0x0A,
	0x01, 0x00, 0x00, 0x01, 0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};

const unsigned char lut_wb[] = {
	0x80, 0x17, 0x00, 0x00, 0x00, 0x02, 0x90, 0x17, 0x17, 0x00, 0x00, 0x02, 0x80, 0x0A,
	0x01, 0x00, 0x00, 0x01, 0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};

Epd4in2::~Epd4in2(){};

Epd4in2::Epd4in2( unique_ptr< EpdInterface > epd,
				  uint8_t resetPin,
				  uint8_t dcPin,
				  uint8_t csPin,
				  uint8_t bussyPin,
				  int width,
				  int height )
	: Epd( move( epd ), resetPin, dcPin, csPin, bussyPin, width, height )
{
	m_epd->init( m_resetPin, m_dcPin, m_busyPin );
}

bool Epd4in2::init( void )
{
	waitUntilIdle();
	reset();
	sendCommand( POWER_SETTING );
	sendData( 0x03 ); // VDS_EN, VDG_EN
	sendData( 0x00 ); // VCOM_HV, VGHL_LV[1], VGHL_LV[0]
	sendData( 0x2b ); // VDH
	sendData( 0x2b ); // VDL
	sendData( 0xff ); // VDHR
	sendCommand( BOOSTER_SOFT_START );
	sendData( 0x17 );
	sendData( 0x17 );
	sendData( 0x17 ); // 07 0f 17 1f 27 2F 37 2f
	sendCommand( POWER_ON );
	waitUntilIdle();
	sendCommand( PANEL_SETTING );
	sendData( 0xbf ); // KW-BF   KWR-AF  BWROTP 0f
	sendData( 0x0b );
	sendCommand( PLL_CONTROL );
	sendData( 0x3c ); // 3A 100HZ   29 150Hz 39 200HZ  31 171HZ
	return true;
}

void Epd4in2::waitUntilIdle( void )
{
	m_epd->delayMs( 100 );
	sendCommand( GET_STATUS );
	while( m_epd->digitalRead( m_busyPin ) == 0 )
	{ // 0: busy, 1: idle
		m_epd->delayMs( 100 );
	}
}

void Epd4in2::reset( void )
{
	m_epd->digitalWrite( m_resetPin, LOW ); // module reset
	m_epd->delayMs( 200 );
	m_epd->digitalWrite( m_resetPin, HIGH );
	m_epd->delayMs( 200 );
}

void Epd4in2::setLut( void )
{
	size_t count;
	sendCommand( LUT_FOR_VCOM ); // vcom
	for( count = 0; count < 44; count++ )
	{
		sendData( lut_vcom0[ count ] );
	}

	sendCommand( LUT_WHITE_TO_WHITE ); // ww --
	for( count = 0; count < 42; count++ )
	{
		sendData( lut_ww[ count ] );
	}

	sendCommand( LUT_BLACK_TO_WHITE ); // bw r
	for( count = 0; count < 42; count++ )
	{
		sendData( lut_bw[ count ] );
	}

	sendCommand( LUT_WHITE_TO_BLACK ); // wb w
	for( count = 0; count < 42; count++ )
	{
		sendData( lut_bb[ count ] );
	}

	sendCommand( LUT_BLACK_TO_BLACK ); // bb b
	for( count = 0; count < 42; count++ )
	{
		sendData( lut_wb[ count ] );
	}
}

void Epd4in2::displayFrame( const uint8_t* frame_buffer )
{
	waitUntilIdle();
	sendCommand( RESOLUTION_SETTING );
	sendData( static_cast< unsigned char >( m_width >> 8 ) );
	sendData( static_cast< unsigned char >( m_width & 0xff ) );
	sendData( static_cast< unsigned char >( m_height >> 8 ) );
	sendData( static_cast< unsigned char >( m_height & 0xff ) );
	sendCommand( VCM_DC_SETTING );
	sendData( 0x12 );

	sendCommand( VCOM_AND_DATA_INTERVAL_SETTING );
	sendCommand( 0x97 ); // VBDF 17|D7 VBDW 97  VBDB 57  VBDF F7  VBDW 77  VBDB 37  VBDR B7

	if( frame_buffer != NULL )
	{
		sendCommand( DATA_START_TRANSMISSION_1 );
		for( size_t i = 0; i < m_width * m_height / 8; i++ )
		{
			sendData( 0xFF ); // bit set: white, bit reset: black
		}
		m_epd->delayMs( 2 );
		sendCommand( DATA_START_TRANSMISSION_2 );
		for( size_t i = 0; i < m_width * m_height / 8; i++ )
		{
			sendData( frame_buffer[ i ] );
		}
		m_epd->delayMs( 2 );
	}

	setLut();

	sendCommand( DISPLAY_REFRESH );
	m_epd->delayMs( 100 );
}

/**
 *  @brief: After this command is transmitted, the chip would enter the
 *          deep-sleep mode to save power.
 *          The deep sleep mode would return to standby by hardware reset.
 *          The only one parameter is a check code, the command would be
 *          executed if check code = 0xA5.
 *          You can use Epd4in2::Reset() to awaken or Epd4in2::Init() to initialize
 */
void Epd4in2::sleep()
{
	waitUntilIdle();
	sendCommand( VCOM_AND_DATA_INTERVAL_SETTING );
	sendData( 0x17 );			   // border floating
	sendCommand( VCM_DC_SETTING ); // VCOM to 0V
	sendCommand( PANEL_SETTING );
	m_epd->delayMs( 100 );

	sendCommand( POWER_SETTING ); // VG&VS to 0V fast
	sendData( 0x00 );
	sendData( 0x00 );
	sendData( 0x00 );
	sendData( 0x00 );
	sendData( 0x00 );
	m_epd->delayMs( 100 );

	sendCommand( POWER_OFF ); // power off
	waitUntilIdle();
	sendCommand( DEEP_SLEEP ); // deep sleep
	sendData( 0xA5 );
}
