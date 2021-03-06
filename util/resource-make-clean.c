
// Copyright (c) 2008-2010, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#include "internal.h"
#include "bionet-util.h"


void bionet_resource_make_clean(bionet_resource_t *resource) {
    int di;

    for (di = 0; di < bionet_resource_get_num_datapoints(resource); di ++) {
        bionet_datapoint_t *d;

        d = bionet_resource_get_datapoint_by_index(resource, di);
        bionet_datapoint_make_clean(d);
    }
}

// Emacs cruft
// Local Variables:
// mode: C
// c-file-style: "Stroustrup"
// End:
