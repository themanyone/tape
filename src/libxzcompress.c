///////////////////////////////////////////////////////////////////////////////
//
/// \file       01_compress_easy.c
/// \brief      Compress from stdin to stdout in multi-call mode
///
/// Usage:      ./01_compress_easy PRESET < INFILE > OUTFILE
///
/// Example:    ./01_compress_easy 6 < foo > foo.xz
//
//  Author:     Lasse Collin
//
//  This file has been put into the public domain.
//  You can do whatever you want with this file.
//
///////////////////////////////////////////////////////////////////////////////

#include "lz.h"

bool init_encoder(lzma_stream *strm, uint32_t preset)
{
    // Initialize the encoder using a preset. Set the integrity to check
    // to CRC64, which is the default in the xz command line tool. If
    // the .xz file needs to be decompressed with XZ Embedded, use
    // LZMA_CHECK_CRC32 instead.
    lzma_ret ret = lzma_easy_encoder(strm, preset, LZMA_CHECK_CRC64);

    // Return successfully if the initialization went fine.
    if (ret == LZMA_OK)
        return true;

    // Something went wrong. The possible errors are documented in
    // lzma/container.h (src/liblzma/api/lzma/container.h in the source
    // package or e.g. /usr/include/lzma/container.h depending on the
    // install prefix).
    const char *msg;
    switch (ret) {
    case LZMA_MEM_ERROR:
        msg = "Memory allocation failed";
        break;

    case LZMA_OPTIONS_ERROR:
        msg = "Specified preset is not supported";
        break;

    case LZMA_UNSUPPORTED_CHECK:
        msg = "Specified integrity check is not supported";
        break;

    default:
        // This is most likely LZMA_PROG_ERROR indicating a bug in
        // this program or in liblzma. It is inconvenient to have a
        // separate error message for errors that should be impossible
        // to occur, but knowing the error code is important for
        // debugging. That's why it is good to print the error code
        // at least when there is no good error message to show.
        msg = "Unknown error, possibly a bug";
        break;
    }

    fprintf(stderr, "Error initializing the encoder: %s (error code %u)\n",
            msg, ret);
    return false;
}

size_t
lzcompress(lzma_stream *strm, FILE *infile, FILE *outfile)
{
    // This will be LZMA_RUN until the end of the input file is reached.
    // This tells lzma_code() when there will be no more input.
    lzma_action action = LZMA_RUN;

    // Buffers to temporarily hold uncompressed input
    // and compressed output.
    uint8_t inbuf[BUFSIZ];
    uint8_t outbuf[BUFSIZ];

    // Initialize the input and output pointers. Initializing next_in
    // and avail_in isn't really necessary when we are going to encode
    // just one file since LZMA_STREAM_INIT takes care of initializing
    // those already. But it doesn't hurt much and it will be needed
    // if encoding more than one file like we will in 02_decompress.c.
    //
    // While we don't care about strm->total_in or strm->total_out in this
    // example, it is worth noting that initializing the encoder will
    // always reset total_in and total_out to zero. But the encoder
    // initialization doesn't touch next_in, avail_in, next_out, or
    // avail_out.
    strm->next_in = NULL;
    strm->avail_in = 0;
    strm->next_out = outbuf;
    strm->avail_out = sizeof(outbuf);

    // Loop until the file has been successfully compressed or until
    // an error occurs.
    while (true) {
        // Fill the input buffer if it is empty.
        if (strm->avail_in == 0 && !feof(infile)) {
            strm->next_in = inbuf;
            strm->avail_in = fread(inbuf, 1, sizeof(inbuf),
                    infile);

            if (ferror(infile)) {
                fprintf(stderr, "Read error: %s\n",
                        strerror(errno));
                return 0;
            }

            // Once the end of the input file has been reached,
            // we need to tell lzma_code() that no more input
            // will be coming and that it should finish the
            // encoding.
            if (feof(infile))
                action = LZMA_FINISH;
        }

        // Tell liblzma do the actual encoding.
        //
        // This reads up to strm->avail_in bytes of input starting
        // from strm->next_in. avail_in will be decremented and
        // next_in incremented by an equal amount to match the
        // number of input bytes consumed.
        //
        // Up to strm->avail_out bytes of compressed output will be
        // written starting from strm->next_out. avail_out and next_out
        // will be incremented by an equal amount to match the number
        // of output bytes written.
        //
        // The encoder has to do internal buffering, which means that
        // it may take quite a bit of input before the same data is
        // available in compressed form in the output buffer.
        lzma_ret ret = lzma_code(strm, action);

        // If the output buffer is full or if the compression finished
        // successfully, write the data from the output bufffer to
        // the output file.
        if (strm->avail_out == 0 || ret == LZMA_STREAM_END) {
            // When lzma_code() has returned LZMA_STREAM_END,
            // the output buffer is likely to be only partially
            // full. Calculate how much new data there is to
            // be written to the output file.
            size_t write_size = sizeof(outbuf) - strm->avail_out;

            if (fwrite(outbuf, 1, write_size, outfile)
                    != write_size) {
                fprintf(stderr, "Write error: %s\n",
                        strerror(errno));
                return 0;
            }

            // Reset next_out and avail_out.
            strm->next_out = outbuf;
            strm->avail_out = sizeof(outbuf);
        }

        // Normally the return value of lzma_code() will be LZMA_OK
        // until everything has been encoded.
        if (ret != LZMA_OK) {
            // Once everything has been encoded successfully, the
            // return value of lzma_code() will be LZMA_STREAM_END.
            //
            // It is important to check for LZMA_STREAM_END. Do not
            // assume that getting ret != LZMA_OK would mean that
            // everything has gone well.
            if (ret == LZMA_STREAM_END)
                return strm->total_out;

            // It's not LZMA_OK nor LZMA_STREAM_END,
            // so it must be an error code. See lzma/base.h
            // (src/liblzma/api/lzma/base.h in the source package
            // or e.g. /usr/include/lzma/base.h depending on the
            // install prefix) for the list and documentation of
            // possible values. Most values listen in lzma_ret
            // enumeration aren't possible in this example.
            const char *msg;
            switch (ret) {
            case LZMA_MEM_ERROR:
                msg = "Memory allocation failed";
                break;

            case LZMA_DATA_ERROR:
                // This error is returned if the compressed
                // or uncompressed size get near 8 EiB
                // (2^63 bytes) because that's where the .xz
                // file format size limits currently are.
                // That is, the possibility of this error
                // is mostly theoretical unless you are doing
                // something very unusual.
                //
                // Note that strm->total_in and strm->total_out
                // have nothing to do with this error. Changing
                // those variables won't increase or decrease
                // the chance of getting this error.
                msg = "File size limits exceeded";
                break;

            default:
                // This is most likely LZMA_PROG_ERROR, but
                // if this program is buggy (or liblzma has
                // a bug), it may be e.g. LZMA_BUF_ERROR or
                // LZMA_OPTIONS_ERROR too.
                //
                // It is inconvenient to have a separate
                // error message for errors that should be
                // impossible to occur, but knowing the error
                // code is important for debugging. That's why
                // it is good to print the error code at least
                // when there is no good error message to show.
                msg = "Unknown error, possibly a bug";
                break;
            }

            fprintf(stderr, "Encoder error: %s (error code %u)\n",
                    msg, ret);
            return 0;
        }
    }
}
