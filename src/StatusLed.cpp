#include <iostream>
#include <bcm2835.h>
#include <pugixml.hpp>
#include "StatusLed.hpp"

using namespace std;

LedStatus::LedStatus( std::experimental::filesystem::path xmlPath )
{
	bcm2835_init();

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
		bcm2835_gpio_fsel( m_gpioPin, BCM2835_GPIO_FSEL_OUTP );
		bcm2835_gpio_pud( m_gpioPin );
		m_led = thread( std::bind( &LedStatus::ledWorker, this ) );
	}
}

LedStatus::~LedStatus() {

	m_threadLoop = false;
	m_led.join();
	bcm2835_gpio_write( m_gpioPin, 0 );
}

void LedStatus::ledWorker()
{
	while( m_threadLoop )
	{
		this_thread::sleep_for( 2s );
		bcm2835_gpio_write( m_gpioPin, 1 );
		this_thread::sleep_for( 50ms );
		bcm2835_gpio_write( m_gpioPin, 0 );
	}
}
