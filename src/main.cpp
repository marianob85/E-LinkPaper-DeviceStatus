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

int main( int argc, char** argv )
{
	path xmlPath( "usr/local/etc/E-LinkStatusConfig.xml" );

	if( argc > 1 )
		xmlPath = argv[ 1 ];

	signal( SIGABRT, sig_handler );
	signal( SIGTERM, sig_handler );
	signal( SIGINT, sig_handler );

	// start: ------------------- Test -------------------

	auto epd = make_unique< Epd7in5b >( make_unique< EpdWiringPi >( 0 ), 13, 26, 7, 6, 640, 385 );
	epd->init();

	auto painter = make_unique< Paint3Colors >( epd->width(), epd->height() );

	painter->clear( Color::White );

	auto font = painter->createFonter< FontPainterKS0108 >( liberationMono12 );
	font->drawString( 10, 200, "Hello world !!!", Color::Color1 );
	//painter->drawFilledCircle( 100, 100, 100, Color::Color1 );

	epd->displayFrame( *painter );

	return 0;
	// end: --------------------- Test -------------------

	StatusManager statusManager( xmlPath );

	if( !statusManager.init() )
		return -1;

	statusManager.add( make_unique< StatusPing >( xmlPath ), 60 );

	statusManager.setPage( 0, 0 );
	statusManager.autoChange( true );

	std::unique_lock< std::mutex > lk( m );
	cv.wait( lk );

	std::cout << "close" << endl;
	statusManager.close();

	return 0;
}
