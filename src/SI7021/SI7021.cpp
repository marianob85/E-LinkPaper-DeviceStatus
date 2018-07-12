#include <thread>
#include <algorithm>
#include <bcm2835.h>
#include "SI7021.hpp"

// start: ------------------- SI7021 -------------------

const uint8_t SI7021_DEFAULT_ADDRESS	 = 0x40;
const uint8_t SI7021_MEASRH_HOLD_CMD	 = 0xE5;
//const uint8_t SI7021_MEASRH_NOHOLD_CMD   = 0xF5;
const uint8_t SI7021_MEASTEMP_HOLD_CMD   = 0xE3;
//const uint8_t SI7021_MEASTEMP_NOHOLD_CMD = 0xF3;
//const uint8_t SI7021_READPREVTEMP_CMD	= 0xE0;
const uint8_t SI7021_RESET_CMD			 = 0xFE;
//const uint8_t SI7021_WRITERHT_REG_CMD	= 0xE6;
//const uint8_t SI7021_READRHT_REG_CMD	 = 0xE7;
//const uint8_t SI7021_WRITEHEATER_REG_CMD = 0x51;
//const uint8_t SI7021_READHEATER_REG_CMD  = 0x11;
//const uint16_t SI7021_ID1_CMD			 = 0xFA0F;
//const uint16_t SI7021_ID2_CMD			 = 0xFCC9;
//const uint16_t SI7021_FIRMVERS_CMD		 = 0x84B8;

std::mutex SI7021::m_refreshMutex;

SI7021::SI7021()
{
	bcm2835_init();
	bcm2835_i2c_set_baudrate( 10000 );

	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress( SI7021_DEFAULT_ADDRESS );
	bcm2835_i2c_write( ( char* )&SI7021_RESET_CMD, 1 );
	bcm2835_i2c_end();
}

std::pair< float, bool > SI7021::getTemp() const
{
	std::lock_guard< std::mutex > lock( m_refreshMutex );
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress( SI7021_DEFAULT_ADDRESS );
	auto writeCode = bcm2835_i2c_write( ( char* )&SI7021_MEASTEMP_HOLD_CMD, 1 );
	if( writeCode != BCM2835_I2C_REASON_OK )
	{
		bcm2835_i2c_end();
		return { 0, false };
	}
	std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
	uint8_t read[ 3 ];
	auto readCode = bcm2835_i2c_read( ( char* )read, 3 );

	if( readCode != BCM2835_I2C_REASON_OK )
	{
		bcm2835_i2c_end();
		return { 0, false };
	}

	bcm2835_i2c_end();

	unsigned int temp = read[ 0 ];
	temp <<= 8;
	temp |= read[ 1 ];

	if( crc( read, 2 ) != read[ 2 ] || read[ 2 ] == 0 )
		return { 0, false };

	float temperature = float( temp );
	temperature *= 175.72f;
	temperature /= 65536.0f;
	temperature -= 46.85f;

	return { temperature, true };
}

std::pair< float, bool > SI7021::gethumidity() const
{
	std::lock_guard< std::mutex > lock( m_refreshMutex );
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress( SI7021_DEFAULT_ADDRESS );
	auto writeCode = bcm2835_i2c_write( ( char* )&SI7021_MEASRH_HOLD_CMD, 1 );
	if( writeCode != BCM2835_I2C_REASON_OK )
	{
		bcm2835_i2c_end();
		return { 0, false };
	}
	std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
	uint8_t read[ 3 ];
	auto readCode = bcm2835_i2c_read( ( char* )read, 3 );

	if( readCode != BCM2835_I2C_REASON_OK )
	{
		bcm2835_i2c_end();
		return { 0, false };
	}

	bcm2835_i2c_end();

	unsigned int hum = read[ 0 ];
	hum <<= 8;
	hum |= read[ 1 ];

	if( crc( read, 2 ) != read[ 2 ] || read[ 2 ] == 0 )
		return { 0, false };

	float humidity = float( hum );
	humidity *= 125.0f;
	humidity /= 65536.0f;
	humidity -= 6.0f;

	return { std::min( 100.0f, humidity ), true };
}

unsigned SI7021::crc( uint8_t* data, size_t len )
{
	unsigned crc = 0;
	for( uint8_t index = 0; index < len; ++index )
	{
		crc ^= *( data + index );
		for( uint8_t i = 0; i < 8; ++i )
		{
			if( crc & 0x80 )
			{
				crc <<= 1;
				crc ^= 0x131;
			}
			else
			{
				crc <<= 1;
			}
		}
	}
	return crc;
}

// end: --------------------- SI7021 -------------------
