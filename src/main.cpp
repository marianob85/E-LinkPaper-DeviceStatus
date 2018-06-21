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

#define SI7021_MEASRH_HOLD_CMD 0xE5
#define SI7021_MEASRH_NOHOLD_CMD 0xF5
#define SI7021_MEASTEMP_HOLD_CMD 0xE3
#define SI7021_MEASTEMP_NOHOLD_CMD 0xF3
#define SI7021_READPREVTEMP_CMD 0xE0
#define SI7021_RESET_CMD 0xFE
#define SI7021_WRITERHT_REG_CMD 0xE6
#define SI7021_READRHT_REG_CMD 0xE7
#define SI7021_WRITEHEATER_REG_CMD 0x51
#define SI7021_READHEATER_REG_CMD 0x11
#define SI7021_ID1_CMD 0xFA0F
#define SI7021_ID2_CMD 0xFCC9
#define SI7021_FIRMVERS_CMD 0x84B8

void test()
{
	auto fd  = wiringPiI2CSetup( 0x40 );
	auto ret = wiringPiI2CWrite( fd, SI7021_RESET_CMD );
	//ret		 = wiringPiI2CWrite( fd, SI7021_MEASTEMP_HOLD_CMD );
	//std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
	//unsigned int a = wiringPiI2CRead( fd );
	//std::cout << a << endl;
	//unsigned int b = wiringPiI2CRead( fd );
	//std::cout << b << endl;
	//unsigned int c = wiringPiI2CRead( fd );
	//std::cout << c << endl;

	auto temp = wiringPiI2CReadReg8( fd, SI7021_MEASTEMP_HOLD_CMD );

	//unsigned int temp = a;
	//temp <<= 8;
	//temp |= b;

	float temperature = temp;
	temperature *= 175.72;
	temperature /= 65536;
	temperature -= 46.85;
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
