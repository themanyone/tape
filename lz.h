/* lz.h
*
* Copyright (C) 2016 by Henry Kroll III, www.thenerdshow.com
* 
*                            Apache License
*                      Version 2.0, January 2004
*                   http://www.apache.org/licenses
*/
#ifndef LZ_H_   /* Include guard */
#define LZ_H_
#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>
#include <lzma.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

extern size_t lzcompress(lzma_stream *, FILE *, FILE *);
extern bool init_encoder(lzma_stream *, uint32_t );
extern bool lzdecompress(lzma_stream *, const char *, FILE *, FILE *);
extern bool init_decoder(lzma_stream *);

#endif // LZ_H_
