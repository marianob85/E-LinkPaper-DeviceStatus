bin_PROGRAMS = eLinkDisplayStatus

eLinkDisplayStatus_CXXFLAGS = -I"DS18B20"
eLinkDisplayStatus_CXXFLAGS += -I"e-link"
eLinkDisplayStatus_CXXFLAGS += -I"fonts"
eLinkDisplayStatus_CXXFLAGS += -I"lib/pugi"
eLinkDisplayStatus_CXXFLAGS += -I"painter"
eLinkDisplayStatus_CXXFLAGS += -I"ping"
eLinkDisplayStatus_CXXFLAGS += -W"no-psabi"

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

eLinkDisplayStatus_dir = /usr/local/etc
eLinkDisplayStatus__DATA = ../E-LinkStatusConfig.xml
