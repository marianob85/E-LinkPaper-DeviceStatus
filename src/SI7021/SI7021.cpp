#include <thread>
#include <iostream>
#include <algorithm>
#include <pugixml.hpp>
#include "SI7021.hpp"
#include <i2c.h>
// start: ------------------- SI7021 -------------------

const uint8_t SI7021_DEFAULT_ADDRESS = 0x40;
const uint8_t SI7021_MEASRH_HOLD_CMD = 0xE5;
// const uint8_t SI7021_MEASRH_NOHOLD_CMD   = 0xF5;
const uint8_t SI7021_MEASTEMP_HOLD_CMD = 0xE3;
// const uint8_t SI7021_MEASTEMP_NOHOLD_CMD = 0xF3;
// const uint8_t SI7021_READPREVTEMP_CMD	= 0xE0;
const uint8_t SI7021_RESET_CMD = 0xFE;
// const uint8_t SI7021_WRITERHT_REG_CMD	= 0xE6;
// const uint8_t SI7021_READRHT_REG_CMD	 = 0xE7;
// const uint8_t SI7021_WRITEHEATER_REG_CMD = 0x51;
// const uint8_t SI7021_READHEATER_REG_CMD  = 0x11;
// const uint16_t SI7021_ID1_CMD			 = 0xFA0F;
// const uint16_t SI7021_ID2_CMD			 = 0xFCC9;
// const uint16_t SI7021_FIRMVERS_CMD		 = 0x84B8;

std::mutex SI7021::m_refreshMutex;

SI7021::SI7021( std::experimental::filesystem::path xmlPath )
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( xmlPath.string().c_str() );

	if( !result )
	{
		std::cout << "XML [" << xmlPath.string().c_str() << "] parsed with errors, attr value: ["
				  << doc.child( "node" ).attribute( "attr" ).value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..."
				  << ( xmlPath.string().c_str() + result.offset ) << "]\n\n";
		throw std::exception();
	}
	auto config  = doc.child( "Config" );
	auto ledNode = config.child( "Sensors" ).child( "SI7021" );

	m_dev = ledNode.text().as_string();
}

std::pair< float, bool > SI7021::getTemp() const
{
	if( m_dev.empty() )
		return { 0.f, false };

	std::lock_guard< std::mutex > lock( m_refreshMutex );

	int bus;
	if( ( bus = i2c_open( m_dev.c_str() ) ) == -1 )
	{
		return { 0.f, false };
	}

	I2CDevice device{};

	i2c_init_device( &device );

	/* 24C04 */
	device.bus		   = bus;					 /* Bus 0 */
	device.addr		   = SI7021_DEFAULT_ADDRESS; /* Slave address is 0x50, 7-bit */
	device.iaddr_bytes = 0;						 /* Device internal address is 1 byte */
	device.page_bytes  = 8;

	i2c_ioctl_write( &device, 0x0, ( char* )&SI7021_RESET_CMD, 1 );
	std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
	if( i2c_ioctl_write( &device, 0x0, ( char* )&SI7021_MEASTEMP_HOLD_CMD, 1 ) == 1 )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
		uint8_t buffer[ 3 ] = {};
		ssize_t size		= sizeof( buffer );

		/* From i2c 0x0 address read 256 bytes data to buffer */
		if( i2c_ioctl_read( &device, 0x0, buffer, size ) == size )
		{
			i2c_close( bus );

			return computeTemp( buffer );
		}
	}

	i2c_close( bus );
	return { 0.f, false };
}

std::pair< float, bool > SI7021::computeTemp( uint8_t data[ 3 ] ) const
{
	unsigned int temp = data[ 0 ];
	temp <<= 8;
	temp |= data[ 1 ];

	if( crc( data, 2 ) != data[ 2 ] || data[ 2 ] == 0 )
		return { 0, false };

	float temperature = float( temp );
	temperature *= 175.72f;
	temperature /= 65536.0f;
	temperature -= 46.85f;

	return { temperature, true };
}

std::pair< float, bool > SI7021::computeHumidity( uint8_t data[ 3 ] ) const
{
	unsigned int hum = data[ 0 ];
	hum <<= 8;
	hum |= data[ 1 ];

	if( crc( data, 2 ) != data[ 2 ] || data[ 2 ] == 0 )
		return { 0, false };

	float humidity = float( hum );
	humidity *= 125.0f;
	humidity /= 65536.0f;
	humidity -= 6.0f;

	return { std::min( 100.0f, humidity ), true };
}

std::pair< float, bool > SI7021::gethumidity() const
{
	if( m_dev.empty() )
		return { 0.f, false };

	std::lock_guard< std::mutex > lock( m_refreshMutex );

	int bus;
	if( ( bus = i2c_open( m_dev.c_str() ) ) == -1 )
	{
		return { 0.f, false };
	}

	I2CDevice device{};

	i2c_init_device( &device );

	/* 24C04 */
	device.bus		   = bus;					 /* Bus 0 */
	device.addr		   = SI7021_DEFAULT_ADDRESS; /* Slave address is 0x50, 7-bit */
	device.iaddr_bytes = 0;						 /* Device internal address is 1 byte */
	device.page_bytes  = 8;
	i2c_ioctl_write( &device, 0x0, ( char* )&SI7021_RESET_CMD, 1 );
	std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
	if( i2c_ioctl_write( &device, 0x0, ( char* )&SI7021_MEASRH_HOLD_CMD, 1 ) == 1 )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
		uint8_t buffer[ 3 ] = {};
		ssize_t size		= sizeof( buffer );

		/* From i2c 0x0 address read 256 bytes data to buffer */
		if( i2c_ioctl_read( &device, 0x0, buffer, size ) == size )
		{
			i2c_close( bus );

			return computeHumidity( buffer );
		}
	}

	i2c_close( bus );
	return { 0.f, false };
}

unsigned SI7021::crc( uint8_t* data, size_t len )
{
	unsigned crc = 0;
	for( uint8_t index = 0; index < len; ++index )
	{
		crc ^= *( data + index );
		for( uint8_t i = 0; i < 8; ++i )
		{
			if( crc & 0x80 )
			{
				crc <<= 1;
				crc ^= 0x131;
			}
			else
			{
				crc <<= 1;
			}
		}
	}
	return crc;
}

// end: --------------------- SI7021 -------------------
