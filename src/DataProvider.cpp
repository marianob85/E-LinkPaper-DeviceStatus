#include <cmath>
#include <future>
#include "DataProvider.hpp"

using namespace std;

// start: ------------------- EnvironmentDataProvider -------------------

EnvironmentDataProvider::EnvironmentDataProvider( std::function< void( float ) > callback ) : m_callback( callback ) {}

void EnvironmentDataProvider::reset()
{
	auto data = getData();
	if( data.second )
		m_lastTemp = data.first;
}

// end: --------------------- EnvironmentDataProvider -------------------

// start: ------------------- TempProvider -------------------

TempProvider::TempProvider( std::function< void( float ) > callback ) : EnvironmentDataProvider( callback )
{
	if( createSI7021() == false )
		createDS18B20();
}

std::pair< float, bool > TempProvider::getData() const
{
	if( m_SI7021Sensor )
		return m_SI7021Sensor->getTemp();
	else if( m_DS18B20Sensor )
		return { m_DS18B20Sensor->getTemp(), true };

	return { 0.f, false };
}

bool TempProvider::isAvailable() const
{
	return bool( m_DS18B20Sensor ) || bool( m_SI7021Sensor );
}

bool TempProvider::createDS18B20()
{
	DS18B20Mgr ds18B20Mgr( BUS );
	if( ds18B20Mgr.count() > 0 )
	{
		m_DS18B20Sensor = make_unique< DS18B20 >( ds18B20Mgr[ 0 ] );
		m_DS18B20Sensor->setUnits( DS18B20 ::TemperatureType::Celcius );
		m_watcher = thread( std::bind( &TempProvider::threadWatcherDS18B20, this ) );
	}

	return bool( m_DS18B20Sensor );
}

bool TempProvider::createSI7021()
{
	m_SI7021Sensor = std::make_unique< SI7021 >();

	auto ret = m_SI7021Sensor->getTemp();
	ret		 = m_SI7021Sensor->getTemp();
	ret		 = m_SI7021Sensor->getTemp();
	if( ret.second )
	{
		m_lastTemp = ret.first;
		m_watcher  = thread( std::bind( &TempProvider::threadWatcherSI7021, this ) );
	}
	else
		m_SI7021Sensor.reset();

	return bool( m_SI7021Sensor );
}

void TempProvider::threadWatcherDS18B20()
{
	while( true )
	{
		auto temp = m_DS18B20Sensor->getTemp();
		if( fabs( temp - m_lastTemp ) > 0.2f )
		{
			m_lastTemp = temp;
			std::async( std::launch::async, m_callback, m_lastTemp );
		};
		std::this_thread::sleep_for( std::chrono::seconds( 30 ) );
	}
}

void TempProvider::threadWatcherSI7021()
{
	while( true )
	{
		auto temp = m_SI7021Sensor->getTemp();
		if( !temp.second )
			temp = m_SI7021Sensor->getTemp();
		if( temp.second )
		{
			if( fabs( temp.first - m_lastTemp ) > 0.2f )
			{
				m_lastTemp = temp.first;
				std::async( std::launch::async, m_callback, m_lastTemp );
			};
			std::this_thread::sleep_for( std::chrono::seconds( 30 ) );
		}
		else
			std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}
}

// end: --------------------- TempProvider -------------------

// start: ------------------- HumiditProvider -------------------

HumiditProvider::HumiditProvider( std::function< void( float ) > callback ) : EnvironmentDataProvider( callback )
{
	createSI7021();
}

std::pair< float, bool > HumiditProvider::getData() const
{
	if( m_SI7021Sensor )
		return m_SI7021Sensor->gethumidity();
	return { 0.f, false };
}

bool HumiditProvider::isAvailable() const
{
	return bool( m_SI7021Sensor );
}

bool HumiditProvider::createSI7021()
{
	m_SI7021Sensor = std::make_unique< SI7021 >();

	auto ret = m_SI7021Sensor->gethumidity();
	ret		 = m_SI7021Sensor->gethumidity();
	ret		 = m_SI7021Sensor->gethumidity();
	if( ret.second )
	{
		m_lastTemp = ret.first;
		m_watcher  = thread( std::bind( &HumiditProvider::threadWatcherSI7021, this ) );
	}
	else
		m_SI7021Sensor.reset();

	return bool( m_SI7021Sensor );
}

void HumiditProvider::threadWatcherSI7021()
{
	while( true )
	{

		auto temp = m_SI7021Sensor->gethumidity();
		if( !temp.second )
			temp = m_SI7021Sensor->gethumidity();
		if( temp.second )
		{
			if( fabs( temp.first - m_lastTemp ) > 0.5f )
			{
				m_lastTemp = temp.first;
				std::async( std::launch::async, m_callback, m_lastTemp );
			};
			std::this_thread::sleep_for( std::chrono::seconds( 30 ) );
		}
		else
			std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}
}

// end: --------------------- HumiditProvider -------------------
