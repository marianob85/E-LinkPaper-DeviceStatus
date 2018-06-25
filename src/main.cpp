#include <signal.h>
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <experimental/filesystem>
#include "StatusManager.hpp"
#include "StatusPing.hpp"
#include <KS0108.hpp>
// start: ------------------- Test -------------------
#include <epdWiringPi.hpp>
#include <epd7in5b.hpp>
#include <wiringPiI2C.h>
#include <bcm2835.h>
// end: --------------------- Test -------------------

using namespace std;
using namespace std::experimental::filesystem;

std::mutex m;
std::condition_variable cv;

void sig_handler( int sig )
{
	switch( sig )
	{
	case SIGTERM:
	case SIGABRT:
	case SIGINT:
		std::cout << "SIG" << sig << endl;
		cv.notify_one();
		break;
	default:
		fprintf( stderr, "wasn't expecting that!\n" );
		abort();
	}
}
/*=========================================================================
	I2C ADDRESS/BITS
	-----------------------------------------------------------------------*/
#define SI7021_DEFAULT_ADDRESS ( 0x40 )

static const uint8_t SI7021_MEASRH_HOLD_CMD		= 0xE5;
static const uint8_t SI7021_MEASRH_NOHOLD_CMD   = 0xF5;
static const uint8_t SI7021_MEASTEMP_HOLD_CMD   = 0xE3;
static const uint8_t SI7021_MEASTEMP_NOHOLD_CMD = 0xF3;
static const uint8_t SI7021_READPREVTEMP_CMD	= 0xE0;
static const uint8_t SI7021_RESET_CMD			= 0xFE;
static const uint8_t SI7021_WRITERHT_REG_CMD	= 0xE6;
static const uint8_t SI7021_READRHT_REG_CMD		= 0xE7;
static const uint8_t SI7021_WRITEHEATER_REG_CMD = 0x51;
static const uint8_t SI7021_READHEATER_REG_CMD  = 0x11;
static const uint16_t SI7021_ID1_CMD			= 0xFA0F;
static const uint16_t SI7021_ID2_CMD			= 0xFCC9;
static const uint16_t SI7021_FIRMVERS_CMD		= 0x84B8;

uint16_t crc( uint8_t* data, size_t len )
{
	uint16_t crc = 0;
	for( size_t index = 0; index < len; ++index )
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

void test()
{

	if( !bcm2835_init() )
		return;

	bcm2835_i2c_set_baudrate( 10000 );

	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress( SI7021_DEFAULT_ADDRESS );

	// The I2C address of PCF8591 is 0x48.

	bcm2835_i2c_write( ( char* )&SI7021_RESET_CMD, 1 );
	bcm2835_i2c_write( ( char* )&SI7021_MEASTEMP_HOLD_CMD, 1 );
	std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
	uint8_t read[ 3 ];
	bcm2835_i2c_read( ( char* )read, 3 );

	bcm2835_i2c_end();

	unsigned int temp = read[ 0 ];
	temp <<= 8;
	temp |= read[ 1 ];

	// auto crcCalc = crc( read, 2 );

	float temperature = float( temp );
	temperature *= 175.72f;
	temperature /= 65536.0f;
	temperature -= 46.85f;
}

int main( int argc, char** argv )
{
	test();

	path xmlPath( "usr/local/etc/E-LinkStatusConfig.xml" );

	if( argc > 1 )
		xmlPath = argv[ 1 ];

	signal( SIGABRT, sig_handler );
	signal( SIGTERM, sig_handler );
	signal( SIGINT, sig_handler );

	StatusManager statusManager( xmlPath );

	if( !statusManager.init() )
		return -1;

	statusManager.add( make_unique< StatusPing >( xmlPath ) );
	statusManager.setPage( 0, 0 );

	std::unique_lock< std::mutex > lk( m );
	cv.wait( lk );

	std::cout << "close" << endl;
	statusManager.close();

	return 0;
}
