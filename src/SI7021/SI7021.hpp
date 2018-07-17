#pragma once
#include <cstdlib>
#include <mutex>

class SI7021
{
public:
	virtual ~SI7021() = default;

	std::pair< float, bool > getTemp() const;
	std::pair< float, bool > gethumidity() const;

	static unsigned crc( uint8_t* data, size_t len );

protected:
	std::pair< float, bool > computeTemp( uint8_t data[ 3 ] ) const;
	std::pair< float, bool > computeHumidity( uint8_t data[ 3 ] ) const;

private:
	static std::mutex m_refreshMutex;
};