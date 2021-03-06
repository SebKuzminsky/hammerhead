
// Copyright (c) 2008-2010, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "hardware-abstractor.h"

extern int num_listeners;


void cb_stream_unsubscription(const char *client_id, const bionet_stream_t *stream) {
    fprintf(stderr, "subscriber left: %s\n", client_id);
    num_listeners--;
}

