
//
// Copyright (C) 2008, Regents of the University of Colorado.
//


#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "bionet-data-manager.h"

#include "bdm-util.h"




void disconnect_client(client_t *client) {
    g_log(BDM_LOG_DOMAIN, G_LOG_LEVEL_INFO, "disconnecting client");

    if (client->ch != NULL) {
        g_io_channel_shutdown(client->ch, FALSE, NULL);
        g_io_channel_unref(client->ch);
    }

    if (client->message != NULL) {
        ASN_STRUCT_FREE(asn_DEF_BDM_C2S_Message, client->message);
    }

    free(client);
}
