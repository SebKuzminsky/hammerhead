
//
// Copyright (C) 2008, Regents of the University of Colorado.
//


#include <glib.h>

#include "libhab-internal.h"
#include "cal-server.h"
#include "bionet-asn.h"


void libhab_cal_callback(const cal_event_t *event) {
    switch (event->type) {
        case CAL_EVENT_CONNECT: {
            g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_INFO, "CAL Connect event from '%s'", event->peer_name);
            break;
        }

        case CAL_EVENT_DISCONNECT: {
            g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_INFO, "CAL Disconnect event from '%s'", event->peer_name);
            break;
        }

        case CAL_EVENT_MESSAGE: {
            g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_INFO, "CAL Message event from '%s'", event->peer_name);
            break;
        }

        case CAL_EVENT_SUBSCRIBE: {
            GSList *i;

            g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_INFO, "CAL Subscribe event from '%s' for topic '%s'", event->peer_name, event->topic);

            for (i = libhab_this->nodes; i != NULL; i = i->next) {
                bionet_asn_buffer_t buf;
                bionet_node_t *node = i->data;
                int r;

                if (libhab_cal_topic_matches(node->id, event->topic) != 0) continue;

                r = bionet_node_to_asnbuf(node, &buf);
                if (r != 0) {
                    // an error has already been logged, and the buffer has been freed
                    continue;
                }

                // "publish" the message to the newly connected subscriber (via sendto)
                // cal_server.sendto takes the buf so we dont need to free it
                cal_server.sendto(event->peer_name, buf.buf, buf.size);

                g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_INFO, "node %s matches topic %s", node->id, event->topic);
            }
            
            break;
        }

        default: {
            printf("unhandled CAL event type %d\n", event->type);
            break;
        }
    }
}

