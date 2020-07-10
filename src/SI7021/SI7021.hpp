#pragma once
#include <cstdlib>
#include <mutex>
#include <experimental/filesystem>

class SI7021
{
public:
	SI7021( std::experimental::filesystem::path xmlPath );
	virtual ~SI7021() = default;

	std::pair< float, bool > getTemp() const;
	std::pair< float, bool > gethumidity() const;

	static unsigned crc( uint8_t* data, size_t len );

protected:
	std::pair< float, bool > computeTemp( uint8_t data[ 3 ] ) const;
	std::pair< float, bool > computeHumidity( uint8_t data[ 3 ] ) const;

private:
	static std::mutex m_refreshMutex;
	std::string m_dev;
	float m_hOffset{ 0 };
	float m_tOffset{ 0 };
};