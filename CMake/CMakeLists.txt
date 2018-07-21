# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.7)

# Set the project name
project (eLinkDisplayStatus)

set(_build_version "unknown")
set(_branch_version "unknown")
set(CPACK_PACKAGE_NAME "eLinkDisplayStatus")
find_package(Git)

if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    WORKING_DIRECTORY "${local_dir}"
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
    WORKING_DIRECTORY "${local_dir}"
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

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wno-unused-function -Wno-switch-bool -Wno-sign-conversion -Wno-conversion -Wno-switch-bool -Wno-self-assign)
add_compile_options($<$<CONFIG:RELEASE>:-O3>)
set_source_files_properties( ../src/lib/wiringNPi/wiringPi.c PROPERTIES COMPILE_FLAGS "-Wno-int-to-pointer-cast -Wno-pointer-to-int-cast" )

############################################################
# Create an executable
############################################################

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
		../src/StatusLed.cpp 
		../src/StatusManager.cpp 
		../src/StatusPing.cpp 
		../src/main.cpp 
		../src/InfluxWriter.cpp
		../src/DataProvider.cpp 
		../src/DS18B20/DS18B20.cpp 
		../src/e-link/epdWiringPi.cpp 
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
		../src/lib/libi2c/i2c.c
		../src/lib/influxdb.cpp
		../src/lib/wiringNPi/boardtype_friendlyelec.c
		../src/lib/wiringNPi/drcSerial.c
		../src/lib/wiringNPi/max5322.c
		../src/lib/wiringNPi/max31855.c
		../src/lib/wiringNPi/mcp23s08.c
		../src/lib/wiringNPi/mcp23s17.c
		../src/lib/wiringNPi/mcp3002.c
		../src/lib/wiringNPi/mcp3004.c
		../src/lib/wiringNPi/mcp3422.c
		../src/lib/wiringNPi/mcp4802.c
		../src/lib/wiringNPi/mcp23008.c
		../src/lib/wiringNPi/mcp23016.c
		../src/lib/wiringNPi/mcp23017.c
		../src/lib/wiringNPi/pcf8574.c
		../src/lib/wiringNPi/pcf8591.c
		../src/lib/wiringNPi/piHiPri.c
		../src/lib/wiringNPi/piThread.c
		../src/lib/wiringNPi/sn3218.c
		../src/lib/wiringNPi/softPwm.c
		../src/lib/wiringNPi/softServo.c
		../src/lib/wiringNPi/softTone.c
		../src/lib/wiringNPi/sr595.c
		../src/lib/wiringNPi/wiringPi.c
		../src/lib/wiringNPi/wiringPiI2C.c
		../src/lib/wiringNPi/wiringPiSPI.c
		../src/lib/wiringNPi/wiringSerial.c
		../src/lib/wiringNPi/wiringShift.c
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
		${PROJECT_SOURCE_DIR}/../src/lib/libi2c
		${PROJECT_SOURCE_DIR}/../src/lib/wiringNPi
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
	FILES ../E-LinkStatusConfig.xml
	DESTINATION etc)

configure_file("Package/eLinkDisplayStatus.service.cmake" "Package/eLinkDisplayStatus.service")

install (
	FILES Package/eLinkDisplayStatus.service
	DESTINATION /lib/systemd/system
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install (
	CODE
	"
	execute_process(
		COMMAND systemctl daemon-reload
		COMMAND systemctl enable eLinkDisplayStatus.service
		COMMAND systemctl start eLinkDisplayStatus.service
	)
	"
)


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

set( CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/Package/postinst;${CMAKE_CURRENT_SOURCE_DIR}/Package/prerm;${CMAKE_CURRENT_SOURCE_DIR}/Package/postrm" )

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE armhf)
message(STATUS "${CPACK_PACKAGE_NAME}")

set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${_branch_version}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

# Include CPack
include(CPack)
