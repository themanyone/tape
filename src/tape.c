/* tape.c * * * * * * * * * * * * * * ** * * * * * * * * * * * * * * * \
*                                                                      *
* Things All Peoplel Enjoy (TAPE)                                      *
*                                                                      *
* Create playable multimedia archives. Compatible with current players!*
*                                                                      *
* Copyright (C) 2016 by Henry Kroll III, www.thenerdshow.com
* 
                             Apache License
                       Version 2.0, January 2004
                    http://www.apache.org/licenses/ 

*/
#include "tape.h"

int main (int argc, char** argv) {
    FILE *f_in, *f_out;
    int this_file, option, pipe = 0, compress = 0, extract = 0, cd=0;
    size_t file_size[255];
    char buf[buf_MAX], *dir = NULL;
    setlocale (LC_ALL, '\0');
    /* argc is always 1 more than what we need... */
    argc--;
    /* handle options */
    option = 1;
    if (argc < 1) help(argv);
    while (argv[option][0] == '-') {
        for (int c = 1;argv[option][c];c++) {
            switch (argv[option][c]) {
                case 'c': {
                    compress = 1;
                    break;
                } case 'l': {
                    if (argc < option + 1) {
                        ERR ("No file to list.\n");
                    } list_archive(argv[argc]);
                    return 0;
                } case 'o': { /* pipe */
                    cd = 1;
                    break;
                } case 'p': { /* pipe */
                    pipe = 1;
                    extract = 1;
                    break;
                } case 'x': {
                    extract = 1;
                    break;
                } default: {
                    if (argv[option][c]!='h') {
                        INFO ("Unknown option, %c.\n", argv[option][c]);
                    } help(argv);
                } return 1;
        }   } option++;
    } if (cd) dir = argv[option++];
    if (extract) {
        if (argc < option) {
            ERR ("Error: No archive to extract from.\n");
        }        /* get catalog as list */
        ps_list ctl = NULL;
        f_in = open_archive (argv[argc], &ctl);
        /* extract all? */
        if (argc==option) extract_list (f_in, ctl, dir, pipe);
        else {
            /* go through catalog list */
            FOR_IN (item, ctl) {
                int i=option;
                /* extract files named in command line */
                for (;i<argc;i++) {
                    if (!strcmp(&item->name, argv[i])) {
                        extract_next (f_in, item, dir, pipe);
                        break;
                }   }                /* skip ahead if we didn't extract a file */
                if (i==argc) fseek(f_in, item->sz, SEEK_CUR);
        }   } free_catalog(ctl);
        fclose (f_in);
        /* Close stdout to catch possible write errors */
        if (fclose(stdout)) {
            INFO ("Write error: %s\n", strerror(errno));
        } return 0;
    } char *e1, *e2;
    e1 = strrchr (argv[argc], '.');
    e2 = strrchr (argv[option], '.');
    if (!e1||!e2||strcmp(e1, e2)) INFO ("Warning: First and last files have different extensions.\nMedia might not play.\n");
    /* open last file for append in binary mode? */
    DID (f_out=fopen(argv[argc],"w+"));
    /* yes, let's iterate through each file */
    for (this_file=option; this_file < argc; this_file++) {
        /* can the file be opened for reading? */
        INFO ("Adding %s\n", argv[this_file]);
        DID (f_in=fopen(argv[this_file],"rb"));
        /* yes, append f_in to f_out */
        sz = attach_file (f_in, f_out, compress);
        /* now close input file */
        DID (!fclose(f_in));
        /* keep track of file sizes */
        file_size[this_file] = sz;
        if (compress == 1) compress = 6;
    }    /* write catalog */
    sz = 0;
    for (this_file=option; this_file < argc; this_file++) {
        sz += fprintf (f_out, "%s, %lu\n", argv[this_file], file_size[this_file]);
    }    /* write catalog size */
    fprintf (f_out, "%lu\n", sz);
    /* close output file */
    DID (!fclose(f_out));
    return 0;
} 