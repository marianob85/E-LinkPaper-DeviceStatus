#include "Task.hpp"

Task::Task( const std::chrono::milliseconds& timeOut ) : m_timeOut( timeOut ), m_futureObj( m_exitSignal.get_future() )
{
}

Task::Task( Task&& obj ) : m_exitSignal( std::move( obj.m_exitSignal ) ), m_futureObj( std::move( obj.m_futureObj ) ) {}
Task& Task::operator=( Task&& obj )
{
	m_exitSignal = std::move( obj.m_exitSignal );
	m_futureObj	 = std::move( obj.m_futureObj );
	return *this;
}
void Task::operator()()
{
	run();
}
bool Task::stopRequested()
{
	if( m_futureObj.wait_for( m_timeOut ) == std::future_status::timeout )
		return false;
	return true;
}
void Task::stop()
{
	m_exitSignal.set_value();
}