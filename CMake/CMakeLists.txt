# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.7)

# Set the project name
project (eLinkDisplayStatus)

# set a project version
set (eLinkDisplayStatus_VERSION_MAJOR 1)
set (eLinkDisplayStatus_VERSION_MINOR 0)
set (eLinkDisplayStatus_VERSION_PATCH 0)
set (eLinkDisplayStatus_VERSION "${eLinkDisplayStatus_VERSION_MAJOR}.${eLinkDisplayStatus_VERSION_MINOR}.${eLinkDisplayStatus_VERSION_PATCH}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wno-unused-function -Wno-switch-bool)
add_compile_options($<$<CONFIG:RELEASE>:-O3>)
set_source_files_properties( ../src/lib/wiringPi/wiringPi/pseudoPins.c PROPERTIES COMPILE_FLAGS "-Wno-int-to-pointer-cast -Wno-pointer-to-int-cast" )

############################################################
# Create an executable
############################################################

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
		../src/StatusManager.cpp 
		../src/StatusPing.cpp 
		../src/main.cpp 
		../src/DataProvider.cpp 
		../src/DS18B20/DS18B20.cpp 
		../src/e-link/epdWiringPi.cpp 
		../src/e-link/epdBcm2835.cpp 
		../src/e-link/epd4in2.cpp 
		../src/e-link/epd7in5b.cpp 
		../src/e-link/epd.cpp 
		../src/fonts/KS0108.cpp 
		../src/fonts/KS0108/Arial28.cpp 
		../src/fonts/KS0108/Digital-7-36Bold.cpp 
		../src/fonts/KS0108/Ebrima28.cpp 
		../src/fonts/KS0108/Georgia12.cpp 
		../src/fonts/KS0108/Liberation_Mono12.cpp 
		../src/fonts/KS0108/Liberation_Mono11Bold.cpp 
		../src/fonts/KS0108/Liberation_Mono10Bold.cpp 
		../src/fonts/KS0108/Liberation_Mono10.cpp 
		../src/fonts/KS0108/Liberation_Mono9Bold.cpp 
		../src/fonts/KS0108/Liberation_Mono9.cpp 
		../src/fonts/KS0108/Liberation_Mono8.cpp 
		../src/lib/pugi/pugixml.cpp 
		../src/painter/PainterFont.cpp 
		../src/painter/Painter.cpp 
		../src/ping/ping.cpp 
		../src/image/Image.cpp 
		../src/image/Data/icons8-humidity-32.cpp 
		../src/image/Data/icons8-temperature-30.cpp 
		../src/SI7021/SI7021.cpp 
		../src/lib/wiringPi/wiringPi/wiringPi.c						
		../src/lib/wiringPi/wiringPi/wiringSerial.c 
		../src/lib/wiringPi/wiringPi/wiringShift.c				
		../src/lib/wiringPi/wiringPi/piHiPri.c
		../src/lib/wiringPi/wiringPi/piThread.c					
		../src/lib/wiringPi/wiringPi/wiringPiSPI.c
		../src/lib/wiringPi/wiringPi/wiringPiI2C.c				
		../src/lib/wiringPi/wiringPi/softPwm.c
		../src/lib/wiringPi/wiringPi/softTone.c					
		../src/lib/wiringPi/wiringPi/mcp23008.c
		../src/lib/wiringPi/wiringPi/mcp23016.c 
		../src/lib/wiringPi/wiringPi/mcp23017.c			
		../src/lib/wiringPi/wiringPi/mcp23s08.c
		../src/lib/wiringPi/wiringPi/mcp23s17.c					
		../src/lib/wiringPi/wiringPi/sr595.c							
		../src/lib/wiringPi/wiringPi/pcf8574.c
		../src/lib/wiringPi/wiringPi/pcf8591.c					
		../src/lib/wiringPi/wiringPi/mcp3002.c
		../src/lib/wiringPi/wiringPi/mcp3004.c
		../src/lib/wiringPi/wiringPi/mcp4802.c
		../src/lib/wiringPi/wiringPi/mcp3422.c			
		../src/lib/wiringPi/wiringPi/max31855.c
		../src/lib/wiringPi/wiringPi/max5322.c
		../src/lib/wiringPi/wiringPi/ads1115.c				
		../src/lib/wiringPi/wiringPi/sn3218.c						
		../src/lib/wiringPi/wiringPi/bmp180.c
		../src/lib/wiringPi/wiringPi/htu21d.c
		../src/lib/wiringPi/wiringPi/ds18b20.c
		../src/lib/wiringPi/wiringPi/rht03.c			
		../src/lib/wiringPi/wiringPi/drcSerial.c
		../src/lib/wiringPi/wiringPi/drcNet.c					
		../src/lib/wiringPi/wiringPi/pseudoPins.c						
		../src/lib/wiringPi/wiringPi/wpiExtensions.c 
		../src/lib/bcm2835/bcm2835.c 
)

# Add an executable with the above sources
add_executable(eLinkDisplayStatus ${SOURCES})

target_link_libraries(${PROJECT_NAME} stdc++fs)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} crypt)
target_link_libraries(${PROJECT_NAME} rt)


# Set the direcoties that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(eLinkDisplayStatus
    PRIVATE 
		${PROJECT_SOURCE_DIR}/../src/DS18B20
		${PROJECT_SOURCE_DIR}/../src/e-link
		${PROJECT_SOURCE_DIR}/../src/fonts
		${PROJECT_SOURCE_DIR}/../src/image
		${PROJECT_SOURCE_DIR}/../src/painter
		${PROJECT_SOURCE_DIR}/../src/ping
		${PROJECT_SOURCE_DIR}/../src/SI7021
		${PROJECT_SOURCE_DIR}/../src/lib
		${PROJECT_SOURCE_DIR}/../src/lib/pugi
		${PROJECT_SOURCE_DIR}/../src/lib/bcm2835
		${PROJECT_SOURCE_DIR}/../src/lib/wiringPi
		${PROJECT_SOURCE_DIR}/../src/lib/wiringPi/wiringPi
		${PROJECT_SOURCE_DIR}/../src/lib/wiringPi/wiringPiD
)

############################################################
# Install
############################################################

# Binaries
install (TARGETS eLinkDisplayStatus
    DESTINATION bin)

# Config
install (FILES ../E-LinkStatusConfig.xml
    DESTINATION etc)

############################################################
# Create DEB
############################################################

# Tell CPack to generate a .deb package
set(CPACK_GENERATOR "DEB")

# Set a Package Maintainer.
# This is required
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Mariusz Brzeski")

# Set a Package Version
set(CPACK_PACKAGE_VERSION ${eLinkDisplayStatus_VERSION})

# Include CPack
include(CPack)

