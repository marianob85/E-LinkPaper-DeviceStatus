#pragma once
#include <functional>
#include <memory>
#include <thread>
#include <DS18B20.h>
#include <SI7021.hpp>

class EnvironmentDataProvider
{
public:
	EnvironmentDataProvider( std::function< void( float ) > callback );
	virtual ~EnvironmentDataProvider() = default;

	virtual float getData() const	= 0;
	virtual bool isAvailable() const = 0;

protected:
	std::function< void( float ) > m_callback;
	std::thread m_watcher;
	float m_lastTemp{ 0.f };
};

class TempProvider : public EnvironmentDataProvider
{
public:
	TempProvider( std::function< void( float ) > callback );

	// start: ------------------- EnvironmentDataProvider -------------------
	virtual float getData() const override;
	virtual bool isAvailable() const override;
	// end: --------------------- EnvironmentDataProvider -------------------
private:
	bool createDS18B20();
	bool createSI7021();

	void threadWatcherDS18B20();

private:
	std::unique_ptr< DS18B20 > m_DS18B20Sensor;
	std::unique_ptr< SI7021 > m_SI7021Sensor;
};