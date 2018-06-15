#pragma once

#include <memory>
#include "epd.hpp"

class Epd7in5b : public Epd
{
public:
	Epd7in5b( std::unique_ptr< EpdInterface > epd,
			  uint8_t resetPin,
			  uint8_t dcPin,
			  uint8_t csPin,
			  uint8_t bussyPin,
			  int width,
			  int height );
	~Epd7in5b();

	// start: ------------------- Epd -------------------
	bool init( void ) override;
	void waitUntilIdle( void ) override;
	void reset( void ) override;
	void setLut( void ) override;
	void displayFrame( const uint8_t* frame_buffer ) override;
	void sleep( void ) override;
	// end: --------------------- Epd -------------------
};
