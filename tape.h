/* tape.h
*
* Copyright (C) 2016 by Henry Kroll III, www.thenerdshow.com
* 
*                            Apache License
*                      Version 2.0, January 2004
*                   http://www.apache.org/licenses
*/
#ifndef TAPE_H_   /* Include guard */
#define TAPE_H_
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <langinfo.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <math.h>

#define LZMA 1
#ifdef LZMA
#include <lzma.h>
#include <stdbool.h>
#include <errno.h>
size_t lzcompress(lzma_stream *, FILE *, FILE *);
bool init_encoder(lzma_stream *, uint32_t );
bool lzdecompress(lzma_stream *, const char *, FILE *, FILE *);
bool init_decoder(lzma_stream *);
#endif
/* Use any size buffer... */
/* I/O is buffered anyway, so it doesn't matter */
#define buf_MAX 512
#define MAX_PATH 256
#define cat_SZ 16384 //max size of text catalog
#define OFS 20 // catalog size: up to 20 digits for size_t data type
size_t sz, offset;
typedef struct cat {
    struct cat *next;
    size_t sz;
    char name;
} s_list, *ps_list;
void list_archive(const char *);
void extract_list(FILE *, ps_list, char*, int);
FILE *open_archive(const char *, ps_list*);
size_t attach_file(FILE* , FILE*, uint32_t);
ps_list parse_catalog(char *);
void extract_next (FILE *, ps_list, char*, int);
void free_catalog(ps_list);
void help(char **);
#if defined(_WIN32) || defined(_WIN64) // if windows
#define DLL_EXPORT __declspec(dllexport) // make DLL
#else
#define DLL_EXPORT
#endif
// Error handling macros
#define _STR(a) #a
#define STR(a) _STR(a)
#define DID(a,...) if (!(a)){                                              \
    perror(__FILE__":"STR(__LINE__)" "STR(__VA_ARGS__));exit(1);}
#define ERR(...) fprintf (stderr, STR(__LINE__)                        \
 ": " __VA_ARGS__);exit (EXIT_FAILURE)
#define INFO(...) fprintf (stderr, __VA_ARGS__);
#define FOR_IN(item, list) for (ps_list item=list;item;item=item->next)
#endif // TAPE_H_
