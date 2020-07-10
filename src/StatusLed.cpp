#include <iostream>
#include <wiringPi.h>
#include <pugixml.hpp>
#include <functional>
#include "StatusLed.hpp"

using namespace std;

LedStatus::LedStatus( std::experimental::filesystem::path xmlPath )
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( xmlPath.string().c_str() );

	if( !result )
	{
		std::cout << "XML [" << xmlPath.string().c_str() << "] parsed with errors, attr value: ["
				  << doc.child( "node" ).attribute( "attr" ).value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..."
				  << ( xmlPath.string().c_str() + result.offset ) << "]\n\n";
		throw std::exception();
	}
	auto config  = doc.child( "Config" );
	auto ledNode = config.child( "LedStatus" ).child( "Pin" );

	if( !ledNode.empty() )
	{
		m_gpioPin = static_cast< uint8_t >( ledNode.text().as_uint() );
		pinMode( m_gpioPin, OUTPUT );
		m_led = thread( std::bind( &LedStatus::ledWorker, this ) );
	}
}

LedStatus::~LedStatus()
{
	m_threadLoop = false;
	m_led.join();
	digitalWrite( m_gpioPin, LOW );
}

void LedStatus::ledWorker()
{
	while( m_threadLoop )
	{
		this_thread::sleep_for( 10s );
		digitalWrite( m_gpioPin, LOW );
		this_thread::sleep_for( 30ms );
		digitalWrite( m_gpioPin, HIGH );
	}
}
