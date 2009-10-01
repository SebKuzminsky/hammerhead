
// Copyright (c) 2008-2009, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/time.h>

#include <glib.h>

#include "bionet-util.h"
#include "hardware-abstractor.h"

#include "libhab-internal.h"
#include "bionet-asn.h"




int hab_report_datapoints(const bionet_node_t *node) {
    int ri;

    for (ri = 0; ri < bionet_node_get_num_resources(node); ri ++) {
        bionet_asn_buffer_t buf;
        int r;
        char topic[(BIONET_NAME_COMPONENT_MAX_LEN * 2) + 2];
        bionet_resource_t *resource;

        resource = bionet_node_get_resource_by_index(node, ri);
        if (!bionet_resource_is_dirty(resource)) continue;

        // send dirty datapoints only
        r = bionet_resource_datapoints_to_asnbuf(resource, &buf, 1);
        if (r != 0) continue;

        bionet_resource_make_clean(resource);

        snprintf(topic, sizeof(topic), 
		 "D %s", bionet_resource_get_local_name(resource));

        // publish the message to any connected subscribers
        cal_server.publish(topic, buf.buf, buf.size);

        // FIXME: cal_server.publish should take the buf
        free(buf.buf);
    }

    // FIXME: should be void maybe
    return 0;
}
