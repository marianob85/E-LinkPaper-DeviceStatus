#include <signal.h>
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
#include <experimental/filesystem>
#include "StatusManager.hpp"
#include "StatusPing.hpp"
#include <KS0108.hpp>

using namespace std;
using namespace std::experimental::filesystem;

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

int main( int argc, char** argv )
{
	path xmlPath( argv[ 0 ] );

	xmlPath = xmlPath.parent_path();
	xmlPath /= "E-LinkStatusConfig.xml";

	if( argc > 1 )
		xmlPath = argv[ 1 ];

	signal( SIGABRT, sig_handler );
	signal( SIGTERM, sig_handler );
	signal( SIGINT, sig_handler );

	StatusManager statusManager( xmlPath );

	if( !statusManager.init() )
		return -1;

	statusManager.add( make_unique< StatusPing >( xmlPath ), 60 );

	statusManager.setPage( 0, 0 );
	statusManager.autoChange( true );

	while( true )
		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

	return 0;
}
