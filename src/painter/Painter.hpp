#pragma once
#include <vector>
#include <memory>
#include <PainterDef.hpp>
#include <PainterFont.hpp>
#include <FontDef.hpp>

class Paint
{
	friend class FontPainter;

public:
	Paint( size_t width, size_t height );
	Paint( Paint& paint ) = default;
	Paint( Paint&& )	  = default;
	virtual ~Paint();

	operator const uint8_t*() const;

	void clear( Color color );
	size_t getWidth( void );
	size_t getHeight( void );
	Rotate getRotate( void );
	void setRotate( Rotate rotate );
	void drawPixel( size_t x, size_t y, Color color );
	void drawLine( size_t x0, size_t y0, size_t x1, size_t y1, Color color );
	void drawHorizontalLine( size_t x, size_t y, size_t width, Color color );
	void drawVerticalLine( size_t x, size_t y, size_t height, Color color );
	void drawRectangle( size_t x0, size_t y0, size_t x1, size_t y1, Color color );
	void drawFilledRectangle( size_t x0, size_t y0, size_t x1, size_t y1, Color color );
	void drawCircle( size_t x, size_t y, size_t radius, Color color );
	void drawFilledCircle( size_t x, size_t y, size_t radius, Color color );
	bool merge( size_t offsetX, size_t offsetY, std::unique_ptr< Paint > painter );

	template< class Fonter >
	std::shared_ptr< FontPainter > createFonter( FontData fontData )
	{
		return std::make_shared< Fonter >(
			fontData,
			std::bind( &Paint::drawPixel, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	}

protected:
	virtual void drawAbsolutePixel( size_t x, size_t y, Color color ) = 0;
	virtual Color getAbsolutePixel( size_t x, size_t y ) const		  = 0;

protected:
	std::vector< uint8_t > m_image;
	size_t m_imageSize{ 0 };
	size_t m_width{ 0 };
	size_t m_height{ 0 };
	Rotate m_rotate{ Rotate::Rotate0 };
};

class Paint2Colors : public Paint
{
public:
	Paint2Colors( size_t width, size_t height );
	Paint2Colors( Paint2Colors& paint ) = default;
	Paint2Colors( Paint2Colors&& )		= default;
	virtual ~Paint2Colors();

protected:
	virtual void drawAbsolutePixel( size_t x, size_t y, Color color );
	virtual Color getAbsolutePixel( size_t x, size_t y ) const;
};