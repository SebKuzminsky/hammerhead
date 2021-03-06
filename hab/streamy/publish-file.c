
// Copyright (c) 2008-2010, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <glib.h>

#include "hardware-abstractor.h"
#include "streamy-hab.h"


int publish_file(void *stream_as_voidp) {
    bionet_stream_t *stream = (bionet_stream_t *)stream_as_voidp;
    char *filename;
    FILE *f;

    filename = bionet_stream_get_user_data(stream);
    f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "error opening '%s' for read: %s\n", filename, strerror(errno));
        return FALSE;
    }

    do {
        char buf[100];
        int r;

        r = fread(buf, 1, sizeof(buf), f);
        if (r <= 0) break;

        hab_publish_stream(stream, buf, r);
        if(bandwidth_limit > 0){
            //fprintf(stderr, "Sleeping %lf usec for %d bytes\n", 
            //    (r/bandwidth_limit * (1e6/1024)), r);
            g_usleep(r/bandwidth_limit * (1e6/1024));
        }
    } while(1);

    fclose(f);

    return FALSE;
}

