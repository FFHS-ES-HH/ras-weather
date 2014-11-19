src				= $(top_srcdir)/src
lib				= $(top_srcdir)/lib

bin_PROGRAMS			= piweather

lib_LTLIBRARIES			= libpiweather.la

piweather_SOURCES		=\
				 $(src)/view/Lcd.hpp \
				 $(src)/view/Lcd.cpp \
				 $(src)/view/Viewable.hpp \
				 $(src)/device/Button.hpp \
				 $(src)/device/Button.cpp \
				 $(src)/db/Values.hpp

piweather_LDADD			= libpiweather.la

libpiweather_la_SOURCES		=\
				 $(lib)/bricklet_ambient_light.c \
				 $(lib)/bricklet_ambient_light.h \
				 $(lib)/bricklet_barometer.c \
				 $(lib)/bricklet_barometer.h \
				 $(lib)/bricklet_humidity.c \
				 $(lib)/bricklet_humidity.h \
				 $(lib)/bricklet_lcd_20x4.c \
				 $(lib)/bricklet_lcd_20x4.h \
				 $(lib)/ip_connection.c \
				 $(lib)/ip_connection.h \
				 $(src)/device/UidRegistry.hpp \
				 $(src)/device/UidRegistry.cpp

libpiweather_la_CFLAGS		= $(PTHREAD_CFLAGS) $(SQLITE3_CFLAGS)
libpiweather_la_CXXFLAGS	= $(libpiweather_la_CFLAGS)
libpiweather_la_LIBADD		= $(PTHREAD_LIBS) $(SQLITE3_LDFLAGS)
LIBPIWEATHER_VERSION		= 1:0:0
libpiweather_la_LDFLAGS		= -version-info $(LIBPIWEATHER_VERSION)

AM_CPPFLAGS			= -I$(src) -I$(lib)

