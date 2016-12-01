/* libtape.c * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
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
#include <lzma.h>
/** List contents of [archive name].
 * name: list_archive
 * @param *name is the playable archive to examine
 */
DLL_EXPORT void list_archive(const char *name) {
    ps_list cat = NULL;
    size_t fs;
    int i = 0;
    char sizes[] = " KMGT";
    FILE *f_in = open_archive (name, &cat);
    printf ("Contents of %s:\n", name);
    FOR_IN (item, cat) {
        fs = item->sz;
        for (i=0;i<4;i++) if (!(fs / (size_t)pow(1000, i+1))) break;
        fs /= pow(1000, i);
        printf ("%lu%c\t%s\n", fs, sizes[i], &item->name);
    } DID (fclose) f_in;
    free_catalog (cat);
}
/** Open named archive and extract a list of files.
 * name: extract_list
 * @param *f_in is an opened file handle to extract from
 * @param files is a pointer to linked list of files to extract
 * @param pipe is an integer flag 1=pipe to stdout
 */
DLL_EXPORT void extract_list(FILE *f_in, ps_list files, char *dir, int pipe) {
    /* go through catalog list */
    FOR_IN (item, files) extract_next (f_in, item, dir, pipe);
    /* Close stdout to catch possible write errors */
    if (pipe && !fclose(stdout)) {
        INFO ("Write errors: %s\n", strerror(errno));
}   }
/** Extract an item from f_in, or optionally pipe to stdout.
 * name: extract_next
 * @param item is a pointer to a catalog struct in the list
 * @param f_in is an opened file handle to extract from
 * @param pipe = 0 to extract to file, 1 to pipe to stdout
 */
DLL_EXPORT void extract_next (FILE *f_in, ps_list item, char *dir, int pipe) {
    char buf[item->sz], fullpath[MAX_PATH];
    FILE *infile, *outfile;
    int compress = 0;
    sprintf (fullpath, "%s%s%s", dir, dir? "/":"", &item->name);
    INFO ("extracting %s\n", fullpath);
    /* read magic to see if item is compressed */
    DID (fread(buf, 1, OFS, f_in));
    DID (!fseek(f_in, -OFS, SEEK_CUR));
    /* write compressed input to tmp file */
    if (strstr(buf, "7zXZ") == buf + 1) {
        compress = 1;
        outfile = tmpfile();
    } else {
        if (pipe) outfile = stdout;
        else DID (outfile = fopen(fullpath,"w"));
    } DID (fread(buf, 1, item->sz, f_in));
    DID (fwrite(buf, 1, item->sz, outfile));
    if (compress) {
        DID (!fseek(outfile, 0, SEEK_SET));
        infile = outfile;
        if (pipe) outfile = stdout;
        else DID (outfile = fopen(&item->name,"w"));
        lzma_stream strm = LZMA_STREAM_INIT;
        DID (init_decoder(&strm));
        DID (lzdecompress(&strm, &item->name, infile, outfile));
        lzma_end(&strm);
}   }
/** Get text catalog in buf and return file pointer.
 * name: open_archive
 * @param *name is a pointer to an archive name to open
 * @param *cat references a pointer to attach catalog list to
 * @return a file handle to the opened archive
 */
DLL_EXPORT FILE *open_archive(const char *name, ps_list *cat) {
    FILE *f_in;
    size_t sz;
    char buf[buf_MAX];
    /* open and read offset offset */
    DID (f_in=fopen(name, "r"));
    if (fseek(f_in, -OFS, SEEK_END)) {
        ERR ("%s unrecognized archive.\n", name);
    } DID (fread(buf, 1, OFS, f_in));
    if (buf[OFS-1]=='\n') buf[OFS-1]=0;
    DID (sz = strrchr(buf, '\n')+1 - buf);
    if (sz > OFS) {
        ERR ("%s does not appear to be a tape archive.\n", name);
    } DID (offset = atol(buf+sz));
    if (offset > cat_SZ) {
        ERR ("Catalogs > %i bytes not supported for now.\n", cat_SZ);
    } DID (!fseek(f_in, -(offset+OFS-sz), SEEK_END));
    DID (fread(buf, 1, offset, f_in));
    buf[offset] = 0;
    *cat = parse_catalog(buf);
    DID (!fseek(f_in, 0, SEEK_SET));
    return f_in;
}
/** Add an opened file to an opened archive.
 * name: attach_file
 * @param f_in is a pointer to an opened input file
 * @param f_out is a pointer to an opened output file
 * @param preset is the compression preset > 1 = compress
 * @return number of bytes written
 */
DLL_EXPORT size_t attach_file(FILE* f_in, FILE* f_out, uint32_t preset) {
    size_t sz = 0;
    char buf[buf_MAX];
    /* flatten only if compression preset is set > 1 */
    if (preset > 1) {
        lzma_stream strm = LZMA_STREAM_INIT;
        int success = init_encoder(&strm, preset);
        if (success) {
            sz = lzcompress(&strm, f_in, f_out);
            lzma_end(&strm);
    }   } else {
        while ((offset=fread(buf, 1, buf_MAX, f_in))) {
            fwrite (buf, 1, offset, f_out);
            sz += offset;
    }   } return sz;
}
/** Parse a text, commas catalog into linked list of names, sizes.
 * name: parse_catalog
 * @param *catalog is a pointer to catalog in text, commas format
 * @return catalog as allocated linked list structure of files, sizes
 * Catalog must be free()'d
 */
DLL_EXPORT ps_list parse_catalog(char *catalog) {
    /* return catalog as linked list (must be freed) */
    char name[MAX_PATH];
    size_t sz;
    ps_list cat, head = NULL;
    while (sscanf(catalog, "%[^,\n], %lu", name, &sz )==2) {
        catalog = strstr(catalog, "\n") + 1;
        if (!head) {
            head = malloc(sizeof *cat + MAX_PATH);
            cat = head;
        } else {
            cat->next = malloc(sizeof *cat + MAX_PATH);
            cat = cat->next;
        }            
        /* assign values to struct cat */
        cat->next = NULL;
        cat->sz = sz;
        strncpy(&cat->name, name, MAX_PATH);
        if (cat->next) puts ("shouldnt happen!");
    } return head;
}
/** Free (empty, cleanup) the catalog linked list structures.
 * name: free_catalog
 * @param cat is a pointer to tape catalog linked list to freedout
 */
DLL_EXPORT void free_catalog(ps_list cat) {
    ps_list next;
    do {
        next = cat->next;
        if (cat) free(cat);
    } while ((cat = next));
}
/** Append name, size to linked list structure.
 * name: free_catalog
 * @param *name is the file name
 * @param sz is the file size
 */
DLL_EXPORT void catalog_append(ps_list cat, char *name, size_t sz);
    /* fixme */

/** Print a help message.
 * name: help
 */
DLL_EXPORT void help(char **argv) {
    char *s = basename (argv[0]);
    printf ("Create playable archive: %s [options] [media] [attachments] ... output_media\n\n" 
    
            "   -c optionally compress attachments\n" 
            "      (media remains playable, and is never compressed).\n\n" 
            
            "Extraction: %s [options] [attachments] ... input_media:\n" 
            "   -l List media attachments\n" 
            "   -x eXtract [attachments] from input_media\n" 
            "   -o output to [directory]\n" 
            "   -p extract [attachments] from input_media to pipe\n" 
            "   -h Help\n", s, s);
    exit (EXIT_FAILURE);
} 