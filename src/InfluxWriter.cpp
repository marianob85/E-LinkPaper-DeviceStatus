#include <iostream>
#include <pugixml.hpp>
#include "InfluxWriter.hpp"

using namespace std;

// start: ------------------- InfluxWriter -------------------

InfluxWriter::InfluxWriter( std::experimental::filesystem::path xmlPath )
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( xmlPath.string().c_str() );

	if( !result )
	{
		std::cout << "XML [" << xmlPath.string().c_str() << "] parsed with errors, attr value: ["
				  << doc.child( "node" ).attribute( "attr" ).value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..."
				  << ( xmlPath.string().c_str() + result.offset ) << "]\n\n";
		throw std::exception();
	}

	auto config		= doc.child( "Config" );
	auto influxNode = config.child( "InfluxData" );
	m_measurmenet   = influxNode.child( "Measurement" ).text().as_string();

	for( auto tag : influxNode.child( "Tags" ) )
	{
		m_tags.emplace( tag.name(), tag.text().as_string() );
	}

	m_influxServer = influxdb_cpp::server_info( influxNode.child( "Address" ).text().as_string(),
												influxNode.child( "Port" ).text().as_uint(),
												influxNode.child( "Database" ).text().as_string(),
												influxNode.child( "User" ).text().as_string(),
												influxNode.child( "Password" ).text().as_string() );

	m_worker = thread( std::bind( &InfluxWriter::worker, this ) );
}

void InfluxWriter::worker()
{
	while( true )
	{
		auto temp = m_SI7021.getTemp();
		auto hum  = m_SI7021.gethumidity();
		std::string resp;
		auto data = influxdb_cpp::builder().meas( m_measurmenet );

		for( const auto& tag : m_tags )
			data.tag( tag.first, tag.second );

		data.field( "temperature", temp.first )
			.field( "tempearture_valid", temp.second )
			.field( "humidity", hum.first )
			.field( "humidity_valid", hum.second )
			.timestamp( getUnixTimeStamp() )
			.post_http( m_influxServer, &resp );

		this_thread::sleep_for( std::chrono::minutes( 1 ) );
	}
}

uint64_t InfluxWriter::getUnixTimeStamp()
{
	return std::chrono::duration_cast< std::chrono::nanoseconds >( std::chrono::system_clock::now().time_since_epoch() )
		.count();
}

// end: --------------------- InfluxWriter -------------------
