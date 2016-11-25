/* libcar.c * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*                                                                      *
* Create playable multimedia archives. Compatible with current players!*
*                                                                      *
* Copyright (C) 2016 by Henry Kroll III, www.thenerdshow.com
* 
                             Apache License
                       Version 2.0, January 2004
                    http://www.apache.org/licenses/ 

*/
#include "car.h"
size_t sz, offset;
int  pipe = 0, compress = 0;
typedef struct cat {
    struct cat *next;
    size_t sz;
    char name;
} s_cat, *ps_cat;
/* attach text catalog to buf and return file pointer */
DLL_EXPORT FILE *getfile(const char *name, char *buf) {
    FILE *f_in;
    size_t sz;
    /* open and read offset offset */
    DID (f_in=fopen(name, "r"));
    if (fseek(f_in, -OFS, SEEK_END)) {
        ERR ("%s unrecognized archive.\n",name);
    } DID (fread(buf, 1, OFS, f_in));
    if (buf[OFS-1]=='\n') buf[OFS-1]=0;
    DID (sz = strrchr(buf, '\n')+1 - buf);
    if (sz > OFS) {
        ERR ("%s does not appear to be a car file.\n", name);
    } DID (offset = atol(buf+sz));
    if (offset > cat_SZ) {
        ERR ("Catalogs > %i bytes not supported at the moment.\n", cat_SZ);
    } DID (!fseek(f_in, -(offset+OFS-sz), SEEK_END));
    DID (fread(buf, 1, offset, f_in));
    return f_in;
}
/* attach a file to the media archive */
DLL_EXPORT size_t attach_file(FILE* f_in, FILE* f_out) {
    size_t sz = 0;
    char buf[buf_MAX];
    /* compress 2nd file on up */
    if (compress==2) {
        uint32_t preset = 9;
        lzma_stream strm = LZMA_STREAM_INIT;
        int success = init_encoder(&strm, preset);
        if (success) {
            sz = lzcompress(&strm, f_in, f_out);
            lzma_end(&strm);
    }   } else {
        while ((offset=fread(buf, 1, buf_MAX, f_in))) {
            fwrite (buf, 1, offset, f_out);
            sz += offset;
    }   } if (compress == 1) compress = 2;
    return sz;
} static ps_cat parse_catalog(char *catalog) {
    /* return catalog as linked list (must be freed) */
    char name[MAX_PATH];
    size_t sz;
    ps_cat cat, head = NULL;
    while (sscanf(catalog, "%[^,\n], %lu", name, &sz )==2) {
        catalog = strstr(catalog, "\n") + 1;
        if (!head) {
            head = malloc(sizeof cat + MAX_PATH);
            cat = head;
        } else {
            cat->next = malloc(sizeof cat + MAX_PATH);
            cat = cat->next;
        }            
        /* assign values to struct cat */
        cat->next = NULL;
        cat->sz = sz;
        strcpy(&cat->name, name);
    } return head;
} void extract (ps_cat item, FILE *f_in) {
    char buf[item->sz];
    FILE *infile, *outfile;
    INFO ("extracting %s\n", &item->name);
    DID (fread(buf, 1, OFS, f_in));
    DID (!fseek(f_in, -OFS, SEEK_CUR));
    /* write compressed input to tmp file */
    if (strstr(buf, "7zXZ") == buf+1) {
        compress = 1;
        outfile = tmpfile();
    } else {
        if (pipe) outfile = stdout;
        else DID (outfile = fopen(&item->name,"w"));
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
    } DID (fclose(outfile));
} void freecatalog(ps_cat cat) {
    ps_cat next;
    do {
        next = cat->next;
        if (cat) free(cat);
    } while ((cat = next));
} void help(char **argv) {
    char *s = basename (argv[0]);
    printf ("Create playable archive: %s [options] [media] [attachments] ... output_media\n\n" 
    
            "   -c optionally compress attachments\n" 
            "      (media remains playable, and is never compressed).\n\n" 
            
            "Extraction: %s [options] [attachments] ... input_media:\n" 
            "   -l List media attachments\n" 
            "   -x eXtract [attachments] from input_media\n" 
            "   -p extract [attachments] from input_media to pipe\n" 
            "   -h Help\n", s, s);
    exit (EXIT_FAILURE