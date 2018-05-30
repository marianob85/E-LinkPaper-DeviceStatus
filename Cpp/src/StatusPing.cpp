#include <memory>
#include <algorithm>
#include <future>
#include <ping.h>
#include <Painter.hpp>
#include <KS0108.hpp>
#include "StatusPing.hpp"
#include "DeviceDef.hpp"

using namespace std;

// start: ------------------- StatusPing -------------------

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

map< string, bool > StatusPing::getDeviceStatus() const
{
	vector< future< std::pair< string, bool > > > values;

	auto pingSingleIP = []( const char* address ) -> pair< string, bool > {
		PingResult pingResult;
		Ping ping   = Ping();
		bool status = ping.ping( address, 4, pingResult );
		status &= std::count_if( pingResult.icmpEchoReplys.begin(),
								 pingResult.icmpEchoReplys.end(),
								 []( const IcmpEchoReply& echoReplay ) { return echoReplay.isReply; } )
			> 0;

		return { address, status };
	};

	for( const auto& device : deviceAddress )
		values.push_back( std::async( std::launch::async, pingSingleIP, device.ip ) );

	std::map< std::string, bool > deviceStatus;

	for( auto& val : values )
		deviceStatus.insert( val.get() );

	return deviceStatus;
}

Paint StatusPing::currentPage() const
{
	std::unique_ptr< uint8_t[] > frameBuffer = make_unique< uint8_t[] >( m_width / 8 * m_height );
	auto painter							 = Paint( move( frameBuffer ), m_width, m_height );

	auto font = painter.createFonter< FontPainterKS0108 >( liberationMono10 );

	static const string online  = "online";
	static const string offline = "offline";

	auto devicestatus = getDeviceStatus();

	unsigned startLine = 5;
	for( const auto& device : deviceAddress )
	{
		auto status = devicestatus.at( device.ip );

		const char* text = m_currentPage == 0 ? device.ip : device.name;

		auto size = font->getStringSize( text );
		font->drawString( 2, startLine, text, UNCOLORED );

		if( status )
		{
			size = font->getStringSize( online );
			font->drawString( m_width / 2 - size.width - 2, startLine, online, UNCOLORED );
		}
		else
		{
			size		  = font->getStringSize( offline );
			auto position = make_pair( m_width / 2 - size.width - 2, startLine );

			painter.drawFilledRectangle( position.first - 2,
										 position.second - 2,
										 position.first + size.width,
										 position.second + size.height,
										 UNCOLORED );

			font->drawString( position.first, position.second, offline, COLORED );
		}

		startLine += size.height + 3;
	}

	return painter;
}

// end: --------------------- StatusPing -------------------
