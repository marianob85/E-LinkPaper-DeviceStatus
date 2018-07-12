#include "epd.hpp"

#define LOW 0
#define HIGH 1

// start: ------------------- Epd -------------------

Epd::Epd( std::unique_ptr< EpdInterface > epd,
		  uint8_t resetPin,
		  uint8_t dcPin,
		  uint8_t /*csPin*/,
		  uint8_t bussyPin,
		  size_t width,
		  size_t height )
	: m_epd( move( epd ) ),
	  m_resetPin( resetPin ),
	  m_dcPin( dcPin ),
	  m_busyPin( bussyPin ),
	  m_width( width ),
	  m_height( height )
{
}

void Epd::sendCommand( uint8_t command )
{
	m_epd->digitalWrite( m_dcPin, LOW );
	m_epd->spiTransfer( command );
}

void Epd::sendData( uint8_t data )
{
	m_epd->digitalWrite( m_dcPin, HIGH );
	m_epd->spiTransfer( data );
}

size_t Epd::width() const
{
	return m_width;
}

size_t Epd::height() const
{
	return m_height;
}

// end: --------------------- Epd -------------------
