#pragma once
#include <thread>
#include <map>
#include <vector>
#include <Task.hpp>

class GpioPooling : private Task
{
public:
	using ButtonEvent = std::function< void( int, bool ) >;

public:
	GpioPooling();
	virtual ~GpioPooling();

	bool registerEvent( int pin, ButtonEvent event );

private:
	virtual void run() override;
	void poolingThread();

private:
	std::mutex m_refreshMutex;
	std::map< int, std::vector< ButtonEvent > > m_registeredEvents;
	std::map< int, bool > m_pinState;
	std::thread m_poolingThread;
};