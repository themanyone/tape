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

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

//limits.h
//float.h
/* Use any size buffer... */
/* I/O is buffered anyway, so it doesn't matter */
#define buf_MAX 512
#define MAX_PATH 256
#define cat_SZ 16384 //max size of text catalog
#define OFS 22 // catalog size: up to 20 digits + 2 lf
size_t sz, offset;
typedef struct tape {
    struct tape *next;
    size_t sz;
    int ex;
    char name;
} s_list, *ps_list;
extern void list_archive(const char *, int);
extern void extract_list(FILE *, ps_list, char*, int);
extern FILE *open_archive(const char *, ps_list*);
extern size_t attach_file(FILE*, char* , unsigned int);
extern ps_list parse_catalog(char *);
extern void extract_next (FILE *, ps_list, char*, int);
extern void tape_from_array(int, char **, int);
extern void catalog_append(ps_list *, char *, size_t);
extern void free_catalog(ps_list);

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
