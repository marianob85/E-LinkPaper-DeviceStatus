/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Name        : DS18B20.h
 * Author      : Georgi Todorov <terahz@geodar.com>
 * Version     :
 * Created on  : Jan 3, 2013
 *
 * Copyright © 2013 Georgi Todorov <terahz@geodar.com>
 * https://github.com/TeraHz/DS18B20
 */
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

private:
	TemperatureType m_temperatureType{ TemperatureType::Celcius };
	std::string m_sensorPath;
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
