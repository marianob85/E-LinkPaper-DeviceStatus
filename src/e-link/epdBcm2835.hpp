#pragma once
#include <epdInterface.hpp>


class EpdBcm2835 : public EpdInterface
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