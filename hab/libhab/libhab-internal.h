
//
// Copyright (C) 2008, Regents of the University of Colorado.
//


#ifndef __LIBHAB_INTERNAL_H
#define __LIBHAB_INTERNAL_H




#include <glib.h>

#include "cal-server.h"
#include "bionet-util.h"




extern int libhab_cal_fd;

extern bionet_hab_t *libhab_this;




int libhab_cal_topic_matches(const char *topic, const char *subscription);

void libhab_cal_callback(const cal_event_t *event);


extern void (*libhab_callback_set_resource)(bionet_resource_t *resource, bionet_value_t *value);
extern void (*libhab_callback_stream_subscription)(const char *client_id, const bionet_stream_t *stream);
extern void (*libhab_callback_stream_unsubscription)(const char *client_id, const bionet_stream_t *stream);
extern void (*libhab_callback_lost_client)(const char *client_id);



#endif // __LIBHAB_INTERNAL_H


