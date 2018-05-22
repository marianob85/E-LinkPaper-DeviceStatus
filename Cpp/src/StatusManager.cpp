#include <iostream>
#include <tuple>
#include <epd.h>
#include "DeviceDef.hpp"
#include "StatusManager.hpp"

using namespace std;

// start: ------------------- StatusManager -------------------
StatusManager::StatusManager()
	: m_epd( make_unique< EpdWiringPi >( CHANNEL ), RST_PIN, DC_PIN, CS_PIN, BUSY_PIN, EPD_WIDTH, EPD_HEIGHT )
{
	std::unique_ptr< uint8_t[] > frameBuffer = make_unique< uint8_t[] >( m_epd.width() / 8 * m_epd.height() );
	m_painter = make_unique< Paint >( move( frameBuffer ), m_epd.width(), m_epd.height() );
}

StatusManager::~StatusManager() {}

bool StatusManager::init()
{
	if( m_epd.init() == false )
	{
		wcout << L"e-Paper init failed\n";
		return false;
	}

	return true;
}

bool StatusManager::add( std::unique_ptr< StatusPage > statusPage, unsigned intervalSeconds )
{
	if( !statusPage->init( m_epd.width(), m_epd.height() - s_headeWHeight ) )
	{
		return false;
	}

	m_pages.push_back( make_pair( move( statusPage ), intervalSeconds ) );

	m_currentPage = m_pages.begin();
	m_currentPage->first->setPage( 0 );
	return true;
}

void StatusManager::setNext()
{
	if( !m_currentPage->first->setNext() )
		if( ++m_currentPage == m_pages.end() )
			m_currentPage = m_pages.begin();

	refreshPage();
}

bool StatusManager::setPage( unsigned page ) {}

void StatusManager::autoChange( bool set ) {}

void StatusManager::refreshPage()
{
	// m_epd.reset();
	m_painter->clear( UNCOLORED );

	printHeader();

	// Merge page content
	m_painter->merge( 0, s_headeWHeight, m_currentPage->first->currentPage() );

	// Draw vertical line
	m_painter->drawVerticalLine( m_epd.width() / 2, s_headeWHeight, m_epd.height(), COLORED );

	m_epd.displayFrame( m_painter->rawImage() );
	// m_epd.sleep();
}

void StatusManager::printHeader()
{
	const auto* font = &Font16;

	// Print pages
	char text[ 128 ] = {};
	sprintf( text, "Page %d/%d", currentPageNo() + 1, pagesNo() );
	auto size = m_painter->getStringSize( text, font );
	auto y	= ( s_headeWHeight - 0 - size.second ) / 2 + 1;
	m_painter->drawFilledRectangle( 0, 0, m_epd.width(), s_headeWHeight, COLORED );
	m_painter->drawStringAt( m_epd.width() - size.first - 2, y, text, font, UNCOLORED );

	// Print description
	auto description = m_currentPage->first->getDescription();
	size			 = m_painter->getStringSize( description.c_str(), font );
	y				 = ( s_headeWHeight - 0 - size.second ) / 2 + 1;
	m_painter->drawStringAt( 2, y, description.c_str(), font, UNCOLORED );
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
