#include <memory>
#include <epdpaint.h>
#include "StatusPing.hpp"
#include "DeviceDef.hpp"

using namespace std;

// start: ------------------- StatusPing -------------------

bool StatusPing::setPage( unsigned page )
{
	return false;
}

bool StatusPing::setNext()
{
	return false;
}

std::string StatusPing::getDescription() const
{
	return "Device status";
}

unsigned StatusPing::pageNo() const
{
	return 1;
}

unsigned StatusPing::currentPageNo() const
{
	return 0;
}

Paint StatusPing::currentPage() const
{
	std::unique_ptr< uint8_t[] > frameBuffer = make_unique< uint8_t[] >( m_width / 8 * m_height );
	auto painter							 = Paint( move( frameBuffer ), m_width, m_height );

	const auto* font		   = &Font12;
	static const char* online  = "online";
	static const char* offline = "offline";

	unsigned startLine = 5;
	for( const auto& device : deviceAddress )
	{
		auto size = painter.getStringSize( device.ip, font );
		painter.drawStringAt( 2, startLine, device.ip, font, UNCOLORED );

		bool status = rand() % 3;

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
										 position.first + size.first ,
										 position.second + size.second ,
										 UNCOLORED );

			painter.drawStringAt( position.first, position.second, offline, font, COLORED );
		}

		startLine += size.second + 5;
	}

	return painter;
}

// end: --------------------- StatusPing -------------------
