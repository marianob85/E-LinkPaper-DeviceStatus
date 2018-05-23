#include <signal.h>
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
#include "StatusManager.hpp"
#include "StatusPing.hpp"
#include <DS18B20.h>

using namespace std;

void sig_handler( int sig )
{
	switch( sig )
	{
	case SIGTERM:
	case SIGABRT:
	case SIGINT:
		exit( 0 );
		break;
	default:
		fprintf( stderr, "wasn't expecting that!\n" );
		abort();
	}
}

int main( void )
{
	signal( SIGABRT, sig_handler );
	signal( SIGTERM, sig_handler );
	signal( SIGINT, sig_handler );

	DS18B20Mgr ds18B20Mgr( BUS );
	auto sensor = ds18B20Mgr[ 0 ];

	while( true )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
		auto temp = sensor.getTemp();
		temp = temp;
	}
	return 0;

	StatusManager statusManager;

	if( !statusManager.init() )
		return -1;

	statusManager.add( make_unique< StatusPing >(), 60 );

	statusManager.setPage( 0, 0 );
	statusManager.autoChange( true );

	while( true )
		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

	return 0;
}
