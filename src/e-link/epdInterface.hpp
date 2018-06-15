#pragma once
#include <stdint.h>

class EpdInterface
{
public:
	EpdInterface( uint8_t channel ) : m_channel( channel ) {}
	virtual ~EpdInterface() {}
	virtual bool init( uint8_t rstPin, uint8_t dcPin, uint8_t busyPin ) = 0;
	virtual void digitalWrite( uint8_t pin, uint8_t value )				= 0;
	virtual uint8_t digitalRead( uint8_t pin )							= 0;
	virtual void delayMs( unsigned int delaytime )						= 0;
	virtual void spiTransfer( uint8_t data )							= 0;

protected:
	uint8_t m_channel{ 0 };
};

