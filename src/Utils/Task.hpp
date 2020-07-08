#pragma once
#include <chrono>
#include <future>

class Task
{
public:
	Task( const std::chrono::milliseconds& timeOut );
	Task( Task&& obj );

	Task& operator=( Task&& obj );
	void operator()();

	bool stopRequested();
	void stop();

protected:
	virtual void run() = 0;

private:
	std::chrono::milliseconds m_timeOut;
	std::promise< void > m_exitSignal;
	std::future< void > m_futureObj;
};