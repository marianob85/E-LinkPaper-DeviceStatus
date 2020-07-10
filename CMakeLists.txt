# Supported Platfrom: RaspberryPi/NanoPiNeo
# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.17)

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

set( CMAKE_CXX_VISIBILITY_PRESET hidden )
set( CMAKE_VISIBILITY_INLINES_HIDDEN ON )

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wno-psabi -Wno-unused-function -Wno-switch-bool -Wno-sign-conversion -Wno-conversion -Wno-switch-bool -Wno-unused-result)
set_source_files_properties( src/lib/wiringPi/wiringPi/pseudoPins.c PROPERTIES COMPILE_FLAGS "-Wno-int-to-pointer-cast -Wno-pointer-to-int-cast" )
set_source_files_properties( src/lib/wiringNPi/wiringPi.c PROPERTIES COMPILE_FLAGS "-Wno-int-to-pointer-cast -Wno-pointer-to-int-cast" )
add_compile_options($<$<CONFIG:RELEASE>:-O3>)

############################################################
# Create an library
############################################################
add_library( wiringPI_library STATIC )

if( PlatformType STREQUAL "RaspberryPi" )
  target_sources( wiringPI_library PRIVATE
    src/lib/wiringPi/wiringPi/ads1115.c
    src/lib/wiringPi/wiringPi/bmp180.c
    src/lib/wiringPi/wiringPi/drcNet.c
    src/lib/wiringPi/wiringPi/drcSerial.c
    src/lib/wiringPi/wiringPi/ds18b20.c
    src/lib/wiringPi/wiringPi/htu21d.c
    src/lib/wiringPi/wiringPi/max31855.c
    src/lib/wiringPi/wiringPi/max5322.c
    src/lib/wiringPi/wiringPi/mcp23008.c
    src/lib/wiringPi/wiringPi/mcp23016.c
    src/lib/wiringPi/wiringPi/mcp23017.c
    src/lib/wiringPi/wiringPi/mcp23s08.c
    src/lib/wiringPi/wiringPi/mcp23s17.c
    src/lib/wiringPi/wiringPi/mcp3002.c
    src/lib/wiringPi/wiringPi/mcp3004.c
    src/lib/wiringPi/wiringPi/mcp3422.c
    src/lib/wiringPi/wiringPi/mcp4802.c
    src/lib/wiringPi/wiringPi/pcf8574.c
    src/lib/wiringPi/wiringPi/pcf8591.c
    src/lib/wiringPi/wiringPi/piHiPri.c
    src/lib/wiringPi/wiringPi/piThread.c
    src/lib/wiringPi/wiringPi/pseudoPins.c
    src/lib/wiringPi/wiringPi/rht03.c
    src/lib/wiringPi/wiringPi/sn3218.c
    src/lib/wiringPi/wiringPi/softPwm.c
    src/lib/wiringPi/wiringPi/softServo.c
    src/lib/wiringPi/wiringPi/softTone.c
    src/lib/wiringPi/wiringPi/sr595.c
    src/lib/wiringPi/wiringPi/wiringPi.c
    src/lib/wiringPi/wiringPi/wiringPiI2C.c
    src/lib/wiringPi/wiringPi/wiringPiSPI.c
    src/lib/wiringPi/wiringPi/wiringSerial.c
    src/lib/wiringPi/wiringPi/wiringShift.c
  )
  target_include_directories(wiringPI_library PUBLIC
    src/lib/wiringPi
    src/lib/wiringPi/wiringPi
    src/lib/wiringPi/wiringPiD
  )
elseif( PlatformType STREQUAL "NanoPiNeo" )
  target_sources( wiringPI_library PRIVATE
    src/lib/wiringNPi/boardtype_friendlyelec.c
    src/lib/wiringNPi/drcSerial.c
    src/lib/wiringNPi/max31855.c
    src/lib/wiringNPi/max5322.c
    src/lib/wiringNPi/mcp23008.c
    src/lib/wiringNPi/mcp23016.c
    src/lib/wiringNPi/mcp23017.c
    src/lib/wiringNPi/mcp23s08.c
    src/lib/wiringNPi/mcp23s17.c
    src/lib/wiringNPi/mcp3002.c
    src/lib/wiringNPi/mcp3004.c
    src/lib/wiringNPi/mcp3422.c
    src/lib/wiringNPi/mcp4802.c
    src/lib/wiringNPi/pcf8574.c
    src/lib/wiringNPi/pcf8591.c
    src/lib/wiringNPi/piHiPri.c
    src/lib/wiringNPi/piThread.c
    src/lib/wiringNPi/sn3218.c
    src/lib/wiringNPi/softPwm.c
    src/lib/wiringNPi/softServo.c
    src/lib/wiringNPi/softTone.c
    src/lib/wiringNPi/sr595.c
    src/lib/wiringNPi/wiringPi.c
    src/lib/wiringNPi/wiringPiI2C.c
    src/lib/wiringNPi/wiringPiSPI.c
    src/lib/wiringNPi/wiringSerial.c
    src/lib/wiringNPi/wiringShift.c
  )
  target_include_directories(wiringPI_library PUBLIC
    src/lib/wiringNPi
  )
else()
    message( FATAL_ERROR "PlatformType: ${PlatformType} not supported" )    
endif()

############################################################
# Create an executable
############################################################

# Add an executable with the above sources
add_executable( eLinkDisplayStatus 
  src/gpioPooling.cpp
  src/StatusLed.cpp 
  src/StatusManager.cpp 
  src/StatusPing.cpp 
  src/main.cpp 
  src/InfluxWriter.cpp
  src/DataProvider.cpp 
  src/DS18B20/DS18B20.cpp 
  src/e-link/epdWiringPi.cpp 
  src/e-link/epd4in2.cpp 
  src/e-link/epd7in5b.cpp 
  src/e-link/epd.cpp 
  src/fonts/KS0108.cpp 
  src/fonts/KS0108/Arial28.cpp 
  src/fonts/KS0108/Digital-7-36Bold.cpp 
  src/fonts/KS0108/Ebrima28.cpp 
  src/fonts/KS0108/Georgia12.cpp 
  src/fonts/KS0108/Liberation_Mono12.cpp 
  src/fonts/KS0108/Liberation_Mono11Bold.cpp 
  src/fonts/KS0108/Liberation_Mono10Bold.cpp 
  src/fonts/KS0108/Liberation_Mono10.cpp 
  src/fonts/KS0108/Liberation_Mono9Bold.cpp 
  src/fonts/KS0108/Liberation_Mono9.cpp 
  src/fonts/KS0108/Liberation_Mono8.cpp 
  src/painter/PainterFont.cpp 
  src/painter/Painter.cpp 
  src/ping/ping.cpp 
  src/image/Image.cpp 
  src/image/Data/icons8-humidity-32.cpp 
  src/image/Data/icons8-temperature-30.cpp 
  src/image/Data/avatar.cpp 
  src/SI7021/SI7021.cpp 
  src/lib/libi2c/i2c.c
  src/lib/influxDB/influxdb.cpp
  src/lib/pugi/pugixml.cpp
  src/Utils/Task.cpp
)

target_include_directories( eLinkDisplayStatus PRIVATE 
		src/DS18B20
		src/e-link
		src/fonts
		src/image
		src/painter
		src/ping
		src/SI7021
		src/lib/pugi
		src/lib/libi2c
    src/lib/influxDB
    src/Utils
)

target_link_libraries( eLinkDisplayStatus 
	wiringPI_library 
	stdc++fs
	pthread
	crypt
	rt
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
