bin_PROGRAMS = eLinkDisplayStatus

eLinkDisplayStatus_CXXFLAGS = -I"DS18B20"
eLinkDisplayStatus_CXXFLAGS += -I"e-link_4in2"
eLinkDisplayStatus_CXXFLAGS += -I"fonts"
eLinkDisplayStatus_CXXFLAGS += -I"lib/pugi"
eLinkDisplayStatus_CXXFLAGS += -I"painter"
eLinkDisplayStatus_CXXFLAGS += -I"ping"
eLinkDisplayStatus_CXXFLAGS += -O2 -Wall -W"no-psabi"


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

