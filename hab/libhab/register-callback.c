
//
// Copyright (C) 2008, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G and NNC06CB40C.
//


#include "hardware-abstractor.h"
#include "libhab-internal.h"

void hab_register_callback_set_resource(void (*cb_set_resource)(bionet_resource_t *resource, bionet_value_t *value)) {
    libhab_callback_set_resource = cb_set_resource;
}


void hab_register_callback_lost_client(void (*cb_lost_client)(const char *client_id)) {
    libhab_callback_lost_client = cb_lost_client;
}

