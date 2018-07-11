bin_PROGRAMS = eLinkDisplayStatus

eLinkDisplayStatus_CXXFLAGS = -I"src/DS18B20"
eLinkDisplayStatus_CXXFLAGS += -I"src/e-link"
eLinkDisplayStatus_CXXFLAGS += -I"src/fonts"
eLinkDisplayStatus_CXXFLAGS += -I"src/image"
eLinkDisplayStatus_CXXFLAGS += -I"src/painter"
eLinkDisplayStatus_CXXFLAGS += -I"src/ping"
eLinkDisplayStatus_CXXFLAGS += -I"src/SI7021"
eLinkDisplayStatus_CXXFLAGS += -I"src/lib/pugi"
eLinkDisplayStatus_CXXFLAGS += -I"src/lib/bcm2835"
eLinkDisplayStatus_CXXFLAGS += -I"src/lib/wiringPI"src/
eLinkDisplayStatus_CXXFLAGS += -I"src/lib/wiringPi/wiringPI"src/
eLinkDisplayStatus_CXXFLAGS += -I"src/lib/wiringPi/wiringPiD"
eLinkDisplayStatus_CXXFLAGS += -W"no-psabI"src/
eLinkDisplayStatus_CXXFLAGS += -D_GNU_SOURCE
eLinkDisplayStatus_CXXFLAGS += -D_POSIX_C_SOURCE=200112L


eLinkDisplayStatus_SOURCES = src/StatusManager.cpp \
				src/StatusPing.cpp \
				src/main.cpp \
				src/DataProvider.cpp \
				src/DS18B20/DS18B20.cpp \
				src/e-link/epdWiringPi.cpp \
				src/e-link/epdBcm2835.cpp \
				src/e-link/epd4in2.cpp \
				src/e-link/epd7in5b.cpp \
				src/e-link/epd.cpp \
				src/fonts/KS0108.cpp \
				src/fonts/KS0108/Arial28.cpp \
				src/fonts/KS0108/Digital-7-36Bold.cpp \
				src/fonts/KS0108/Ebrima28.cpp \
				src/fonts/KS0108/Georgia12.cpp \
				src/fonts/KS0108/Liberation_Mono12.cpp \
				src/fonts/KS0108/Liberation_Mono11Bold.cpp \
				src/fonts/KS0108/Liberation_Mono10Bold.cpp \
				src/fonts/KS0108/Liberation_Mono10.cpp \
				src/fonts/KS0108/Liberation_Mono9Bold.cpp \
				src/fonts/KS0108/Liberation_Mono9.cpp \
				src/fonts/KS0108/Liberation_Mono8.cpp \
				src/lib/pugi/pugixml.cpp \
				src/painter/PainterFont.cpp \
				src/painter/Painter.cpp \
				src/ping/ping.cpp \
				src/image/Image.cpp \
				src/image/Data/icons8-humidity-32.cpp \
				src/image/Data/icons8-temperature-30.cpp \
				src/SI7021/SI7021.cpp \
				src/lib/wiringPi/wiringPi/wiringPi.c						\
				src/lib/wiringPi/wiringPi/wiringSerial.c src/lib/wiringPi/wiringPi/wiringShift.c				\
				src/lib/wiringPi/wiringPi/piHiPri.c src/lib/wiringPi/wiringPi/piThread.c					\
				src/lib/wiringPi/wiringPi/wiringPiSPI.c src/lib/wiringPi/wiringPi/wiringPiI2C.c				\
				src/lib/wiringPi/wiringPi/softPwm.c src/lib/wiringPi/wiringPi/softTone.c					\
				src/lib/wiringPi/wiringPi/mcp23008.c src/lib/wiringPi/wiringPi/mcp23016.c src/lib/wiringPi/wiringPi/mcp23017.c			\
				src/lib/wiringPi/wiringPi/mcp23s08.c src/lib/wiringPi/wiringPi/mcp23s17.c					\
				src/lib/wiringPi/wiringPi/sr595.c							\
				src/lib/wiringPi/wiringPi/pcf8574.c src/lib/wiringPi/wiringPi/pcf8591.c					\
				src/lib/wiringPi/wiringPi/mcp3002.c src/lib/wiringPi/wiringPi/mcp3004.c src/lib/wiringPi/wiringPi/mcp4802.c src/lib/wiringPi/wiringPi/mcp3422.c			\
				src/lib/wiringPi/wiringPi/max31855.c src/lib/wiringPi/wiringPi/max5322.c src/lib/wiringPi/wiringPi/ads1115.c				\
				src/lib/wiringPi/wiringPi/sn3218.c						\
				src/lib/wiringPi/wiringPi/bmp180.c src/lib/wiringPi/wiringPi/htu21d.c src/lib/wiringPi/wiringPi/ds18b20.c src/lib/wiringPi/wiringPi/rht03.c			\
				src/lib/wiringPi/wiringPi/drcSerial.c src/lib/wiringPi/wiringPi/drcNet.c					\
				src/lib/wiringPi/wiringPi/pseudoPins.c						\
				src/lib/wiringPi/wiringPi/wpiExtensions.c \
				src/lib/bcm2835/bcm2835.c 

#eLinkDisplayStatus_dir = /usr/local/etc
#eLinkDisplayStatus__DATA = ../E-LinkStatusConfig.xml
