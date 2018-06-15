#pragma once
#include <cstdint>

struct FontData
{
	const unsigned short* chars;
	const uint8_t width;
	const uint8_t height;
	const char startCharacter;
	const unsigned space; // Character space
};
