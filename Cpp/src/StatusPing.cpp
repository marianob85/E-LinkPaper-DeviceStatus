#include <memory>
#include <algorithm>
#include <future>
#include <epdpaint.h>
#include "StatusPing.hpp"
#include "DeviceDef.hpp"
#include <ping.h>

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

	const auto* font		   = &Font12;
	static const char* online  = "online";
	static const char* offline = "offline";

	auto devicestatus = getDeviceStatus();

	unsigned startLine = 5;
	for( const auto& device : deviceAddress )
	{
		auto status = devicestatus.at( device.ip );

		const char* text = m_currentPage == 0 ? device.ip : device.name;

		auto size = painter.getStringSize( text, font );
		painter.drawStringAt( 2, startLine, text, font, UNCOLORED );

		if( status )
		{
			size = painter.getStringSize( online, font );
			painter.drawStringAt( m_width / 2 - size.first - 2, startLine, online, font, UNCOLORED );
		}
		else
		{
			size		  = painter.getStringSize( offline, font );
			auto position = make_pair( m_width / 2 - size.first - 2, startLine );

			painter.drawFilledRectangle( position.first - 2,
										 position.second - 2,
										 position.first + size.first,
										 position.second + size.second,
										 UNCOLORED );

			painter.drawStringAt( position.first, position.second, offline, font, COLORED );
		}

		startLine += size.second + 5;
	}

	return painter;
}

// end: --------------------- StatusPing -------------------
