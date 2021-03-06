#pragma once
#include <functional>
#include <memory>
#include <thread>
#include <experimental/filesystem>
#include <DS18B20.h>
#include <SI7021.hpp>

class EnvironmentDataProvider
{
public:
	EnvironmentDataProvider( std::experimental::filesystem::path xmlPath, std::function< void( float ) > callback );
	virtual ~EnvironmentDataProvider() = default;

	virtual std::pair< float, bool > getData() const = 0;
	virtual bool isAvailable() const				 = 0;
	void reset();

protected:
	std::function< void( float ) > m_callback;
	std::thread m_watcher;
	float m_lastTemp{ 0.f };
	std::experimental::filesystem::path m_xmlPath;
};

class TempProvider : public EnvironmentDataProvider
{
public:
	TempProvider( std::experimental::filesystem::path xmlPath, std::function< void( float ) > callback );

	// start: ------------------- EnvironmentDataProvider -------------------
	virtual std::pair< float, bool > getData() const override;
	virtual bool isAvailable() const override;
	// end: --------------------- EnvironmentDataProvider -------------------
private:
	bool createDS18B20();
	bool createSI7021();

	void threadWatcherDS18B20();
	void threadWatcherSI7021();

private:
	std::unique_ptr< DS18B20 > m_DS18B20Sensor;
	std::unique_ptr< SI7021 > m_SI7021Sensor;
};

class HumiditProvider : public EnvironmentDataProvider
{
public:
	HumiditProvider( std::experimental::filesystem::path xmlPath, std::function< void( float ) > callback );

	// start: ------------------- EnvironmentDataProvider -------------------
	virtual std::pair< float, bool > getData() const override;
	virtual bool isAvailable() const override;
	// end: --------------------- EnvironmentDataProvider -------------------
protected:
	bool createSI7021();
	void threadWatcherSI7021();

protected:
	std::unique_ptr< SI7021 > m_SI7021Sensor;
};