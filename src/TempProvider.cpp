#include <cmath>
#include <future>
#include "TempProvider.hpp"

using namespace std;

// start: ------------------- EnvironmentDataProvider -------------------

EnvironmentDataProvider::EnvironmentDataProvider( std::function< void( float ) > callback ) : m_callback( callback ) {}

// end: --------------------- EnvironmentDataProvider -------------------

// start: ------------------- TempProvider -------------------

TempProvider::TempProvider( std::function< void( float ) > callback ) : EnvironmentDataProvider( callback )
{
	if( createSI7021() == false )
		createDS18B20();
}

float TempProvider::getData() const
{
	if( m_DS18B20Sensor )
		return m_DS18B20Sensor->getTemp();

	return 0.f;
}

bool TempProvider::isAvailable() const
{
	return bool( m_DS18B20Sensor );
}

bool TempProvider::createDS18B20()
{
	DS18B20Mgr ds18B20Mgr( BUS );
	if( ds18B20Mgr.count() > 0 )
	{
		m_DS18B20Sensor = make_unique< DS18B20 >( ds18B20Mgr[ 0 ] );
		m_DS18B20Sensor->setUnits( DS18B20 ::TemperatureType::Celcius );
		// cout << "Found DS18B20 sensor. ID: " << m_tempSensor->id();

		m_watcher = thread( std::bind( &TempProvider::threadWatcherDS18B20, this ) );
		return true;
	}
	else
		return false;
}

bool TempProvider::createSI7021()
{
	return false;
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

// end: --------------------- TempProvider -------------------
