#include "gpioPooling.hpp"
#include <functional>
#include <wiringPi.h>

using namespace std::chrono_literals;

GpioPooling::GpioPooling() : Task( 100ms )
{
	m_poolingThread = std::thread( [&]() { run(); } );
}

GpioPooling::~GpioPooling()
{
	stop();
	if( m_poolingThread.joinable() )
		m_poolingThread.join();
}

bool GpioPooling::registerEvent( int pin, ButtonEvent event )
{
	std::lock_guard< std::mutex > lock( m_refreshMutex );
	if( m_registeredEvents.find( pin ) == m_registeredEvents.end() )
	{
		pinMode( pin, INPUT );
		pullUpDnControl( pin, PUD_UP );
		m_pinState[ pin ] = true; // Because of PullUp
	}
	m_registeredEvents[ pin ].push_back( event );

	return true;
}

void GpioPooling::run()
{
	while( stopRequested() == false )
	{
		std::lock_guard< std::mutex > lock( m_refreshMutex );
		std::vector< std::function< void() > > calbInvoke;
		for( auto& event : m_registeredEvents )
		{
			if( const auto pinState = digitalRead( event.first ); pinState != m_pinState.at( event.first ) )
			{
				m_pinState[ event.first ] = pinState;
				calbInvoke.push_back( [&, pinState]() {
					for( const auto ev : event.second )
						ev( event.first, !pinState );
				} );
			}
		}
		for( auto& event : calbInvoke )
			event();
	}
}