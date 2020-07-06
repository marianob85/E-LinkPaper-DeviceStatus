# Supported Platfrom: RaspberryPi/NanoPiNeo
# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.7)

# Set the project name
project (eLinkDisplayStatus)

if (NOT DEFINED PlatformType)
  set( PlatformType "RaspberryPi")
endif()
message( STATUS "Platform type: ${PlatformType}" )

set(_build_version "unknown")
set(_branch_version "unknown")
set(CPACK_PACKAGE_NAME "eLinkDisplayStatus")
find_package(Git)

if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    OUTPUT_VARIABLE _build_version
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message( STATUS "GIT hash: ${_build_version}")
else()
  message(STATUS "GIT not found")
endif()

if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --all --exact-match COMMAND sed "s=.*/=="
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    OUTPUT_VARIABLE _branch_version
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message( STATUS "GIT branch: ${_branch_version}")
endif()

# set a project version
set (eLinkDisplayStatus_VERSION_MAJOR 1)
set (eLinkDisplayStatus_VERSION_MINOR 0)
set (eLinkDisplayStatus_VERSION_PATCH ${_build_version})
set (eLinkDisplayStatus_VERSION "${eLinkDisplayStatus_VERSION_MAJOR}.${eLinkDisplayStatus_VERSION_MINOR}.${eLinkDisplayStatus_VERSION_PATCH}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wno-psabi -Wno-dev -Wno-unused-function -Wno-switch-bool -Wno-sign-conversion -Wno-conversion -Wno-switch-bool -Wno-unused-result)
set_source_files_properties( src/lib/wiringPi/wiringPi/pseudoPins.c PROPERTIES COMPILE_FLAGS "-Wno-int-to-pointer-cast -Wno-pointer-to-int-cast" )
set_source_files_properties( src/lib/wiringNPi/wiringPi.c PROPERTIES COMPILE_FLAGS "-Wno-int-to-pointer-cast -Wno-pointer-to-int-cast" )
add_compile_options($<$<CONFIG:RELEASE>:-O3>)

############################################################
# Create an library
############################################################

if( PlatformType STREQUAL "RaspberryPi" )
    set( WIRING_SOURCES 
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/ads1115.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/bmp180.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/drcNet.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/drcSerial.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/ds18b20.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/htu21d.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/max31855.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/max5322.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/mcp23008.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/mcp23016.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/mcp23017.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/mcp23s08.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/mcp23s17.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/mcp3002.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/mcp3004.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/mcp3422.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/mcp4802.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/pcf8574.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/pcf8591.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/piHiPri.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/piThread.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/pseudoPins.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/rht03.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/sn3218.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/softPwm.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/softServo.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/softTone.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/sr595.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/wiringPi.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/wiringPiI2C.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/wiringPiSPI.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/wiringSerial.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi/wiringShift.c
    )
    set( WIRING_INCLUDES
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPi
        ${PROJECT_SOURCE_DIR}/src/lib/wiringPi/wiringPiD
    )
elseif( PlatformType STREQUAL "NanoPiNeo" )
    set( WIRING_SOURCES 
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/boardtype_friendlyelec.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/drcSerial.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/max31855.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/max5322.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/mcp23008.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/mcp23016.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/mcp23017.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/mcp23s08.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/mcp23s17.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/mcp3002.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/mcp3004.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/mcp3422.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/mcp4802.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/pcf8574.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/pcf8591.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/piHiPri.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/piThread.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/sn3218.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/softPwm.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/softServo.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/softTone.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/sr595.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/wiringPi.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/wiringPiI2C.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/wiringPiSPI.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/wiringSerial.c
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi/wiringShift.c
    )
    set( WIRING_INCLUDES
        ${PROJECT_SOURCE_DIR}/src/lib/wiringNPi
    )
else()
    message( FATAL_ERROR "PlatformType: ${PlatformType} not supported" )    
endif()

add_library(wiringPI_library STATIC ${WIRING_SOURCES} )
target_include_directories(wiringPI_library PUBLIC ${WIRING_INCLUDES} )

############################################################
# Create an executable
############################################################

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
		${PROJECT_SOURCE_DIR}/src/StatusLed.cpp 
		${PROJECT_SOURCE_DIR}/src/StatusManager.cpp 
		${PROJECT_SOURCE_DIR}/src/StatusPing.cpp 
		${PROJECT_SOURCE_DIR}/src/main.cpp 
		${PROJECT_SOURCE_DIR}/src/InfluxWriter.cpp
		${PROJECT_SOURCE_DIR}/src/DataProvider.cpp 
		${PROJECT_SOURCE_DIR}/src/DS18B20/DS18B20.cpp 
		${PROJECT_SOURCE_DIR}/src/e-link/epdWiringPi.cpp 
		${PROJECT_SOURCE_DIR}/src/e-link/epd4in2.cpp 
		${PROJECT_SOURCE_DIR}/src/e-link/epd7in5b.cpp 
		${PROJECT_SOURCE_DIR}/src/e-link/epd.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Arial28.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Digital-7-36Bold.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Ebrima28.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Georgia12.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Liberation_Mono12.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Liberation_Mono11Bold.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Liberation_Mono10Bold.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Liberation_Mono10.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Liberation_Mono9Bold.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Liberation_Mono9.cpp 
		${PROJECT_SOURCE_DIR}/src/fonts/KS0108/Liberation_Mono8.cpp 
		${PROJECT_SOURCE_DIR}/src/painter/PainterFont.cpp 
		${PROJECT_SOURCE_DIR}/src/painter/Painter.cpp 
		${PROJECT_SOURCE_DIR}/src/ping/ping.cpp 
		${PROJECT_SOURCE_DIR}/src/image/Image.cpp 
		${PROJECT_SOURCE_DIR}/src/image/Data/icons8-humidity-32.cpp 
		${PROJECT_SOURCE_DIR}/src/image/Data/icons8-temperature-30.cpp 
		${PROJECT_SOURCE_DIR}/src/SI7021/SI7021.cpp 
		${PROJECT_SOURCE_DIR}/src/lib/libi2c/i2c.c
		${PROJECT_SOURCE_DIR}/src/lib/influxDB/influxdb.cpp
		${PROJECT_SOURCE_DIR}/src/lib/pugi/pugixml.cpp 
)

