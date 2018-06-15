#pragma once
#include <memory>
#include <PainterFont.hpp>
#include <FontDef.hpp>

// Display orientation
enum class Rotate
{
	Rotate0,
	Rotate90,
	Rotate180,
	Rotate270,
};

enum class Color
{
	White,
	Black,
	Color1,
};

class Paint
{
	friend class FontPainter;

public:
	Paint( size_t width, size_t height );
	Paint( Paint& paint );
	Paint( Paint&& ) = default;
	virtual ~Paint();

	Paint& operator=( const Paint& right );
	operator const uint8_t*() const;

	void clear( bool colored );
	size_t getWidth( void );
	size_t getHeight( void );
	Rotate getRotate( void );
	void setRotate( Rotate rotate );
	void drawPixel( size_t x, size_t y, bool colored );
	void drawLine( size_t x0, size_t y0, size_t x1, size_t y1, bool colored );
	void drawHorizontalLine( size_t x, size_t y, size_t width, bool colored );
	void drawVerticalLine( size_t x, size_t y, size_t height, bool colored );
	void drawRectangle( size_t x0, size_t y0, size_t x1, size_t y1, bool colored );
	void drawFilledRectangle( size_t x0, size_t y0, size_t x1, size_t y1, bool colored );
	void drawCircle( size_t x, size_t y, size_t radius, bool colored );
	void drawFilledCircle( size_t x, size_t y, size_t radius, bool colored );
	bool merge( size_t offsetX, size_t offsetY, std::unique_ptr< Paint > painter );

	template< class Fonter >
	std::shared_ptr< FontPainter > createFonter( FontData fontData )
	{
		return std::make_shared< Fonter >(
			fontData,
			std::bind( &Paint::drawPixel, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	}

protected:
	virtual void drawAbsolutePixel( size_t x, size_t y, bool colored ) = 0;
	virtual bool getAbsolutePixel( size_t x, size_t y ) const		   = 0;

protected:
	std::unique_ptr< uint8_t[] > m_image;
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
	virtual void drawAbsolutePixel( size_t x, size_t y, bool colored );
	virtual bool getAbsolutePixel( size_t x, size_t y ) const;
};