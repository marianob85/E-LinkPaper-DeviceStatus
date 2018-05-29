#include <signal.h>
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
#include "StatusManager.hpp"
#include "StatusPing.hpp"
#include <KS0108.hpp>

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

void test()
{
	Epd4in2 epd( make_unique< EpdWiringPi >( CHANNEL ), RST_PIN, DC_PIN, CS_PIN, BUSY_PIN, EPD_WIDTH, EPD_HEIGHT );
	epd.init();

	std::unique_ptr< uint8_t[] > frameBuffer = make_unique< uint8_t[] >( epd.width() / 8 * epd.height() );
	auto painter							 = make_unique< Paint >( move( frameBuffer ), epd.width(), epd.height() );
	painter->clear( false );
	auto font12 = painter->createFonter< FontPainterKS0108 >( fontGeorgia12 );

	font12->drawChar( 10, 20, 'A', UNCOLORED );

	epd.displayFrame( painter->rawImage() );
}

int main( void )
{
	signal( SIGABRT, sig_handler );
	signal( SIGTERM, sig_handler );
	signal( SIGINT, sig_handler );

	test();
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
