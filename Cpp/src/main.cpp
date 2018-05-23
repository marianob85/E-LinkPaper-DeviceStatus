#include <signal.h>
#include <memory>
#include <chrono>
#include <thread>
#include "StatusManager.hpp"
#include "StatusPing.hpp"

using namespace std;

void sig_handler( int sig )
{
	switch( sig )
	{
	case SIGTERM:
	case SIGABRT:
		fprintf( stderr, "give out a backtrace or something...\n" );
		abort();
	default:
		fprintf( stderr, "wasn't expecting that!\n" );
		abort();
	}
}

int main( void )
{
	signal( SIGABRT, sig_handler );
	signal( SIGTERM, sig_handler );

	StatusManager statusManager;

	if( !statusManager.init() )
		return -1;

	statusManager.add( make_unique< StatusPing >(), 10 );

	statusManager.setPage( 0, 0 );
	statusManager.autoChange( true );

	while( true )
		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

	return 0;
}