# Add an executable with the above sources
add_executable(eLinkDisplayStatus ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
	wiringPI_library 
	stdc++fs
	pthread
	crypt
	rt
)

# Set the direcoties that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(eLinkDisplayStatus
    PRIVATE 
		${PROJECT_SOURCE_DIR}/src/DS18B20
		${PROJECT_SOURCE_DIR}/src/e-link
		${PROJECT_SOURCE_DIR}/src/fonts
		${PROJECT_SOURCE_DIR}/src/image
		${PROJECT_SOURCE_DIR}/src/painter
		${PROJECT_SOURCE_DIR}/src/ping
		${PROJECT_SOURCE_DIR}/src/SI7021
		${PROJECT_SOURCE_DIR}/src/lib/pugi
		${PROJECT_SOURCE_DIR}/src/lib/libi2c
		${PROJECT_SOURCE_DIR}/src/lib/wiringPi
		${PROJECT_SOURCE_DIR}/src/lib/influxDB
)

############################################################
# Install
############################################################

# Binaries
install (
	TARGETS eLinkDisplayStatus
	DESTINATION bin)

# Config
install (
	FILES cmake/deb/E-LinkStatusConfig.xml
	DESTINATION etc)

configure_file("${PROJECT_SOURCE_DIR}/cmake/deb/eLinkDisplayStatus.service.cmake" "${PROJECT_SOURCE_DIR}/cmake/deb/eLinkDisplayStatus.service")

install (
	FILES cmake/deb/eLinkDisplayStatus.service
	DESTINATION /lib/systemd/system
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)


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
set(CPACK_DEBIAN_PACKAGE_DEPENDS " ")
set( CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/cmake/deb/postinst;${CMAKE_CURRENT_SOURCE_DIR}/cmake/deb/prerm;${CMAKE_CURRENT_SOURCE_DIR}/cmake/deb/postrm" )

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE armhf)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${_branch_version}_${PlatformType}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
# Include CPack
include(CPack)
