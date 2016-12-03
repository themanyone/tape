bin_PROGRAMS = tape
tape_SOURCES = tape.c
tape_CFLAGS = -g -Wall -pedantic -std=c99

if WITH_LZMA
tape_LDADD = -lm libtape.a libxzcompress.a libxzextract.a @LZMA_LIBS@
tape_CFLAGS += @LZMA_CFLAGS@
noinst_LIBRARIES = libtape.a libxzcompress.a libxzextract.a
libtape_a_SOURCES = libtape.c
libxzcompress_a_SOURCES = libxzcompress.c
libxzextract_a_SOURCES = libxzextract.c
else
tape_LDADD = -lm libtape.a
noinst_LIBRARIES = libtape.a
libtape_a_SOURCES = libtape.c
endif

#~ make foo.c
SUFFIXES = .anch
.anch.c:
	anchor -qp "$<" > "$@"
