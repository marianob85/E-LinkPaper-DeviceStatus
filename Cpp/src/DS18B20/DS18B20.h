#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <vector>

#define BUS "/sys/bus/w1/devices/"
#define PREFIX "/28-"
#define TEMPFILE "/w1_slave"

class DS18B20
{
public:
	enum class TemperatureType
	{
		Celcius,
		Fahrenheit,
	};

public:
	DS18B20( const std::string address );
	virtual ~DS18B20() = default;

	TemperatureType getUnits() const;
	void setUnits( TemperatureType temperatureType );
	float getTemp();
	float CtoF( float );
	std::string id() const;

private:
	TemperatureType m_temperatureType{ TemperatureType::Celcius };
	std::string m_sensorPath;
	std::string m_id;
};

class DS18B20Mgr
{
public:
	DS18B20Mgr( const std::string& path );
	virtual ~DS18B20Mgr() = default;

	size_t count() const;
	DS18B20 operator[]( unsigned sensor ) const;

private:
	std::vector< std::string > readDirectory( const std::string& name ) const;

private:
	std::vector< std::string > m_sensors;
};
