
// Copyright (c) 2008-2010, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <glib.h>

#include "cal-util.h"


// coverity[ -tainted_data_sink : arg-0 ]
int cal_peer_name_is_valid(const char *peer_name) {
    int i;

    if (peer_name == NULL) {
        g_log(CAL_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "cal_peer_name_is_valid(): NULL peer_name passed in");
        return 0;
    }

    for (i = 0; i < CAL_PEER_NAME_MAX_LENGTH; i ++) {
        if (peer_name[i] == (char)0) break;
        if (isprint(peer_name[i])) continue;
        g_log(CAL_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "cal_peer_name_is_valid(): byte at offset %d of peer_name \"%s\" is invalid", i, peer_name);
        return 0;
    }
    if (i == 0) {
        g_log(CAL_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "cal_peer_name_is_valid(): empty peer_name passed in");
        return 0;
    }
    if (i == CAL_PEER_NAME_MAX_LENGTH) {
        g_log(CAL_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "cal_peer_name_is_valid(): passed-in peer_name is too long (max %d)", CAL_PEER_NAME_MAX_LENGTH);
        return 0;
    }

    // it's ok
    return 1;
}

