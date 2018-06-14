#pragma once

#include <thread>

class TimerEvent
{
public:
	TimerEvent() {}

	TimerEvent( int after, bool async, std::function< void() > func )
	{
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