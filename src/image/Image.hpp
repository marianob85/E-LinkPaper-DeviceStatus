#pragma once
#include <cstdint>
#include <PainterDef.hpp>

struct Image
{
	const uint8_t* data;
	uint16_t width;
	uint16_t height;
	uint8_t dataSize;
};

class ImageXX : public PaintMerger
{
public:
	ImageXX( Image image );
	// start: ------------------- PaintMerger -------------------
	virtual Color getAbsolutePixel( size_t x, size_t y );
	virtual size_t width() const;
	virtual size_t height() const;
	// end: --------------------- PaintMerger -------------------
protected:
	const Image m_image;
};

extern const Image icons8humidity32;
extern const Image icons8temperature30;
extern const Image avatar;