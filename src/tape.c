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
/** Print a help message.
 * name: help
 */
void help(char **argv) {
    char *s = strrchr (argv[0], '/');
    if (!s) s = strrchr (argv[0], '\\');
    if (!s) s = argv[0]; else (s++);
#ifdef WITH_LZMA
 printf ("Create playable archive: %s [options] [media] [attachments] ... output_media\n\n" 

            "   -c optionally compress attachments\n" 
            "      (media remains playable, and is never compressed).\n\n" 
            
            "Extraction: %s [options] [attachments] ... input_media:\n" 
            "   -k list with human-readable file sizes\n" 
            "   -l list media attachments\n" 
            "   -x eXtract [attachments] from input_media\n" 
            "   -o output to [directory]\n" 
            "   -p extract [attachments] from input_media to pipe\n" 
            "   -h Help\n", s, s);
#else
 printf ("Create playable archive: %s [options] [media] [attachments] ... output_media\n\n" 

            "Extraction: %s [options] [attachments] ... input_media:\n" 
            "   -k list with human-readable file sizes\n" 
            "   -l list media attachments\n" 
            "   -x eXtract [attachments] from input_media\n" 
            "   -o output to [directory]\n" 
            "   -p extract [attachments] from input_media to pipe\n" 
            "   -h Help\n", s, s);
#endif
 exit (EXIT_FAILURE);
} int main (int argc, char** argv) {
    FILE *f_in;
    int option, pipe = 0, compress = 0, extract = 0, cd=0, human = 0;
    char *dir = NULL;
    // setlocale  LC_ALL, '\0'
    /* argc is always 1 more than what we need */
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
                } case 'k': {
                    human = 1;
                } case 'l': {
                    if (argc < option + 1) {
                        ERR ("No file to list.\n");
                    } list_archive(argv[argc], human);
                    return EXIT_SUCCESS;
                    break;
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
                } return EXIT_FAILURE;
        }   } option++;
    } if (cd) dir = argv[option++];
    if (extract) {
        if (argc < option) {
            ERR ("Error: No archive to extract from.\n");
        }        /* get catalog as list */
        ps_list ctl = NULL;
        f_in = open_archive (argv[argc], &ctl);
        /* mark specified files for extraction */
        if (argc!=option) FOR_IN (item, ctl) {
            item->ex = 0;
            int i=option;
            for (;i<argc;i++) {
                if (!strcmp(item->name, argv[i])) item->ex = 1;
        }   } extract_list (f_in, ctl, dir, pipe);
        free_catalog(ctl);
        fclose (f_in);
        /* Close stdout to catch possible write errors */
        return EXIT_SUCCESS;
    }    /* default action: create tape from what's left of argv array */
    tape_from_array(argc - option, &argv[option], compress);
    return EXIT_SUCCESS;
} 