bin_PROGRAMS = tape
tape_SOURCES = tape.c
tape_LDADD = -lm -llzma libtape.a libxzcompress.a libxzextract.a

noinst_LIBRARIES = libtape.a libxzcompress.a libxzextract.a

libtape_a_SOURCES = libtape.c
libxzcompress_a_SOURCES = libxzcompress.c
libxzextract_a_SOURCES = libxzextract.c

#~ make foo.c
SUFFIXES = .anch
.anch.c:
	anchor -qp "$<" > "$@"
