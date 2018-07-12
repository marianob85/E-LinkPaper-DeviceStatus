#pragma once
#include <cstdlib>
#include <mutex>

class SI7021
{
public:
	SI7021();
	virtual ~SI7021() = default;

	std::pair< float, bool > getTemp() const;
	std::pair< float, bool > gethumidity() const;

	static unsigned crc( uint8_t* data, size_t len );

private:
	static std::mutex m_refreshMutex;
};