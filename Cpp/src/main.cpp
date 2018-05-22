//#include <memory>
//#include <stdio.h>
//#include <stdlib.h>
//#include <epd4in2.h>
//#include <epdpaint.h>
#include <memory>
#include <chrono>
#include <thread>
#include "StatusManager.hpp"
#include "StatusPing.hpp"

using namespace std;

int main( void )
{
	StatusManager statusManager;

	if( !statusManager.init() )
		return -1;

	statusManager.add( make_unique< StatusPing >(), 60 );

	statusManager.setNext();

	//while( true )
		//std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

	// Epd4in2 epd( make_unique< EpdWiringPi >( CHANNEL ), RST_PIN, DC_PIN, CS_PIN, BUSY_PIN, EPD_WIDTH, EPD_HEIGHT );

	// if( epd.init() == false )
	//{
	//	printf( "e-Paper init failed\n" );
	//	return -1;
	//}

	//// unsigned char* frame_buffer = (unsigned char*)malloc(epd.width / 8 * epd.height);
	// auto frame_buffer = make_unique< uint8_t[] >( epd.width() / 8 * epd.height() );

	// Paint paint( frame_buffer.get(), epd.width(), epd.height() );
	// paint.Clear( UNCOLORED );

	///* Draw something to the frame_buffer */
	///* For simplicity, the arguments are explicit numerical coordinates */
	// paint.DrawRectangle( 20, 80, 180, 280, COLORED );
	// paint.DrawLine( 20, 80, 180, 280, COLORED );
	// paint.DrawLine( 180, 80, 20, 280, COLORED );
	// paint.DrawFilledRectangle( 200, 80, 360, 280, COLORED );
	// paint.DrawCircle( 300, 160, 60, UNCOLORED );
	// paint.DrawFilledCircle( 90, 210, 30, COLORED );

	///*Write strings to the buffer */
	// paint.DrawFilledRectangle( 0, 6, 400, 30, COLORED );
	// paint.DrawStringAt( 100, 10, "Hello world!", &Font12, UNCOLORED );
	// paint.DrawStringAt( 100, 40, "e-Paper Demo", &Font12, COLORED );

	///* Display the frame_buffer */
	// epd.displayFrame( frame_buffer.get() );

	///* Display the image buffer */
	//// epd.displayFrame(IMAGE_BUTTERFLY);
	return 0;
}
