
//
// Copyright (C) 2008-2009, Gary Grobe and the Regents of the University of Colorado.
//


#ifndef __PAL_650_HAB_H
#define __PAL_650_HAB_H


#include <glib.h>
#include <hardware-abstractor.h>


#define Max(a, b)  ((a) > (b) ? (a) : (b))


typedef struct {
    time_t time;
} node_data_t;


extern bionet_hab_t *hab;

extern char *pal_ip;
extern int pal_port;
extern int tag_timeout;
extern int show_messages;


int pal_connect(const char *pal_ip, const int pal_port);
int pal_read(int pal_fd);
int pal_request_taglist(int pal_fd);


void timeout_tags(void);
void drop_node(gpointer node);


// If the PAL-650 had any actuators we wanted to expose, this callback would 
// let us know when someone wanted to change them.
void cb_set_resource(bionet_resource_t *resource, bionet_value_t *value);


void make_shutdowns_clean(void);


#endif 

