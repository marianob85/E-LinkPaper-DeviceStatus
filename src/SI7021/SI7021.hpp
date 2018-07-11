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
	const uint8_t SI7021_DEFAULT_ADDRESS	 = 0x40;
	const uint8_t SI7021_MEASRH_HOLD_CMD	 = 0xE5;
	const uint8_t SI7021_MEASRH_NOHOLD_CMD   = 0xF5;
	const uint8_t SI7021_MEASTEMP_HOLD_CMD   = 0xE3;
	const uint8_t SI7021_MEASTEMP_NOHOLD_CMD = 0xF3;
	const uint8_t SI7021_READPREVTEMP_CMD	= 0xE0;
	const uint8_t SI7021_RESET_CMD			 = 0xFE;
	const uint8_t SI7021_WRITERHT_REG_CMD	= 0xE6;
	const uint8_t SI7021_READRHT_REG_CMD	 = 0xE7;
	const uint8_t SI7021_WRITEHEATER_REG_CMD = 0x51;
	const uint8_t SI7021_READHEATER_REG_CMD  = 0x11;
	const uint16_t SI7021_ID1_CMD			 = 0xFA0F;
	const uint16_t SI7021_ID2_CMD			 = 0xFCC9;
	const uint16_t SI7021_FIRMVERS_CMD		 = 0x84B8;

	static std::mutex m_refreshMutex;
};