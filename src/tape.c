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
    int this_file, option, pipe, compress;
    size_t file_size[255];
    char buf[buf_MAX];
    setlocale (LC_ALL, '\0');
    /* argc is always 1 more than what we need... */
    argc--;
    /* handle options */
    option = 1;
    if (argc < 1) help(argv);
    while (argv[option][0] == '-') {
        switch (argv[option][1]) {
            case 'c': {
                compress = 1;
                break;
            } case 'l': {
                if (argc < option + 1) {
                    ERR ("No file listed.\n");
                }                /* open and read offset offset */
                f_in = getfile (argv[argc], buf);
                printf ("%s", buf);
                fclose (f_in);
                return 0;
            } case 'p': { /* pipe */
                pipe = 1;
            } case 'x': {
                if (argc < option + 1) {
                    ERR ("Error: No archive to extract from.\n");
                }                /* open and read offset offset */
                char txtcat[cat_SZ];
                ps_cat item, ctl;
                /* get file handle and text catalog */
                f_in = getfile (argv[argc], txtcat);
                /* convert text catalog into linked list */
                ctl = item = parse_catalog(txtcat);
                DID (!fseek(f_in, 0, SEEK_SET));
                /* go through catalog list */
                for (item=ctl;item;item=item->next) {
                    if (argc==option + 1) { /* extract all */
                        extract (item, f_in, pipe);
                    } else { /* extract files that match supplied args */
                        int i=option + 1;
                        for (;i<argc;i++) {
                            if (!strcmp(&item->name, argv[i])) {
                                extract (item, f_in, pipe);
                                break;
                        }   }                        /* skip ahead if we didn't extract a file */
                        if (i==argc) fseek(f_in, item->sz, SEEK_CUR);
                }   } freecatalog(ctl);
                fclose (f_in);
                /* Close stdout to catch possible write errors */
                if (!fclose(stdout)) {
                    INFO ("Write error: %s\n", strerror(errno));
                } return 0;
            } default: {
                if (argv[option][1]!='h') {
                    INFO ("Unknown option, %s.\n", argv[option]);
                } help(argv);
            } return 1;
        } option++;
    } char *e1, *e2;
    e1 = strrchr (argv[argc], '.');
    e2 = strrchr (argv[option], '.');
    if (strcmp(e1, e2)) INFO ("Warning: different file extensions. Media might not play.\n");
    /* open last file for append in binary mode? */
    DID (f_out=fopen(argv[argc],"w+"));
    /* yes, let's iterate through each file */
    for (this_file=option; this_file < argc; this_file++) {
        /* can the file be opened for reading? */
        INFO ("Adding %s\n", argv[this_file]);
        DID (f_in=fopen(argv[this_file],"rb"));
        /* yes, append f_in to f_out */
        sz = attach_file (f_in, f_out);
        /* now close input file */
        DID (fclose(f_in));
        /* keep track of file sizes */
        file_size[this_file] = sz;
    }    
    /* write catalog */
    sz = 0;
    for (this_file=option; this_file < argc; this_file++) {
        sz += fprintf (f_out, "%s, %lu\n", argv[this_file], file_size[this_file]);
    }    /* write catalog size */
    fprintf (f_out, "%lu\n", sz);
    /* close output file */
    DID (fclose(f_out));
    return 0;
} 