#include <memory>
#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <chrono>
#include <ping.h>
#include <Painter.hpp>
#include <KS0108.hpp>
#include <pugixml.hpp>
#include "StatusPing.hpp"

using namespace std;

// start: ------------------- StatusPing -------------------
StatusPing::StatusPing( std::experimental::filesystem::path xmlPath )
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

	auto config = doc.child( "Config" );

	for( auto device : config.child( "DeviceStatus" ).child( "Devices" ).children() )
		m_devices.push_back( { device.text().as_string(), device.attribute( "Name" ).as_string() } );

	m_pingCount = config.child( "Ping" ).child( "Count" ).text().as_int( 4 );

	m_pinger = thread( std::bind( &StatusPing::pinger, this ) );
}

bool StatusPing::setPage( unsigned page )
{
	m_currentPage = page % pageNo();
	return true;
}

bool StatusPing::setNext()
{
	m_currentPage = ( m_currentPage + 1 ) % pageNo();
	return true;
}

string StatusPing::getDescription() const
{
	return "Device status";
}

unsigned StatusPing::pageNo() const
{
	return 2;
}

unsigned StatusPing::currentPageNo() const
{
	return m_currentPage;
}

// map< string, bool > StatusPing::getDeviceStatus() const
//{
//	vector< future< std::pair< string, bool > > > values;
//
//	auto pingSingleIP = [m_pingCount = m_pingCount]( string address ) -> pair< string, bool > {
//		PingResult pingResult;
//		Ping ping   = Ping();
//		bool status = ping.ping( address.c_str(), m_pingCount, pingResult );
//		status &= std::count_if( pingResult.icmpEchoReplys.begin(),
//								 pingResult.icmpEchoReplys.end(),
//								 []( const IcmpEchoReply& echoReplay ) { return echoReplay.isReply; } )
//			> 0;
//
//		return { address, status };
//	};
//
//	for( const auto& device : m_devices )
//		values.push_back( std::async( std::launch::async, pingSingleIP, device.Ip ) );
//
//	std::map< std::string, bool > deviceStatus;
//
//	for( auto& val : values )
//		deviceStatus.insert( val.get() );
//
//	return deviceStatus;
//}

void StatusPing::pinger()
{
	while( true )
	{
		for( auto device : m_devices )
		{
			PingResult pingResult;
			Ping ping   = Ping();
			bool status = ping.ping( device.Ip.c_str(), m_pingCount, pingResult );
			status &= std::count_if( pingResult.icmpEchoReplys.begin(),
									 pingResult.icmpEchoReplys.end(),
									 []( const IcmpEchoReply& echoReplay ) { return echoReplay.isReply; } )
				> 0;

			std::lock_guard< std::mutex > lock( m_statusMutex );

			if( m_status.find( device.Ip ) != m_status.end() )
				m_status.at( device.Ip ) |= status;
			else
				m_status[ device.Ip ] = status;
		}
	}
}

std::map< std::string, bool > StatusPing::getDeviceStatusV2() const
{
	while( m_devices.size() != m_status.size() )
		this_thread::sleep_for( 1s );

	std::lock_guard< std::mutex > lock( m_statusMutex );

	auto retVal = m_status;
	m_status.clear();
	return retVal;
}

std::unique_ptr< Paint > StatusPing::currentPage() const
{
	auto painter = make_unique< Paint2Colors >( m_width, m_height );

	auto font = painter->createFonter< FontPainterKS0108 >( liberationMono10 );

	static const string online  = "online";
	static const string offline = "offline";
	const auto devicestatus		= getDeviceStatusV2();
	const unsigned startLineDef = 5;

	size_t startLine		   = startLineDef;
	unsigned columnOffset	  = 0;
	const unsigned columnWidth = m_width / 2;
	for( const auto& device : m_devices )
	{
		const auto status = devicestatus.at( device.Ip );
		const auto& text  = m_currentPage == 0 ? device.Ip : device.Name;
		auto size		  = font->getStringSize( text );

		if( size.height + startLine > m_height )
		{
			// Next column
			startLine = startLineDef;
			columnOffset += columnWidth;
		}

		font->drawString( columnOffset + 2, startLine, text, Color::White );

		if( status )
		{
			size = font->getStringSize( online );
			font->drawString( columnOffset + columnWidth - size.width - 2, startLine, online, Color::White );
		}
		else
		{
			size		  = font->getStringSize( offline );
			auto position = make_pair( columnOffset + columnWidth - size.width - 2, startLine );

			painter->drawFilledRectangle( position.first - 2,
										  position.second - 2,
										  position.first + size.width,
										  position.second + size.height,
										  Color::White );

			font->drawString( position.first, position.second, offline, Color::Black );
		}

		startLine += size.height + 3;
	}

	return painter;
}

// end: --------------------- StatusPing -------------------
