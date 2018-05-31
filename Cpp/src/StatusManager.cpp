#include <iostream>
#include <tuple>
#include <epd.h>
#include <pugixml.hpp>
#include <KS0108.hpp>
#include "DeviceDef.hpp"
#include "StatusManager.hpp"

using namespace std;

// start: ------------------- StatusManager -------------------
StatusManager::StatusManager( std::experimental::filesystem::path xmlPath )
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

	auto epdConfig = doc.child( "Config" ).child( "Epd" );

	m_epd = make_unique< Epd4in2 >( make_unique< EpdWiringPi >( epdConfig.child( "SPIChannel" ).text().as_int() ),
									epdConfig.child( "RS_PIN" ).text().as_int(),
									epdConfig.child( "DC_PIN" ).text().as_int(),
									epdConfig.child( "CS_PIN" ).text().as_int(),
									epdConfig.child( "BUSY_PIN" ).text().as_int(),
									epdConfig.child( "Width" ).text().as_int(),
									epdConfig.child( "Height" ).text().as_int() );

	std::unique_ptr< uint8_t[] > frameBuffer = make_unique< uint8_t[] >( m_epd->width() / 8 * m_epd->height() );
	m_painter = make_unique< Paint >( move( frameBuffer ), m_epd->width(), m_epd->height() );

	DS18B20Mgr ds18B20Mgr( BUS );
	if( ds18B20Mgr.count() > 0 )
	{
		m_tempSensor = make_unique< DS18B20 >( ds18B20Mgr[ 0 ] );
		cout << "Found DS18B20 sensor. ID: " << m_tempSensor->id();
	}
}

StatusManager::~StatusManager() {}

bool StatusManager::init()
{
	if( m_epd->init() == false )
	{
		wcout << L"e-Paper init failed\n";
		return false;
	}
	return true;
}

bool StatusManager::add( std::unique_ptr< StatusPage > statusPage, unsigned intervalSeconds )
{
	if( !statusPage->init( m_epd->width(), m_epd->height() - s_headeWHeight ) )
		return false;

	m_pages.push_back( make_pair( move( statusPage ), intervalSeconds ) );

	m_currentPage = m_pages.begin();
	return true;
}

void StatusManager::setNext()
{
	if( !m_currentPage->first->setNext() )
		if( ++m_currentPage == m_pages.end() )
			m_currentPage = m_pages.begin();

	refreshPage();
}

bool StatusManager::setPage( unsigned page, unsigned subPage )
{
	if( page < m_pages.size() )
	{
		m_pages[ 0 ].first->setPage( subPage );
		refreshPage();
		return true;
	}
	return false;
}

void StatusManager::autoChange( bool set )
{
	auto timeMS  = m_currentPage->second * 1000;
	m_timerEvent = TimerEvent( timeMS, true, std::bind( &StatusManager::onTimer, this ) );
}

void StatusManager::onTimer()
{
	setNext();
	auto timeMS  = m_currentPage->second * 1000;
	m_timerEvent = TimerEvent( timeMS, true, std::bind( &StatusManager::onTimer, this ) );
}

void StatusManager::refreshPage()
{
	m_epd->init();
	m_painter->clear( UNCOLORED );

	printHeader();

	// Merge page content
	m_painter->merge( 0, s_headeWHeight, m_currentPage->first->currentPage() );

	// Draw vertical line
	m_painter->drawVerticalLine( m_epd->width() / 2, s_headeWHeight, m_epd->height(), COLORED );

	m_epd->displayFrame( m_painter->rawImage() );
	m_epd->sleep();
}

void StatusManager::printHeader()
{
	auto font = m_painter->createFonter< FontPainterKS0108 >( liberationMono11Bold );

	// Print pages
	char text[ 128 ] = {};
	sprintf( text, "Page %d/%d", currentPageNo() + 1, pagesNo() );
	auto size = font->getStringSize( text );
	auto y	= ( s_headeWHeight - 0 - size.height ) / 2 + 1;
	m_painter->drawFilledRectangle( 0, 0, m_epd->width(), s_headeWHeight, COLORED );

	font->drawString( m_epd->width() - size.width - 2, y, text, UNCOLORED );

	// Print description
	auto description = m_currentPage->first->getDescription();
	size			 = font->getStringSize( description );
	y				 = ( s_headeWHeight - 0 - size.height ) / 2 + 1;

	font->drawString( 2, y, description, UNCOLORED );

	// Print temp
	if( m_tempSensor )
	{
		char text[ 20 ];
		sprintf( text, "T:%4.2f%cC", m_tempSensor->getTemp(), 0xB0 );
		font->drawString( 170, y, text, UNCOLORED );
	}
}

unsigned StatusManager::pagesNo() const
{
	unsigned pagesNo = 0;
	for( const auto& page : m_pages )
		pagesNo += page.first->pageNo();

	return pagesNo;
}

unsigned StatusManager::currentPageNo() const
{
	auto it			= m_pages.begin();
	unsigned pageNo = 0;

	while( it != m_currentPage )
	{
		pageNo += m_currentPage->first->pageNo();
		it++;
	}

	return pageNo + m_currentPage->first->currentPageNo();
}

// end: --------------------- StatusManager -------------------

// start: ------------------- StatusPage -------------------

bool StatusPage::init( unsigned width, unsigned height )
{
	m_height = height;
	m_width  = width;

	return true;
}

// end: --------------------- StatusPage -------------------
