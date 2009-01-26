
//
// Copyright (C) 2008-2009, Regents of the University of Colorado.
//


#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "hardware-abstractor.h"
#include "streamy-hab.h"


void cb_stream_data(const char *client_id, bionet_stream_t *stream, const void *data, unsigned int size) {
    const char *filename = bionet_stream_get_local_name(stream);
    FILE *f;
    int r;

    f = fopen(filename, "a");
    if (f == NULL) {
        fprintf(stderr, "error opening '%s' for append: %s\n", filename, strerror(errno));
        return;
    }

    r = fwrite(data, 1, size, f);
    fclose(f);
    if (r != size) {
        fprintf(stderr, "error appending to '%s'\n", filename);
        return;
    }

    printf("client '%s' sent %d bytes to stream %s\n", client_id, size, filename);
}
