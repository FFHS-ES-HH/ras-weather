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

BINFILE			= $(PACKAGE)-$(PACKAGE_VERSION).bin.tar.gz
BININST			= /tmp/$(PACKAGE)-$(PACKAGE_VERSION)
OWNER			= 1000:1000
bindist:		all
	@echo "Creating $(BINFILE)..."
	rm -rf $(BININST) $(BINFIILE) && \
	$(MAKE) DESTDIR=$(BININST) install && \
	chown $(OWNER) $(BININST)$(localstatedir)/piweather/piweather.sqlite3 && \
	tar -cvzf $(BINFILE) -C $(BININST) . && \
	find $(BININST) -type f | sed 's|$(BININST)||' > $(PACKAGE)-$(PACKAGE_VERSION).bin.files
