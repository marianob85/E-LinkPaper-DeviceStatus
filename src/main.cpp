#include <signal.h>
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <iomanip>
#include <wiringPi.h>
#include <experimental/filesystem>
#include "StatusManager.hpp"
#include "StatusPing.hpp"
#include <KS0108.hpp>
#include "StatusLed.hpp"
#include "InfluxWriter.hpp"

using namespace std;
using namespace std::experimental::filesystem;

std::mutex m;
std::condition_variable cv;

void sig_handler( int sig )
{
	switch( sig )
	{
	case SIGTERM:
	case SIGINT:
		std::cout << "SIG" << sig << endl;
		cv.notify_one();
		break;
	case SIGABRT:
	default:
		fprintf( stderr, "wasn't expecting that!\n" );
		abort();
	}
}

int main( int argc, char** argv )
{
	signal( SIGTERM, sig_handler );
	signal( SIGINT, sig_handler );

	path xmlPath( "/usr/etc/E-LinkStatusConfig.xml" );

	// gpio
	wiringPiSetupPhys();

	/*
	pinMode( 18, INPUT );
	pullUpDnControl(18,PUD_UP );
	while(1)
	{
		cout << digitalRead(18) << endl;
		std::this_thread::sleep_for(100ms);
	}
	*/

	if( argc > 1 )
		xmlPath = argv[ 1 ];
	LedStatus ledStatus( xmlPath );
	InfluxWriter influxWriter( xmlPath );
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
