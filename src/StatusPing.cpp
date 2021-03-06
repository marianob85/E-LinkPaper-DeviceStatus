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

	m_pingCount  = config.child( "Ping" ).child( "Count" ).text().as_int( 4 );
	m_pingLoops  = std::max( 1, config.child( "Ping" ).child( "Loops" ).text().as_int( 1 ) );
	m_loopsDelay = config.child( "Ping" ).child( "LoopsDelayS" ).text().as_int();

	m_pinger = thread( std::bind( &StatusPing::pinger, this ) );
}

bool StatusPing::setPage( unsigned /*page*/ )
{
	return true;
}

bool StatusPing::setNext()
{
	return true;
}

string StatusPing::getDescription( size_t index ) const
{
	switch( index )
	{
	default:
	case 0:
		return "Device status";
		break;
	case 1:
	{
		auto status	= getDeviceStatusV2();
		auto available = std::count_if(
			status.begin(), status.end(), []( const std::pair< string, bool >& status ) { return status.second; } );

		return string( "Online: " ) + to_string( available ) + "/" + to_string( status.size() );
	}
	break;
	}

	return "";
}

unsigned StatusPing::pageNo() const
{
	return 1;
}

unsigned StatusPing::currentPageNo() const
{
	return 0;
}

void StatusPing::pinger()
{
	while( true )
	{
		bool changed = false;
		for( unsigned i = 0; i < m_pingLoops; ++i )
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
				{
					auto last				 = m_status.at( device.Ip );
					m_status.at( device.Ip ) = status;
					changed |= last != status;
				}

				else
					m_status[ device.Ip ] = status;
			}
			this_thread::sleep_for( std::chrono::seconds( m_loopsDelay ) );
		}

		if( changed )
			std::async( std::launch::async, m_refresh, 0 );
	}
}

std::map< std::string, bool > StatusPing::getDeviceStatusV2() const
{
	while( m_devices.size() != m_status.size() )
		this_thread::sleep_for( 1s );

	std::lock_guard< std::mutex > lock( m_statusMutex );

	auto retVal = m_status;
	return retVal;
}

std::unique_ptr< Paint > StatusPing::currentPage( size_t width, size_t height ) const
{
	auto painter = make_unique< Paint3Colors >( width, height );
	painter->clear( Color::White );
	auto font = painter->createFonter< FontPainterKS0108 >( liberationMono10 );

	static const string online  = "online";
	static const string offline = "offline";
	const auto devicestatus		= getDeviceStatusV2();
	const unsigned startLineDef = 5;

	size_t startLine		   = startLineDef;
	unsigned columnOffset	  = 0;
	const unsigned columnWidth = width / 2;
	for( const auto& device : m_devices )
	{
		const auto status = devicestatus.at( device.Ip );
		auto size		  = font->getStringSize( device.Ip );

		if( size.height + startLine > height )
		{
			// Next column
			startLine = startLineDef;
			columnOffset += columnWidth;
		}

		font->drawString( columnOffset + 2, startLine, device.Ip, Color::Black );
		font->drawString( columnOffset + 130, startLine, device.Name, Color::Black );

		if( status )
		{
			size = font->getStringSize( online );
			font->drawString( columnOffset + columnWidth - size.width - 2, startLine, online, Color::Color1 );
		}
		else
		{
			size		  = font->getStringSize( offline );
			auto position = make_pair( columnOffset + columnWidth - size.width - 2, startLine );

			painter->drawFilledRectangle( position.first - 2,
										  position.second - 2,
										  position.first + size.width,
										  position.second + size.height,
										  Color::Color1 );

			font->drawString( position.first, position.second, offline, Color::White );
		}

		startLine += size.height + 3;
	}

	return painter;
}

// end: --------------------- StatusPing -------------------
