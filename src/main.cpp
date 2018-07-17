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
#include "StatusLed.hpp"
#include <influxdb.hpp>

#include <wiringPi.h>

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
	// influxdb_cpp::server_info si( "192.168.1.110", 8086, "telegraf", "grafana", "Gr@fana123456789" );
	// string resp;
	// influxdb_cpp::query( resp, R"(SELECT mean("value") FROM "Environment" WHERE ("host" = 'Ogrodowa-NanoPINeo2' AND
	// "id" = '28-0117b35349ff') AND time > now() - 1h)", si );

	// return 0;
	// wiringPiSetupPhys();
	// pinMode( 7, OUTPUT );
	// digitalWrite( 7, LOW ); // On
	// return 0;
	//cout << "started" << endl;

	//SI7021 sI7021;

	//auto humF = [&]() {
	//	while( 1 )
	//	{
	//		auto hum = sI7021.gethumidity();

	//	if( hum.second == false )
	//			cout << "Error hum" << endl;
	//	}
	//};

	//std::thread tempT = thread( humF );

	//while( 1 )
	//{
	//	auto temp = sI7021.getTemp();

	//	if( temp.second == false )
	//		cout << "Error temp" << endl;
	//}

	//return 0;

	path xmlPath( "/usr/local/etc/E-LinkStatusConfig.xml" );

	if( argc > 1 )
		xmlPath = argv[ 1 ];

	StatusManager statusManager( xmlPath );

	LedStatus ledStatus( xmlPath );

	if( !statusManager.init() )
		return -1;

	statusManager.add( make_unique< StatusPing >( xmlPath ) );
	statusManager.setPage( 0, 0 );

	signal( SIGTERM, sig_handler );
	signal( SIGINT, sig_handler );

	std::unique_lock< std::mutex > lk( m );
	cv.wait( lk );

	std::cout << "close" << endl;
	statusManager.close();

	return 0;
}
