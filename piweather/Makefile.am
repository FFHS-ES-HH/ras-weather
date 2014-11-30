ACLOCAL_AMFLAGS		= -I m4 --install
SUBDIRS			= build

piweatherdir		= $(pkgdatadir)
piweather_DATA		= dist/schema.sql

piweathersqlitedbdir	= $(localstatedir)/piweather
piweathersqlitedb_DATA	= dist/piweather.sqlite3

EXTRA_DIST		=\
			 $(piweather_DATA) \
			 $(piweathersqlitedb_DATA)

CLEANFILES		= *~
MAINTAINERCLEANFILES	= Makefile.in
