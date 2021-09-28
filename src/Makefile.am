bin_PROGRAMS = tape
tape_SOURCES = tape.c
tape_LDFLAGS = -ltape -lm
tape_LDADD = libtape.la
AM_CFLAGS = -g -Wall -pedantic -std=c99

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libtape.la
libtape_la_LDFLAGS = -no-undefined -version-info 0:0:0
libtape_la_SOURCES = libtape.c

if WITH_LZMA
libtape_la_LDFLAGS += @LZMA_LIBS@
libtape_la_CFLAGS   = @LZMA_CFLAGS@
libtape_la_SOURCES += libxzextract.c libxzcompress.c
endif

#~ make foo.c
SUFFIXES = .anch
.anch.c:
	anchor -qp "$<" > "$@"
