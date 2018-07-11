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
	void drawPixel( int x, int y, Color color );
	void drawLine( int x0, int y0, int x1, int y1, Color color );
	void drawHorizontalLine( int x, int y, size_t width, Color color );
	void drawVerticalLine( int x, int y, size_t height, Color color );
	void drawRectangle( int x0, int y0, int x1, int y1, Color color );
	void drawFilledRectangle( int x0, int y0, int x1, int y1, Color color );
	void drawCircle( int x, int y, size_t radius, Color color );
	void drawFilledCircle( int x, int y, size_t radius, Color color );
	bool merge( int offsetX, int offsetY, std::unique_ptr< Paint > painter );
	bool merge( int offsetX, int offsetY, std::unique_ptr< PaintMerger > mergeObject );

	template< class Fonter >
	std::shared_ptr< FontPainter > createFonter( FontData fontData )
	{
		return std::make_shared< Fonter >(
			fontData,
			std::bind( &Paint::drawPixel, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	}

protected:
	virtual void drawAbsolutePixel( int x, int y, Color color ) = 0;
	virtual Color getAbsolutePixel( int x, int y ) const		  = 0;

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
	virtual void drawAbsolutePixel( int x, int y, Color color );
	virtual Color getAbsolutePixel( int x, int y ) const;
};

class Paint3Colors : public Paint
{
public:
	Paint3Colors( size_t width, size_t height );
	Paint3Colors( Paint3Colors& paint ) = default;
	Paint3Colors( Paint3Colors&& )		= default;
	virtual ~Paint3Colors();

protected:
	virtual void drawAbsolutePixel( int x, int y, Color color );
	virtual Color getAbsolutePixel( int x, int y ) const;
};