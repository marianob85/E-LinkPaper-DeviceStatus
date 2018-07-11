#include <iostream>
#include <future>
#include <tuple>
#include <epdWiringPi.hpp>
#include <pugixml.hpp>
#include <KS0108.hpp>
#include <epd7in5b.hpp>
#include "StatusManager.hpp"
#include <Image.hpp>

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

	m_epd = make_unique< Epd7in5b >( make_unique< EpdWiringPi >( epdConfig.child( "SPIChannel" ).text().as_int() ),
									 epdConfig.child( "RS_PIN" ).text().as_int(),
									 epdConfig.child( "DC_PIN" ).text().as_int(),
									 epdConfig.child( "CS_PIN" ).text().as_int(),
									 epdConfig.child( "BUSY_PIN" ).text().as_int(),
									 epdConfig.child( "Width" ).text().as_int(),
									 epdConfig.child( "Height" ).text().as_int() );

	m_painter = make_unique< Paint3Colors >( m_epd->width(), m_epd->height() );

	m_temperature
		= make_unique< TempProvider >( std::bind( &StatusManager::onTemperature, this, std::placeholders::_1 ) );
	m_humidit = make_unique< HumiditProvider >( std::bind( &StatusManager::onHumidit, this, std::placeholders::_1 ) );
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

bool StatusManager::add( std::unique_ptr< StatusPage > statusPage )
{
	statusPage->init( std::bind( &StatusManager::requestPageRefresh, this, m_pages.size(), std::placeholders::_1 ) );
	m_pages.push_back( move( statusPage ) );
	m_currentPage = m_pages.begin();
	return true;
}

void StatusManager::setNext()
{
	if( !( *m_currentPage )->setNext() )
		if( ++m_currentPage == m_pages.end() )
			m_currentPage = m_pages.begin();

	refreshPage();
}

bool StatusManager::setPage( unsigned page, unsigned subPage )
{
	if( page < m_pages.size() )
	{
		m_pages[ 0 ]->setPage( subPage );
		refreshPage();
		return true;
	}
	return false;
}

void StatusManager::close()
{
	m_epd->sleep();
}

void StatusManager::requestPageRefresh( size_t page, size_t subPage )
{
	if( page == currentPageNo() )
		setPage( page, subPage );
}

void StatusManager::refreshPage()
{
	std::lock_guard< std::mutex > lock( m_refreshMutex );

	m_epd->init();
	m_painter->clear( Color::White );

	auto height = printHeader2();

	// Merge page content
	m_painter->merge( 0, height, ( *m_currentPage )->currentPage( m_epd->width(), m_epd->height() - height ) );

	// Draw vertical line
	m_painter->drawVerticalLine( m_epd->width() / 2, height, m_epd->height(), Color::Black );

	m_epd->displayFrame( *m_painter );
	m_epd->sleep();
}

size_t StatusManager::printHeader()
{
	const size_t height = 20;
	auto font			= m_painter->createFonter< FontPainterKS0108 >( liberationMono11Bold );

	// Print pages
	char text[ 128 ] = {};
	sprintf( text, "Page %d/%d", currentPageNo() + 1, pagesNo() );
	auto size = font->getStringSize( text );
	auto y	= ( height - 0 - size.height ) / 2 + 1;
	m_painter->drawFilledRectangle( 0, 0, m_epd->width(), height, Color::Black );

	font->drawString( m_epd->width() - size.width - 2, y, text, Color::White );

	// Print description
	auto description = ( *m_currentPage )->getDescription( 0 );
	size			 = font->getStringSize( description );
	y				 = ( height - 0 - size.height ) / 2 + 1;

	font->drawString( 2, y, description, Color::White );

	// Print temp
	// if( m_tempSensor )
	//{
	//	char text[ 20 ];
	//	sprintf( text, "T:%4.2f%cC", m_tempSensor->getTemp(), 0xB0 );
	//	font->drawString( 170, y, text, Color::White );
	//}

	return height;
}

