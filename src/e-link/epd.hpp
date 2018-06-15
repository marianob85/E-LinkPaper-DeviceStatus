#pragma once
#include <memory>
#include "epdInterface.hpp"

class Epd
{
public:
	Epd( std::unique_ptr< EpdInterface > epd,
		 uint8_t resetPin,
		 uint8_t dcPin,
		 uint8_t csPin,
		 uint8_t bussyPin,
		 size_t width,
		 size_t height );
	~Epd() = default;

	virtual bool init( void )								 = 0;
	virtual void waitUntilIdle( void )						 = 0;
	virtual void sleep( void )								 = 0;
	virtual void reset( void )								 = 0;
	virtual void setLut( void )								 = 0;
	virtual void displayFrame( const uint8_t* frame_buffer ) = 0;

	virtual void sendCommand( uint8_t command );
	virtual void sendData( uint8_t data );
	virtual size_t width() const;
	virtual size_t height() const;

protected:
	std::unique_ptr< EpdInterface > m_epd;
	const uint8_t m_resetPin{ 0 };
	const uint8_t m_dcPin{ 0 };
	const uint8_t m_busyPin{ 0 };
	const size_t m_width{ 0 };
	const size_t m_height{ 0 };
};
