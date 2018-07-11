#pragma once
#include <memory>
#include <vector>
#include <mutex>
#include <experimental/filesystem>
#include <epd.hpp>
#include <Painter.hpp>
#include "Timer.hpp"
#include "DataProvider.hpp"

class StatusPage
{
public:
	virtual ~StatusPage() {}
	virtual bool init( std::function< void( size_t /*page*/ ) > refreshRequest );
	virtual std::unique_ptr< Paint > currentPage( size_t width, size_t height ) const = 0;
	virtual unsigned pageNo() const													  = 0;
	virtual unsigned currentPageNo() const											  = 0;
	virtual bool setPage( unsigned page )											  = 0;
	virtual bool setNext()															  = 0;
	virtual std::string getDescription( size_t index ) const						  = 0;

protected:
	std::function< void( size_t /*page*/ ) > m_refresh;
};

class StatusManager
{
public:
	StatusManager( std::experimental::filesystem::path xmlPath );
	virtual ~StatusManager();

	bool init();

	bool add( std::unique_ptr< StatusPage > statusPage );
	void setNext();
	bool setPage( unsigned page, unsigned subPage );

	void close();

private:
	void requestPageRefresh( size_t page, size_t subPage );
	void refreshPage();
	unsigned pagesNo() const;
	unsigned currentPageNo() const;

	size_t printHeader();
	size_t printHeader2();

	void onTemperature( float temp );
	void onHumidit( float humidit );

private:
	using ContainerT = std::vector< std::unique_ptr< StatusPage > >;
	ContainerT m_pages;
	ContainerT::const_iterator m_currentPage;
	std::unique_ptr< Epd > m_epd;
	std::unique_ptr< Paint > m_painter;

	std::mutex m_refreshMutex;
	std::unique_ptr< TempProvider > m_temperature;
	std::unique_ptr< HumiditProvider > m_humidit;
};
