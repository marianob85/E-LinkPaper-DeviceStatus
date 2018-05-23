#pragma once
#include <memory>
#include <vector>
#include <epdpaint.h>
#include <epd4in2.h>
#include "Timer.hpp"
#include <DS18B20.h>

class StatusPage
{
public:
	virtual bool init( unsigned width, unsigned height );
	virtual ~StatusPage() {}
	virtual Paint currentPage() const		   = 0;
	virtual unsigned pageNo() const			   = 0;
	virtual unsigned currentPageNo() const	 = 0;
	virtual bool setPage( unsigned page )	  = 0;
	virtual bool setNext()					   = 0;
	virtual std::string getDescription() const = 0;

protected:
	unsigned m_height{ 0 };
	unsigned m_width{ 0 };
};

class StatusManager
{
public:
	StatusManager();
	virtual ~StatusManager();

	bool init();

	bool add( std::unique_ptr< StatusPage > statusPage, unsigned intervalSeconds );
	void setNext();
	bool setPage( unsigned page, unsigned subPage );
	void autoChange( bool set );

private:
	void refreshPage();
	void printHeader();
	unsigned pagesNo() const;
	unsigned currentPageNo() const;
	void onTimer();

private:
	static const unsigned s_headeWHeight = 20;

	using ContainerT = std::vector< std::pair< std::unique_ptr< StatusPage >, unsigned > >;
	ContainerT m_pages;
	ContainerT::const_iterator m_currentPage;
	Epd4in2 m_epd;
	std::unique_ptr< Paint > m_painter;
	TimerEvent m_timerEvent;
	std::unique_ptr< DS18B20 > m_tempSensor;
};
