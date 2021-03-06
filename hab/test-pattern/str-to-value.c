
// Copyright (c) 2008-2010, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "bionet-util.h"

bionet_value_t *str_to_value(bionet_resource_t *resource, bionet_resource_data_type_t data_type, char *str) {
    bionet_value_t *value;

    switch (data_type) {
        case BIONET_RESOURCE_DATA_TYPE_BINARY:
            value = bionet_value_new_binary(resource, atoi(str));
            break;

        case BIONET_RESOURCE_DATA_TYPE_UINT8:
            value = bionet_value_new_uint8(resource, atoi(str));
            break;

        case BIONET_RESOURCE_DATA_TYPE_UINT16:
            value = bionet_value_new_uint16(resource, atoi(str));
            break;

        case BIONET_RESOURCE_DATA_TYPE_UINT32:
            value = bionet_value_new_uint32(resource, (uint32_t)strtoul(str, NULL, 10));
            break;

        case BIONET_RESOURCE_DATA_TYPE_INT8:
            value = bionet_value_new_int8(resource, atoi(str));
            break;

        case BIONET_RESOURCE_DATA_TYPE_INT16:
            value = bionet_value_new_int16(resource, atoi(str));
            break;

        case BIONET_RESOURCE_DATA_TYPE_INT32:
            value = bionet_value_new_int32(resource, atoi(str));
            break;

        case BIONET_RESOURCE_DATA_TYPE_FLOAT:
            value = bionet_value_new_float(resource, atof(str));
            break;

        case BIONET_RESOURCE_DATA_TYPE_DOUBLE:
            value = bionet_value_new_double(resource, strtod(str, NULL));
            break;

        case BIONET_RESOURCE_DATA_TYPE_STRING: {
            value = bionet_value_new_str(resource, str);
            break;
        }

        default: 
            g_log("", G_LOG_LEVEL_WARNING, "unable to translate create bionet_value_t* from '%s'\n", str);
            value = NULL;
            break;
    }

    return value;
}