size_t StatusManager::printHeader2()
{
	const size_t height = 40;
	auto font			= m_painter->createFonter< FontPainterKS0108 >( liberationMono11Bold );

	static const string page = "Page";
	string pageData			 = std::to_string( currentPageNo() + 1 );
	pageData += "/";
	pageData += std::to_string( pagesNo() );

	auto y = 2; //	( height - 0 - size.height ) / 2 + 1;

	// start: ------------------- Print page -------------------
	auto sizePage	 = font->getStringSize( page );
	auto sizePageData = font->getStringSize( pageData );
	// m_painter->drawFilledRectangle( 0, 0, m_epd->width() / 2, height, Color::Black );
	m_painter->drawHorizontalLine( 0, height - 1, m_epd->width(), Color::Black );
	font->drawString( m_epd->width() - sizePage.width - 2, y, page, Color::Black );
	font->drawString(
		m_epd->width() - sizePageData.width - 2, height - sizePageData.height - y, pageData, Color::Black );
	// end: --------------------- Print page -------------------

	// start: ------------------- Print description -------------------
	auto description = ( *m_currentPage )->getDescription( 0 );
	font->drawString( 2, y, description, Color::Black );
	// end: --------------------- Print description -------------------

	// start: ------------------- Print description2 -------------------
	description		  = ( *m_currentPage )->getDescription( 1 );
	auto autoDescSize = font->getStringSize( description );
	font->drawString(
		m_epd->width() / 2 - autoDescSize.width - 2, height - autoDescSize.height - y, description, Color::Black );
	// end: --------------------- Print description2 -------------------

	auto courierNew28Bold = m_painter->createFonter< FontPainterKS0108 >( fontEbrima28 );

	// start: ------------------- Temperature -------------------
	m_temperature->reset();
	m_humidit->reset();

	// Print temp
	if( m_temperature->isAvailable() )
	{
		char text[ 20 ];
		auto image = make_unique< ImageXX >( icons8temperature30 );
		m_painter->merge( m_epd->width() / 2, 5, move( image ) );

		auto data = m_temperature->getData();
		if( data.second )
		{
			sprintf( text, "%3.1f%cc", data.first, 0xB0 );
			courierNew28Bold->drawString( m_epd->width() / 2 + 30, 0, text, Color::Color1 );
		}
		else
		{
			courierNew28Bold->drawString( m_epd->width() / 2 + 30, 0, "err", Color::Color1 );
		}
	}

	// end: --------------------- Temperature -------------------

	// start: ------------------- humidity -------------------
	if( m_humidit->isAvailable() )
	{
		auto image = make_unique< ImageXX >( icons8humidity32 );
		m_painter->merge( m_epd->width() / 2 + 140, 5, move( image ) );

		char text[ 20 ];
		auto data = m_humidit->getData();
		if( data.second )
		{
			sprintf( text, "%3.1f%%", data.first );
			courierNew28Bold->drawString( m_epd->width() / 2 + 175, 0, text, Color::Color1 );
		}
		else
		{
			courierNew28Bold->drawString( m_epd->width() / 2 + 175, 0, "err", Color::Color1 );
		}
	}

	// end: --------------------- humidity -------------------

	return height;
}

void StatusManager::onTemperature( float temp )
{
	refreshPage();
}

void StatusManager::onHumidit( float humidit )
{
	refreshPage();
}

unsigned StatusManager::pagesNo() const
{
	unsigned pagesNo = 0;
	for( const auto& page : m_pages )
		pagesNo += page->pageNo();

	return pagesNo;
}

unsigned StatusManager::currentPageNo() const
{
	auto it			= m_pages.begin();
	unsigned pageNo = 0;

	while( it != m_currentPage )
	{
		pageNo += ( *m_currentPage )->pageNo();
		it++;
	}

	return pageNo + ( *m_currentPage )->currentPageNo();
}

// end: --------------------- StatusManager -------------------

// start: ------------------- StatusPage -------------------
bool StatusPage::init( std::function< void( size_t /*page*/ ) > refreshRequest )
{
	m_refresh = refreshRequest;
	return true;
}
// end: --------------------- StatusPage -------------------
