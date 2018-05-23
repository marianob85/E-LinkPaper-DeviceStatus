#pragma once

#include <thread>

class TimerEvent
{
public:
	TimerEvent() {}

	TimerEvent( int after, bool async, std::function< void() > func )
	{
		// std::function< typename std::result_of< callable( arguments... ) >::type() > task(
		//	std::bind( std::forward< callable >( f ), std::forward< arguments >( args )... ) );

		if( async )
		{
			std::thread( [after, func]() {
				std::this_thread::sleep_for( std::chrono::milliseconds( after ) );
				func();
			} )
				.detach();
		}
		else
		{
			std::this_thread::sleep_for( std::chrono::milliseconds( after ) );
			func();
		}
	}
};