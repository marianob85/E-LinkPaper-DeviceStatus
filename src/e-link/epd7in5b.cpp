#include "epd7in5b.hpp"

#define LOW 0
#define HIGH 1

// EPD7IN5B commands
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
#define IMAGE_PROCESS 0x13
#define LUT_FOR_VCOM 0x20
#define LUT_BLUE 0x21
#define LUT_WHITE 0x22
#define LUT_GRAY_1 0x23
#define LUT_GRAY_2 0x24
#define LUT_RED_0 0x25
#define LUT_RED_1 0x26
#define LUT_RED_2 0x27
#define LUT_RED_3 0x28
#define LUT_XON 0x29
#define PLL_CONTROL 0x30
#define TEMPERATURE_SENSOR_COMMAND 0x40
#define TEMPERATURE_CALIBRATION 0x41
#define TEMPERATURE_SENSOR_WRITE 0x42
#define TEMPERATURE_SENSOR_READ 0x43
#define VCOM_AND_DATA_INTERVAL_SETTING 0x50
#define LOW_POWER_DETECTION 0x51
#define TCON_SETTING 0x60
#define TCON_RESOLUTION 0x61
#define SPI_FLASH_CONTROL 0x65
#define REVISION 0x70
#define GET_STATUS 0x71
#define AUTO_MEASUREMENT_VCOM 0x80
#define READ_VCOM_VALUE 0x81
#define VCM_DC_SETTING 0x82

Epd7in5b::Epd7in5b( std::unique_ptr< EpdInterface > epd,
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

Epd7in5b::~Epd7in5b() {}

bool Epd7in5b::init( void )
{
	reset();
	sendCommand( POWER_SETTING );
	sendData( 0x37 );
	sendData( 0x00 );
	sendCommand( PANEL_SETTING );
	sendData( 0xCF );
	sendData( 0x08 );
	sendCommand( BOOSTER_SOFT_START );
	sendData( 0xC7 );
	sendData( 0xCC );
	sendData( 0x28 );
	sendCommand( PLL_CONTROL );
	sendData( 0x3A );
	sendCommand( TEMPERATURE_CALIBRATION );
	sendData( 0x00 );
	sendCommand( VCOM_AND_DATA_INTERVAL_SETTING );
	sendData( 0x77 );
	sendCommand( TCON_SETTING );
	sendData( 0x22 );
	sendCommand( TCON_RESOLUTION );
	sendData( 0x02 ); // source 640
	sendData( 0x80 );
	sendData( 0x01 ); // gate 384
	sendData( 0x80 );
	sendCommand( VCM_DC_SETTING );
	sendData( 0x1E );	// decide by LUT file
	sendCommand( 0xE5 ); // FLASH MODE
	sendData( 0x03 );
	sendCommand( POWER_ON );

	return true;
}

void Epd7in5b::waitUntilIdle( void )
{
	while( m_epd->digitalRead( m_busyPin ) == 0 )
	{ // 0: busy, 1: idle
		m_epd->delayMs( 100 );
	}
}

void Epd7in5b::reset( void )
{
	m_epd->digitalWrite( m_resetPin, LOW ); // module reset
	m_epd->delayMs( 200 );
	m_epd->digitalWrite( m_resetPin, HIGH );
	m_epd->delayMs( 200 );
}

void Epd7in5b::setLut( void ) {}

void Epd7in5b::displayFrame( const uint8_t* frame_buffer )
{
	waitUntilIdle();
	sendCommand( DATA_START_TRANSMISSION_1 );
	for( size_t i = 0; i < m_width / 2 * m_height; i++ )
	{
		sendData( frame_buffer[ i ] );
	}
	sendCommand( DISPLAY_REFRESH );
	m_epd->delayMs( 100 );
}

void Epd7in5b::sleep( void )
{
	waitUntilIdle();
	sendCommand( POWER_OFF );
	waitUntilIdle();
	sendCommand( DEEP_SLEEP );
	sendData( 0xa5 );
}
