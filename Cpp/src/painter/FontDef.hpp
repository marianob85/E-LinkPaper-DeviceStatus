#pragma once
#include <cstdint>

extern const unsigned short Georgia15x15[];

struct FontData
{
	const unsigned short* chars;
	const uint8_t width;
	const uint8_t height;
};


