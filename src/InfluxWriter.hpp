#pragma once
#include <map>
#include <thread>
#include <experimental/filesystem>
#include <SI7021.hpp>
#include <influxdb.hpp>

class InfluxWriter
{
public:
	InfluxWriter( std::experimental::filesystem::path xmlPath );

private:
	void worker();
	uint64_t getUnixTimeStamp();

private:
	std::unique_ptr< SI7021 > m_SI7021;
	std::thread m_worker;
	influxdb_cpp::server_info m_influxServer;
	std::string m_measurmenet;
	std::map< std::string, std::string > m_tags;
};