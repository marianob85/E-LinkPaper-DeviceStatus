#pragma once
#include <memory>
#include <vector>
#include <mutex>
#include <experimental/filesystem>
#include <DS18B20.h>
#include <epd.hpp>
#include <Painter.hpp>
#include "Timer.hpp"

class StatusPage
{
public:
	virtual ~StatusPage() {}
	virtual std::unique_ptr< Paint > currentPage( size_t width, size_t height ) const = 0;
	virtual unsigned pageNo() const													  = 0;
	virtual unsigned currentPageNo() const											  = 0;
	virtual bool setPage( unsigned page )											  = 0;
	virtual bool setNext()															  = 0;
	virtual std::string getDescription( size_t index ) const						  = 0;
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
	void refreshRequest();
	void refreshPage();
	unsigned pagesNo() const;
	unsigned currentPageNo() const;

	size_t printHeader();
	size_t printHeader2();

private:
	using ContainerT = std::vector< std::unique_ptr< StatusPage > >;
	ContainerT m_pages;
	ContainerT::const_iterator m_currentPage;
	std::unique_ptr< Epd > m_epd;
	std::unique_ptr< Paint > m_painter;
	std::unique_ptr< DS18B20 > m_tempSensor;
	std::mutex m_refreshMutex;
};
