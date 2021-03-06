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
#ifdef WITH_LZMA
#include "lz.h"
#endif
/** List contents of [archive name].
 * name: list_archive
 * @param *name is the playable archive to examine
 */
void list_archive(const char *name, int hr) {
    ps_list cat = NULL;
    size_t fs;
    int i = 0;
    double f;
    char sizes[] = " KMGT";
    FILE *f_in = open_archive (name, &cat);
    printf ("Contents of %s:\n", name);
    FOR_IN (item, cat) {
        f = fs = item->sz;
        if (hr) {
            for (i=0;i<4;i++) if (!(fs / (size_t)pow(1024, i+1))) break;
            f = fs >= 1024 ? roundf(fs / pow(1024, i) * 10.0 ) / 10.0 : fs;
        } printf ("%.*lf%c\t%s\n", hr, f, sizes[i], &item->name);
    } fclose(f_in);
    free_catalog (cat);
}
/** Open named archive and extract a list of files.
 * name: extract_list
 * @param *f_in is an opened file handle to extract from
 * @param files is a pointer to linked list of files to extract
 * @param *dir is an optional output directory to send files to
 * @param pipe is an integer flag 1=pipe to stdout
 */
void extract_list(FILE *f_in, ps_list files, char *dir, int pipe) {
    /* go through catalog list */
    FOR_IN (item, files) extract_next (f_in, item, dir, pipe);
    /* Close stdout to catch possible write errors */
    if (pipe && fclose(stdout)) {
        INFO ("Write errors: %s\n", strerror(errno));
}   }
/** Extract an item from *f_in, or optionally pipe to stdout.
 * If item->ex == 0 this routine will skip extraciton.
 * name: extract_next
 * @param *f_in is an opened file handle to extract from
 * @param item points to structure with name, file size to extract
 * If item->ex == 0 no extraction will be performed.
 * @param *dir is an optional output directory to send files to
 * @param pipe = 0 to extract to file, 1 to pipe to stdout
 */
void extract_next (FILE *f_in, ps_list item, char *dir, int pipe) {
    char *buf, fullpath[MAX_PATH];
    FILE *outfile;
    int compress = 0;
    if (!item->ex) {
        /* skip extraction */
        fseek (f_in, item->sz, SEEK_CUR);
        INFO ("skipping %s\n", fullpath);
        return;
    } if (dir && !pipe) sprintf (fullpath, "%s/%s", dir, &item->name);
    else sprintf (fullpath, "%s", &item->name);
    INFO ("extracting %s\n", fullpath);
    DID (buf = malloc(item->sz));
    /* read magic to see if item is compressed */
    DID (fread(buf, 1, OFS, f_in));
    DID (!fseek(f_in, -OFS, SEEK_CUR));
    /* write compressed input to tmp file */
    if (strstr(buf, "7zXZ") == buf + 1) {
        compress = 1;
        outfile = tmpfile();
    } else {
        if (pipe) outfile = stdout;
        else DID (outfile = fopen(fullpath,"wb"));
    } DID (fread(buf, 1, item->sz, f_in));
    DID (fwrite(buf, 1, item->sz, outfile));
    free (buf);
#ifdef WITH_LZMA
 if (compress) {
        DID (!fseek(outfile, 0, SEEK_SET));
        FILE *infile = outfile;
        if (pipe) outfile = stdout;
        else DID (outfile = fopen(item->name,"wb"));
        lzma_stream strm = LZMA_STREAM_INIT;
        DID (init_decoder(&strm));
        DID (lzdecompress(&strm, item->name, infile, outfile));
        lzma_end(&strm);
    }
#else
    INFO ("Compression not supported. Build with --with-xz=yes.\n");
#endif
 return;
}
/** Get text catalog in buf and return file pointer.
 * name: open_archive
 * @param *name is a pointer to an archive name to open
 * @param *cat references a pointer to attach catalog list to
 * @return a file handle to the opened archive
 */
