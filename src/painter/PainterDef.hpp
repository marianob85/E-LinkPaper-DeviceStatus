#pragma once
#include <cstddef>

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

class PaintMerger
{
public:
	virtual ~PaintMerger()								 = default;
	virtual Color getAbsolutePixel( size_t x, size_t y ) = 0;
	virtual size_t width() const						 = 0;
	virtual size_t height() const						 = 0;
};
