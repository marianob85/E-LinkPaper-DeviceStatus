#include <signal.h>
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <iomanip>
#include <experimental/filesystem>
#include "StatusManager.hpp"
#include "StatusPing.hpp"
#include <KS0108.hpp>

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
	//std::mutex refreshMutex;
	//

	//SI7021 st7021;

	//std::thread watcher = thread( [&]() {
	//	while( 1 )
	//	{
	//		auto hum7021 = st7021.gethumidity();

	//		std::lock_guard< std::mutex > lock( refreshMutex );
	//		cout << "Correct: " << hum7021.second << " Humidity: " << std::setprecision( 4 ) << hum7021.first << endl;
	//	}
	//} );

	//while( 1 )
	//{
	//	auto temp7021 = st7021.getTemp();

	//	std::lock_guard< std::mutex > lock( refreshMutex );
	//	cout << "Correct: " << temp7021.second << " Temp: " << std::setprecision( 4 ) << temp7021.first << endl;

	//	// auto hum7021 = st7021.gethumidity();
	//	// cout << "Correct: " << hum7021.second << " Humidity: " << std::setprecision( 2 ) << hum7021.first << endl;
	//}

	//return 0;

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
