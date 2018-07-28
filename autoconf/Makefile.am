bin_PROGRAMS = eLinkDisplayStatus

eLinkDisplayStatus_CXXFLAGS = -I"src/DS18B20"
eLinkDisplayStatus_CXXFLAGS += -I"src/e-link"
eLinkDisplayStatus_CXXFLAGS += -I"src/fonts"
eLinkDisplayStatus_CXXFLAGS += -I"src/image"
eLinkDisplayStatus_CXXFLAGS += -I"src/painter"
eLinkDisplayStatus_CXXFLAGS += -I"src/ping"
eLinkDisplayStatus_CXXFLAGS += -I"src/SI7021"
eLinkDisplayStatus_CXXFLAGS += -I"src/lib"
eLinkDisplayStatus_CXXFLAGS += -I"src/lib/pugi"
eLinkDisplayStatus_CXXFLAGS += -I"src/lib/libi2c"
eLinkDisplayStatus_CXXFLAGS += -I"src/lib/influxDB"
eLinkDisplayStatus_CXXFLAGS += -I"lib/wiringNPi"
eLinkDisplayStatus_CXXFLAGS += -Wno-psabi -Wno-implicit-fallthrough -Wno-unused-result
eLinkDisplayStatus_CXXFLAGS += -D_GNU_SOURCE
eLinkDisplayStatus_CXXFLAGS += -D_POSIX_C_SOURCE=200112L

eLinkDisplayStatus_CFLAGS  = $(eLinkDisplayStatus_CXXFLAGS)
eLinkDisplayStatus_CFLAGS += -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-sign-conversion -Wno-conversion -Wno-switch-bool -Wno-implicit-fallthrough -Wno-unused-result

eLinkDisplayStatus_SOURCES = src/StatusManager.cpp \
				src/StatusPing.cpp \
				src/StatusLed.cpp \
				src/main.cpp \
				src/InfluxWriter.cpp \
				src/DataProvider.cpp \
				src/DS18B20/DS18B20.cpp \
				src/e-link/epdWiringPi.cpp \
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
				src/painter/PainterFont.cpp \
				src/painter/Painter.cpp \
				src/ping/ping.cpp \
				src/image/Image.cpp \
				src/image/Data/icons8-humidity-32.cpp \
				src/image/Data/icons8-temperature-30.cpp \
				src/SI7021/SI7021.cpp \
				src/lib/libi2c/i2c.c \
				src/lib/influxDB/influxdb.cpp \
				src/lib/pugi/pugixml.cpp \
				lib/wiringNPi/boardtype_friendlyelec.c \
				lib/wiringNPi/drcSerial.c \
				lib/wiringNPi/max5322.c \
				lib/wiringNPi/max31855.c \
				lib/wiringNPi/mcp23s08.c \
				lib/wiringNPi/mcp23s17.c \
				lib/wiringNPi/mcp3002.c \
				lib/wiringNPi/mcp3004.c \
				lib/wiringNPi/mcp3422.c \
				lib/wiringNPi/mcp4802.c \
				lib/wiringNPi/mcp23008.c \
				lib/wiringNPi/mcp23016.c \
				lib/wiringNPi/mcp23017.c \
				lib/wiringNPi/pcf8574.c \
				lib/wiringNPi/pcf8591.c \
				lib/wiringNPi/piHiPri.c \
				lib/wiringNPi/piThread.c \
				lib/wiringNPi/sn3218.c \
				lib/wiringNPi/softPwm.c \
				lib/wiringNPi/softServo.c \
				lib/wiringNPi/softTone.c \
				lib/wiringNPi/sr595.c \
				lib/wiringNPi/wiringPi.c \
				lib/wiringNPi/wiringPiI2C.c \
				lib/wiringNPi/wiringPiSPI.c \
				lib/wiringNPi/wiringSerial.c \
				lib/wiringNPi/wiringShift.c

#eLinkDisplayStatus_dir = /usr/local/etc
#eLinkDisplayStatus__DATA = ../E-LinkStatusConfig.xml
