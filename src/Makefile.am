#   Copyright (C) 2004, 2005, 2006, 2007, 2008, 
#   Rocky Bernstein <rocky@gnu.org>
#   Copyright (C) 1998 Monty xiphmont@mit.edu
#

transform = s,cd-paranoia,@CDPARANOIA_NAME@,

if BUILD_CD_PARANOIA

SUBDIRS = doc

GETOPT_C = getopt1.c getopt.c

EXTRA_DIST = usage.txt.in usage-copy.h pod2c.pl \
             doc/FAQ.txt doc/overlapdef.txt $(GETOPT_C) getopt.h

noinst_HEADERS = header.h report.h $(GETOPT_H)

cd_paranoia_SOURCES = cd-paranoia.c \
	buffering_write.c buffering_write.h \
	header.c report.c utils.h version.h $(GETOPT_C)

cd_paranoia_LDADD =  $(LIBCDIO_LIBS) $(LIBCDIO_CDDA_LIBS) $(LIBCDIO_PARANOIA_LIBS) $(LTLIBICONV)

cd_paranoia_DEPENDENCIES =  $(LIBCDIO_DEPS) $(LIBCDIO_CDDA_LIBS) $(LIBCDIO_PARANOIA_LIBS)

bin_PROGRAMS = cd-paranoia

INCLUDES = -I$(top_srcdir) $(LIBCDIO_CFLAGS)

cd-paranoia.$(OBJEXT): usage.h

#: create header file used in help text: the "usage" help.
if HAVE_PERL
usage.h: usage.txt $(srcdir)/pod2c.pl
	$(PERL) $(srcdir)/pod2c.pl usage.txt >usage.h
else
usage.h: usage-copy.h
	cp usage-copy.h $@
endif

endif

MOSTLYCLEANFILES = usage.h usage.txt
