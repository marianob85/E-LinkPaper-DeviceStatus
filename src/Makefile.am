bin_PROGRAMS = eLinkDisplayStatus

eLinkDisplayStatus_CXXFLAGS = -I"DS18B20"
eLinkDisplayStatus_CXXFLAGS += -I"e-link"
eLinkDisplayStatus_CXXFLAGS += -I"fonts"
eLinkDisplayStatus_CXXFLAGS += -I"image"
eLinkDisplayStatus_CXXFLAGS += -I"painter"
eLinkDisplayStatus_CXXFLAGS += -I"ping"
eLinkDisplayStatus_CXXFLAGS += -I"lib/pugi"
eLinkDisplayStatus_CXXFLAGS += -I"lib/bcm2835"
eLinkDisplayStatus_CXXFLAGS += -I"lib/wiringPi"
eLinkDisplayStatus_CXXFLAGS += -I"lib/wiringPi/wiringPi"
eLinkDisplayStatus_CXXFLAGS += -I"lib/wiringPi/wiringPiD"
eLinkDisplayStatus_CXXFLAGS += -W"no-psabi"
eLinkDisplayStatus_CXXFLAGS += -D_GNU_SOURCE
eLinkDisplayStatus_CXXFLAGS += -D_POSIX_C_SOURCE=200112L


eLinkDisplayStatus_SOURCES = StatusManager.cpp \
				StatusPing.cpp \
				main.cpp \
				DS18B20/DS18B20.cpp \
				e-link/epdWiringPi.cpp \
				e-link/epdBcm2835.cpp \
				e-link/epd4in2.cpp \
				e-link/epd7in5b.cpp \
				e-link/epd.cpp \
				fonts/KS0108.cpp \
				fonts/KS0108/Arial28.cpp \
				fonts/KS0108/Digital-7-36Bold.cpp \
				fonts/KS0108/Ebrima28.cpp \
				fonts/KS0108/Georgia12.cpp \
				fonts/KS0108/Liberation_Mono12.cpp \
				fonts/KS0108/Liberation_Mono11Bold.cpp \
				fonts/KS0108/Liberation_Mono10Bold.cpp \
				fonts/KS0108/Liberation_Mono10.cpp \
				fonts/KS0108/Liberation_Mono9Bold.cpp \
				fonts/KS0108/Liberation_Mono9.cpp \
				fonts/KS0108/Liberation_Mono8.cpp \
				fonts/KS0108/Georgia12.cpp \
				lib/pugi/pugixml.cpp \
				painter/PainterFont.cpp \
				painter/Painter.cpp \
				ping/ping.cpp \
				image\Image.cpp \
				image\Data\icons8-humidity-32.cpp \
				image\Data\icons8-temperature-30.cpp \
				lib\wiringPi\wiringPi\wiringPi.c						\
				lib\wiringPi\wiringPi\wiringSerial.c lib\wiringPi\wiringPi\wiringShift.c				\
				lib\wiringPi\wiringPi\piHiPri.c lib\wiringPi\wiringPi\piThread.c					\
				lib\wiringPi\wiringPi\wiringPiSPI.c lib\wiringPi\wiringPi\wiringPiI2C.c				\
				lib\wiringPi\wiringPi\softPwm.c lib\wiringPi\wiringPi\softTone.c					\
				lib\wiringPi\wiringPi\mcp23008.c lib\wiringPi\wiringPi\mcp23016.c lib\wiringPi\wiringPi\mcp23017.c			\
				lib\wiringPi\wiringPi\mcp23s08.c lib\wiringPi\wiringPi\mcp23s17.c					\
				lib\wiringPi\wiringPi\sr595.c							\
				lib\wiringPi\wiringPi\pcf8574.c lib\wiringPi\wiringPi\pcf8591.c					\
				lib\wiringPi\wiringPi\mcp3002.c lib\wiringPi\wiringPi\mcp3004.c lib\wiringPi\wiringPi\mcp4802.c lib\wiringPi\wiringPi\mcp3422.c			\
				lib\wiringPi\wiringPi\max31855.c lib\wiringPi\wiringPi\max5322.c lib\wiringPi\wiringPi\ads1115.c				\
				lib\wiringPi\wiringPi\sn3218.c						\
				lib\wiringPi\wiringPi\bmp180.c lib\wiringPi\wiringPi\htu21d.c lib\wiringPi\wiringPi\ds18b20.c lib\wiringPi\wiringPi\rht03.c			\
				lib\wiringPi\wiringPi\drcSerial.c lib\wiringPi\wiringPi\drcNet.c					\
				lib\wiringPi\wiringPi\pseudoPins.c						\
				lib\wiringPi\wiringPi\wpiExtensions.c \
				lib\bcm2835\bcm2835.c 

eLinkDisplayStatus_dir = /usr/local/etc
eLinkDisplayStatus__DATA = ../E-LinkStatusConfig.xml
