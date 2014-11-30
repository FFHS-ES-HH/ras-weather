src				= $(top_srcdir)/src
lib				= $(top_srcdir)/lib
dist				= $(top_srcdir)/dist
initscripts			= $(dist)/initd
configs				= $(dist)/config
defaultconfigs			= $(dist)/default

AM_CPPFLAGS			= -I$(src) -I$(lib)

bin_PROGRAMS			= piweather

lib_LTLIBRARIES			= libpiweather.la

piweather_SOURCES		=\
				 $(src)/app/piweather.cpp \
				 $(src)/app/Configuration.hpp \
				 $(src)/app/Configuration.cpp \
				 $(src)/app/ConfigCollector.hpp \
				 $(src)/app/ConfigCollector.cpp \
				 $(src)/app/Application.hpp \
				 $(src)/app/Application.cpp \
				 $(src)/db/Values.hpp \
				 $(src)/db/Database.hpp \
				 $(src)/db/Database.cpp \
				 $(src)/view/LcdView.hpp \
				 $(src)/view/Temperature.hpp \
				 $(src)/view/Temperature.cpp \
				 $(src)/view/Illuminance.hpp \
				 $(src)/view/Illuminance.cpp \
				 $(src)/view/AirPressure.hpp \
				 $(src)/view/AirPressure.cpp \
				 $(src)/view/Humidity.hpp \
				 $(src)/view/Humidity.cpp

piweather_LDADD			= libpiweather.la
piweather_CPPFLAGS		=\
				 $(AM_CPPFLAGS) \
				 -DDEFAULT_CONFIG_PATH="\"@sysconfdir@/piweather/piweather.conf\""

libpiweather_la_SOURCES		=\
				 $(lib)/ip_connection.c \
				 $(lib)/bricklet_ambient_light.c \
				 $(lib)/bricklet_barometer.c \
				 $(lib)/bricklet_humidity.c \
				 $(lib)/bricklet_lcd_20x4.c \
				 $(src)/device/UidRegistry.cpp \
				 $(src)/device/Observable.cpp \
				 $(src)/device/Button.cpp \
				 $(src)/device/Connection.cpp \
				 $(src)/device/Lcd.hpp \
				 $(src)/device/Lcd.cpp \
				 $(src)/sensors/Temperature.cpp \
				 $(src)/sensors/Barometer.cpp \
				 $(src)/sensors/Illuminance.cpp \
				 $(src)/sensors/Humidity.cpp

libpiweather_la_includedir	= $(includedir)/piweather
libpiweatherdevice_includedir	= $(libpiweather_la_includedir)/device
libpiweathersensors_includedir	= $(libpiweather_la_includedir)/sensors

libpiweather_la_include_HEADERS	=\
				 $(lib)/bricklet_ambient_light.h \
				 $(lib)/bricklet_barometer.h \
				 $(lib)/bricklet_humidity.h \
				 $(lib)/bricklet_lcd_20x4.h \
				 $(lib)/ip_connection.h

libpiweatherdevice_include_HEADERS =\
				 $(src)/device/UidRegistry.hpp \
				 $(src)/device/Observable.hpp \
				 $(src)/device/Observer.hpp \
				 $(src)/device/Connection.hpp \
				 $(src)/device/Button.hpp

libpiweathersensors_include_HEADERS =\
				 $(src)/sensors/ThresholdObservable.hpp \
				 $(src)/sensors/Humidity.hpp \
				 $(src)/sensors/Barometer.hpp \
				 $(src)/sensors/Illuminance.hpp \
				 $(src)/sensors/Temperature.hpp

libpiweather_la_CFLAGS		= $(PTHREAD_CFLAGS) $(SQLITE3_CFLAGS)
libpiweather_la_CXXFLAGS	= $(libpiweather_la_CFLAGS)
libpiweather_la_LIBADD		= $(PTHREAD_LIBS) $(SQLITE3_LDFLAGS)
LIBPIWEATHER_VERSION		= 1:0:0
libpiweather_la_LDFLAGS		= -version-info $(LIBPIWEATHER_VERSION)

initddir			= $(sysconfdir)/init.d
initd_SCRIPTS			= $(initscripts)/piweather

initdefaultdir			= $(sysconfdir)/default
initdefault_DATA		= $(defaultconfigs)/piweather

piwconfigdir			= $(sysconfdir)/piweather
piwconfig_DATA			= $(configs)/piweather.conf

SUBST="s|[@]sysconfdir[@]|${sysconfdir}|;s|[@]sbindir[@]|${sbindir}|;s|[@]bindir[@]|${bindir}|;s|[@]localstatedir[@]|${localstatedir}|"

$(initscripts)/piweather:	$(initscripts)/piweather.in
	@sed $(SUBST) $< > $@

$(defaultconfigs)/piweather:	$(defaultconfigs)/piweather.in
	@sed $(SUBST) $< > $@

$(configs)/piweather.conf:	$(configs)/piweather.conf.in
	@sed $(SUBST) $< > $@

CLEANFILES			=\
				 $(initd_SCRIPTS) \
				 $(initdefault_DATA) \
				 $(piwconfig_DATA)