FILE *open_archive(const char *name, ps_list *cat) {
    FILE *f_in;
    size_t sz;
    char buf[buf_MAX] = {0}, *s;
    /* open and read offset offset */
    DID (f_in=fopen(name, "rb"));
    if (fseek(f_in, -OFS, SEEK_END)) {
        ERR ("%s unrecognized archive.\n", name);
    } DID (fread(buf, 1, OFS, f_in));
    if ((s = strrchr(buf, '\n'))) *s = 0;
    if ((s = strrchr(buf, '\n'))) sz = s + 1 - buf;
    else ERR ("%s does not appear to be a tape archive.\n", name);
    offset = atol(buf + sz);
    if (!offset || offset > cat_SZ) {
        ERR ("Catalogs > %i bytes not supported for now.\n", cat_SZ);
    } DID (!fseek(f_in, -(offset + OFS - sz), SEEK_END));
    DID (fread(buf, 1, offset, f_in));
    buf[offset] = 0;
    *cat = parse_catalog(buf);
    DID (!fseek(f_in, 0, SEEK_SET));
    return f_in;
}
/** Add an opened file to an opened archive.
 * name: attach_file
 * @param *f_out is a pointer to an opened output file
 * @param *name is the name of input file to attach
 * @param preset is the compression preset > 1 = compress
 * @return number of bytes written
 */
size_t attach_file(FILE *f_out, char *name, unsigned int preset) {
    size_t sz = 0;
    char buf[buf_MAX];
    FILE *f_in;
    /* can the file be opened for reading? */
    INFO ("Adding %s\n", name);
    DID (f_in = fopen(name, "rb"));
    /* flatten only if compression preset is set > 1 */
    #ifdef WITH_LZMA
 if (preset > 1) {
        lzma_stream strm = LZMA_STREAM_INIT;
        int success = init_encoder(&strm, preset);
        if (success) {
            sz = lzcompress (&strm, f_in, f_out);
            lzma_end(&strm);
    }   }
    #endif
    if (preset < 2) {
        while ((offset=fread(buf, 1, buf_MAX, f_in))) {
            fwrite (buf, 1, offset, f_out);
            DID (!ferror(f_out));
            sz += offset;
    }   }    /* now close input file */
    DID (!fclose(f_in));
    return sz;
}
/** Parse a text, commas catalog into linked list of names, sizes.
 * name: parse_catalog
 * @param *catalog is a pointer to catalog in text, commas format
 * @return a catalog linked list structure of files, sizes
 * Call free_catalog() on it when done!
 */
ps_list parse_catalog(char *catalog) {
    /* return catalog as linked list (must be freed) */
    char name[MAX_PATH];
    size_t sz;
    ps_list cat = NULL;
    while (sscanf(catalog, "%[^,\n], %lu", name, &sz )==2) {
        catalog = strstr(catalog, "\n") + 1;
        catalog_append (&cat, name, sz);
    } return cat;
}
/** Free (dispose of) the catalog linked list structures.
 * Call before program termination to prevent a memory leak.
 * name: free_catalog
 * @param cat is a pointer to catalog linked list to dispose of
 */
void free_catalog(ps_list cat) {
    ps_list next;
    if (!cat) return;
    do {
        next = cat->next;
        free (cat);
    } while ((cat = next));
}
/** Append name, size to linked list structure ref.
 * name: catalog_append
 * @param *pcat reference pointer to catalog linked list structure
 * @param *name is the file name string to append
 * @param sz is the file size
 */
void catalog_append(ps_list *pcat, char *name, size_t sz) {
    ps_list cat = *pcat;
    if (!cat) cat = *pcat = malloc(sizeof *cat);
    else {
        while (cat->next) cat = cat->next;
        cat->next = malloc(sizeof *cat);
        cat = cat->next;
    } cat->next = NULL;
    cat->sz = sz;
    cat->ex = 1;
    strncpy(cat->name, name, MAX_PATH);
    return; 
}
/** Write tape from array of file names.
 * name: tape_from_array
 * @param last is an integer specifying array's last element
 * @param **argv is the file name array
 * @param compress is an integer 1 = compress files
 */
void tape_from_array(int last, char **argv, int compress) {
    size_t file_size[255];
    int this_file, option = 0;
    char *e1, *e2;
    FILE *f_out;
    e1 = strrchr (argv[last], '.');
    e2 = strrchr (argv[option], '.');
    if (!e1||!e2||strcmp(e1, e2)) INFO ("Warning: First and last" 
    " files have different extensions.\nMedia might not play.\n");
    /* open last file for append in binary mode? */
    DID (f_out=fopen(argv[last],"wb+"));
    /* yes, let's iterate through each file */
    for (this_file=option; this_file < last; this_file++) {
        sz = attach_file (f_out, argv[this_file], compress);
        /* keep track of file sizes */
        file_size[this_file] = sz;
        if (compress == 1) compress = 6;
    }    /* write catalog */
    sz = 0;
    for (this_file=option; this_file < last; this_file++) {
        sz += fprintf (f_out, "%s, %lu\n", argv[this_file],
        file_size[this_file]);
    }    /* write catalog size */
    fprintf (f_out, "%lu\n", sz);
    /* close output file */
    DID (!fclose(f_out));
} 