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

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR 0

class Paint
{
	friend class FontPainter;

public:
	Paint( std::unique_ptr< uint8_t[] > image, int width, int height );
	Paint( Paint& paint );
	Paint( Paint&& ) = default;
	~Paint();

	Paint& operator=( const Paint& right );

	void clear( bool colored );
	int getWidth( void );
	int getHeight( void );
	Rotate getRotate( void );
	void setRotate( Rotate rotate );
	void drawAbsolutePixel( int x, int y, bool colored );
	bool getAbsolutePixel( unsigned x, unsigned y ) const;
	void drawPixel( int x, int y, bool colored );
	void drawLine( int x0, int y0, int x1, int y1, bool colored );
	void drawHorizontalLine( int x, int y, int width, bool colored );
	void drawVerticalLine( int x, int y, int height, bool colored );
	void drawRectangle( int x0, int y0, int x1, int y1, bool colored );
	void drawFilledRectangle( int x0, int y0, int x1, int y1, bool colored );
	void drawCircle( int x, int y, int radius, bool colored );
	void drawFilledCircle( int x, int y, int radius, bool colored );
	bool merge( unsigned offsetX, unsigned offsetY, const Paint& painter );

	uint8_t* rawImage() const;

	template< class Fonter >
	std::shared_ptr< FontPainter > createFonter( FontData fontData )
	{
		return std::make_shared< Fonter >(
			fontData, std::bind( &Paint::drawPixel, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	}

private:
	std::unique_ptr< uint8_t[] > m_image;
	int m_width{ 0 };
	int m_height{ 0 };
	Rotate m_rotate{ Rotate::Rotate0 };
};
