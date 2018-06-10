bin_PROGRAMS = eLinkDisplayStatus

AM_CXXFLAGS = -I"DS18B20"
AM_CXXFLAGS += -I"e-link_4in2"
AM_CXXFLAGS += -I"fonts"
AM_CXXFLAGS += -I"lib/pugi"
AM_CXXFLAGS += -I"painter"
AM_CXXFLAGS += -I"ping"


eLinkDisplayStatus_SOURCES = StatusManager.cpp \
				StatusPing.cpp \
				main.cpp \
				DS18B20/DS18B20.cpp \
				e-link_4in2/epdWiringPi.cpp \
				e-link_4in2/epdBcm2835.cpp \
				e-link_4in2/epd4in2.cpp \
				fonts/KS0108.cpp \
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
				ping/ping.cpp

LDADDS = bcm2835 wiringPi pthread stdc++fs
