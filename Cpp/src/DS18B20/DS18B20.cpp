#include <string>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>
#include <regex>
#include "DS18B20.h"

using namespace std;

// start: ------------------- DS18B20 -------------------

DS18B20::DS18B20( const std::string address ) : m_id( address )
{
	m_temperatureType = TemperatureType::Celcius;
	m_sensorPath	  = BUS + address + TEMPFILE;
}

float DS18B20::getTemp()
{
	FILE* devFile = fopen( m_sensorPath.c_str(), "r" );
	if( devFile == NULL )
	{
		printf( "Count not open %s\n", m_sensorPath.c_str() );
		perror( "\n" );
	}
	float temp = -1;
	if( devFile != NULL )
	{
		if( !ferror( devFile ) )
		{
			unsigned int tempInt;
			char crcConf[ 5 ];
			fscanf( devFile, "%*x %*x %*x %*x %*x %*x %*x %*x %*x : crc=%*x %s", crcConf );
			if( strncmp( crcConf, "YES", 3 ) == 0 )
			{
				fscanf( devFile, "%*x %*x %*x %*x %*x %*x %*x %*x %*x t=%5d", &tempInt );
				temp = ( float )tempInt / 1000.0f;
			}
		}
	}
	fclose( devFile );

	if( m_temperatureType == TemperatureType::Celcius )
	{
		return temp;
	}
	else
		return CtoF( temp );
}

DS18B20::TemperatureType DS18B20::getUnits() const
{
	return m_temperatureType;
}

void DS18B20::setUnits( TemperatureType temperatureType )
{
	m_temperatureType = temperatureType;
}

float DS18B20::CtoF( float temp )
{
	return temp * 1.8f + 32;
}

std::string DS18B20::id() const
{
	return m_id;
}

// end: --------------------- DS18B20 -------------------

// start: ------------------- DS18B20Mgr -------------------

DS18B20Mgr::DS18B20Mgr( const string& path )
{
	m_sensors = readDirectory( path );
}

vector< string > DS18B20Mgr::readDirectory( const std::string& name ) const
{
	vector< string > files;
	DIR* dirp = opendir( name.c_str() );
	struct dirent* dp{ nullptr };
	while( ( dp = readdir( dirp ) ) != NULL )
		files.push_back( dp->d_name );
	closedir( dirp );

	auto badSensorPattern = []( const std::string& name ) -> bool {
		regex pattern( "^28-[0-9A-Fa-f]+$" );
		return !regex_match( name, pattern );
	};

	files.erase( std::unique( files.begin(), files.end() ), files.end() );
	files.erase( remove_if( files.begin(), files.end(), badSensorPattern ), files.end() );
	return files;
}

size_t DS18B20Mgr::count() const
{
	return m_sensors.size();
}

DS18B20 DS18B20Mgr::operator[]( unsigned sensor ) const
{
	return DS18B20( m_sensors[ sensor ] );
}

// end: --------------------- DS18B20Mgr -------------------
