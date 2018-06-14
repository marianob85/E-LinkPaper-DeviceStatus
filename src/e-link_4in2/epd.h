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
