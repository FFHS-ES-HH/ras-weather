src				= $(top_srcdir)/src
lib				= $(top_srcdir)/lib
dist				= $(top_srcdir)/dist

AM_CPPFLAGS			= -I$(src) -I$(lib)

bin_PROGRAMS			= piweather
initddir			= $(sysconfdir)/init.d
initd_SCRIPTS			= $(dist)/piweather

lib_LTLIBRARIES			= libpiweather.la

piweather_SOURCES		=\
				 $(src)/view/Lcd.hpp \
				 $(src)/view/Lcd.cpp \
				 $(src)/view/Viewable.hpp \
				 $(src)/db/Values.hpp \
				 $(src)/Application.hpp \
				 $(src)/Application.cpp \
				 $(src)/piw-daemon.cpp

piweather_LDADD			= libpiweather.la

libpiweather_la_SOURCES		=\
				 $(lib)/bricklet_ambient_light.c \
				 $(lib)/bricklet_barometer.c \
				 $(lib)/bricklet_humidity.c \
				 $(lib)/bricklet_lcd_20x4.c \
				 $(lib)/ip_connection.c \
				 $(src)/device/UidRegistry.cpp \
				 $(src)/device/Observable.cpp \
				 $(src)/device/Button.cpp \
				 $(src)/db/Database.cpp \
				 $(src)/sensors/Temperature.cpp \
				 $(src)/sensors/Barometer.cpp \
				 $(src)/sensors/Illuminance.cpp \
				 $(src)/sensors/Humidity.cpp

libpiweather_la_includedir	= $(includedir)/piweather
libpiweatherdevice_includedir	= $(libpiweather_la_includedir)/device
libpiweathersensors_includedir	= $(libpiweather_la_includedir)/sensors
libpiweatherdb_includedir	= $(libpiweather_la_includedir)/db

libpiweather_la_include_HEADERS	=\
				 $(lib)/bricklet_ambient_light.h \
				 $(lib)/bricklet_barometer.h \
				 $(lib)/bricklet_humidity.h \
				 $(lib)/bricklet_lcd_20x4.h \
				 $(lib)/ip_connection.h

libpiweatherdevice_include_HEADERS =\
				 $(src)/device/UidRegistry.hpp \
				 $(src)/device/Observable.hpp \
				 $(src)/device/Button.hpp

libpiweathersensors_include_HEADERS =\
				 $(src)/sensors/ThresholdObservable.hpp \
				 $(src)/sensors/Humidity.hpp \
				 $(src)/sensors/Barometer.hpp \
				 $(src)/sensors/Illuminance.hpp \
				 $(src)/sensors/Temperature.hpp

libpiweatherdb_include_HEADERS =\
				 $(src)/db/Database.hpp

libpiweather_la_CFLAGS		= $(PTHREAD_CFLAGS) $(SQLITE3_CFLAGS)
libpiweather_la_CXXFLAGS	= $(libpiweather_la_CFLAGS)
libpiweather_la_LIBADD		= $(PTHREAD_LIBS) $(SQLITE3_LDFLAGS)
LIBPIWEATHER_VERSION		= 1:0:0
libpiweather_la_LDFLAGS		= -version-info $(LIBPIWEATHER_VERSION)

CLEANFILES			= $(bin_SCRIPTS)

SUBST="s|!!sysconfdir!!|${sysconfdir}|;s|!!sbindir!!|${sbindir}|;s|!!bindir!!|${bindir}|;s|!!localstatedir!!|${localstatedir}|"
$(dist)/piweather:	$(dist)/piweather.in
	sed $(SUBST) $< > $@

