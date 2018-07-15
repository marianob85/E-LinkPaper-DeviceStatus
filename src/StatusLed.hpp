#pragma once
#include <thread>
#include <experimental/filesystem>

class LedStatus
{
public:
	LedStatus( std::experimental::filesystem::path xmlPath );
	virtual ~LedStatus();

protected:
	void ledWorker();

protected:
	uint8_t m_gpioPin{ 0 };
	std::thread m_led;
	bool m_threadLoop{ true };
};