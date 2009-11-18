
// Copyright (c) 2008-2009, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#ifndef BIONET_DATA_MANAGER_H
#define BIONET_DATA_MANAGER_H

#include "config.h"

#include <glib.h>

#include "bionet.h"

#include "bionet-asn.h"
#include "bionet-util.h"
#include "cal-event.h"

#if ENABLE_ION
#include "zco.h"
#include "sdr.h"
#include "bp.h"
#endif


#include <sqlite3.h>

// Number of bytes to use for the resource key
// from the sha1 hash. Must be <= to SHA_DIGEST_LENGTH;
#define BDM_RESOURCE_KEY_LENGTH 8 

//
// The bionet code uses glib's logging facility to log internal messages,
// and this is the domain.
//

#define  BDM_LOG_DOMAIN  "bdm"

//
// bdm server listens on this TCP port, clients connect
//

#define BDM_PORT      (11002)
#define BDM_SYNC_PORT (11003)



extern bionet_bdm_t * this_bdm;
extern char *bdm_pidfile;
extern GMainLoop *bdm_main_loop;

extern GHashTable * bdm_opts_table;


// Global flag. Set true to stop all auxillary threads
extern int bdm_shutdown_now;

// Global CAL vars
extern int libbdm_cal_fd;
extern void libbdm_cal_callback(const cal_event_t *event);
extern int libbdm_cal_topic_matches(const char * topic, const char *subscription);

// Call from threads instead of sleep.
// Returns early with non-zero status if thread should exit
int bdm_thread_sleep(long usec);

typedef enum {
    DB_INT = 0,
    DB_DOUBLE,
    DB_STRING
} db_type_t;

typedef union {
    int i;
    double d;
    char * str;
} db_value_t;

typedef struct {
    const char * bdm_id;
    struct timeval timestamp;
    db_type_t type;
    db_value_t value;
} bdm_datapoint_t;

typedef struct {
    struct timeval entry_ts;
    bionet_datapoint_t * datapoint;
} bdm_record_t;

typedef enum {
    BDM_SYNC_METHOD_TCP = 0,
    BDM_SYNC_METHOD_ION = 1,
} bdm_sync_method_t;

typedef struct {
    bdm_sync_method_t method;  // The method used to send
    struct timeval start_time; // Start-time datapoint filter
    struct timeval end_time;   // End-time datapoint filter
    char resource_name_pattern[BIONET_NAME_COMPONENT_MAX_LEN * 4];
    unsigned int frequency;
    char * sync_recipient;
    int remote_port;
#if ENABLE_ION
    // Group ion configs for clarity
    struct {
        int basekey; // ION base key, to allow multiple instance on a machine
        Sdr	sdr; // The SDR of the open transaction. NULL when not in transaction
        BpSAP   sap;      // Sender endpoint ID
        Object	bundleZco; // The ZCO that is being appended to.
        size_t  bundle_size; // Running total of all extents appended to bundle
    } ion;
#endif

    //State vars
    sqlite3 *db;
    int last_entry_end_seq;    
    int last_entry_end_seq_metadata; 
    // TCP Specific
    int fd;
    int bytes_sent;
} sync_sender_config_t;

// 
// interface to the database backend
//


//
// Main thread db handle. Used by bionet callbacks
//
extern sqlite3 * main_db;

//
// set up the database
//
// return 0 on success, -1 on failure
//
sqlite3 * db_init(void);


// 
// shut down the database
//

void db_shutdown(sqlite3 *db);

//
// Make a resource key
//
int db_make_resource_key(
    const char * hab_type,
    const char * hab_id,
    const char * node_id,
    const char * resource_id,
    bionet_resource_data_type_t data_type,
    bionet_resource_flavor_t flavor,
    uint8_t resource_key[BDM_RESOURCE_KEY_LENGTH]);

//
// Each of these functions inserts a bionet object and, if needed, all its
// parent objects.  These functions are idempotent.
//
// Return 0 on success, -1 on failure.
//

int db_add_datapoint(sqlite3 *db, bionet_datapoint_t *datapoint);
int db_add_resource(sqlite3 *db, bionet_resource_t *resource);
int db_add_node(sqlite3 *db, bionet_node_t *node);
int db_add_hab(sqlite3 *db, bionet_hab_t *hab);
int db_add_bdm(sqlite3 *db, const char * bdm_id);


//
// Insert a datapoint, possibly before the metadata is available
//
int db_add_datapoint_sync(
    sqlite3 *db,
    uint8_t resource_key[BDM_RESOURCE_KEY_LENGTH],
    bdm_datapoint_t * dp);


// 
// Finds all matching datapoints, and returns them as a GPtrArray of bionet_bdm_t.
// Use bdm_list_free to free the list returned
//
GPtrArray *db_get_resource_datapoints(
    sqlite3 *db,
    const char *hab_type,
    const char *hab_id,
    const char *node_id,
    const char *resource_id,
    struct timeval *datapoint_start,
    struct timeval *datapoint_end,
    int entry_start,
    int entry_end
);

// 
// Finds all matching metadata, and returns them as a GPtrArray of bionet_bdm_t.
// Use bdm_list_free to free the list returned
//
GPtrArray *db_get_metadata(
    sqlite3 *db,
    const char *hab_type,
    const char *hab_id,
    const char *node_id,
    const char *resource_id,
    struct timeval *datapoint_start,
    struct timeval *datapoint_end,
    int entry_start,
    int entry_end);

//
// Free the list returned from db_get_metadata or db_get_resource_datapoints
//
void bdm_list_free(GPtrArray *bdm_list);

int db_get_latest_entry_seq(sqlite3 *db);
int db_get_last_sync_seq_metadata(sqlite3 *db, char * bdm_id);
void db_set_last_sync_seq_metadata(sqlite3 *db, char * bdm_id, int seq);
int db_get_last_sync_seq_datapoints(sqlite3 *db, char * bdm_id);
void db_set_last_sync_seq_datapoints(sqlite3 *db, char * bdm_id, int seq);

//
// Stuff to handle messages and asn
//
void libbdm_handle_resourceDatapointsQuery(
        const char * peer_name, 
        ResourceDatapointsQuery_t *rdpq);

BDM_Sync_Message_t * bdm_sync_metadata_to_asn(GPtrArray *bdm_list);
BDM_Sync_Message_t * bdm_sync_datapoints_to_asn(GPtrArray *bdm_list);


int bdm_report_datapoint(
        bionet_bdm_t * bdm,
        bionet_resource_t * resource,
        bionet_datapoint_t * datapoint,
        int entry_seq) ;

int bdm_report_new_node(
        bionet_bdm_t * bdm,
        bionet_node_t * node,
        int entry_seq) ;

int bdm_report_new_hab(
        bionet_bdm_t * bdm,
        bionet_hab_t * hab,
        int entry_seq) ;

// 
// stuff for being a bionet client
//

#define MAX_SUBSCRIPTIONS 100
extern int hab_list_index;
extern char *hab_list_name_patterns[MAX_SUBSCRIPTIONS];

extern int node_list_index;
extern char *node_list_name_patterns[MAX_SUBSCRIPTIONS];

extern int resource_index;
extern char *resource_name_patterns[MAX_SUBSCRIPTIONS];

extern char * security_dir;
extern int require_security;

int try_to_connect_to_bionet(void *unused);




// 
// stuff to actually be BDM
//


typedef struct {
    int fd;
    GIOChannel *ch;

#if ENABLE_ION
    struct {
        int	running;
        BpSAP	sap;
        Sdr		sdr;
	ZcoReader	reader;
    } ion;
#endif

    // keep track of messages coming in from the client
    union {
	BDM_C2S_Message_t             *C2S_message;
	BDM_Sync_Message_t            *sync_message;
    } message;
    unsigned char buffer[(10 * 1024)];  // FIXME: use cal
    int index;
} client_t;

#ifdef ENABLE_ION
extern client_t dtn_thread_data;
extern char * dtn_endpoint_id;
#endif

int client_connecting_handler(GIOChannel *ch, GIOCondition condition, gpointer listening_fd_as_pointer);
int client_readable_handler(GIOChannel *unused, GIOCondition unused2, client_t *client);
void handle_client_message(client_t *client, BDM_C2S_Message_t *message);
void disconnect_client(client_t *client);

int sync_receive_connecting_handler(GIOChannel *ch, GIOCondition condition, gpointer listening_fd_as_pointer);
int sync_receive_readable_handler(GIOChannel *unused, GIOCondition unused2, client_t *client);
void handle_sync_datapoints_message(client_t *client, BDM_Sync_Datapoints_Message_t *message);
void handle_sync_metadata_message(client_t *client, BDM_Sync_Metadata_Message_t *message);



int make_listening_socket(int port);
void keepalive(int socket, int idle, int count, int interval);
void make_shutdowns_clean(int withThreads);


// BDM sync'ing
gpointer sync_thread(gpointer config);
gpointer dtn_receive_thread(gpointer config);

extern GSList * sync_config_list;

sync_sender_config_t * read_config_file(const char * fname);

#endif //  BIONET_DATA_MANAGER_H

// Emacs cruft
// Local Variables:
// mode: C
// c-file-style: "Stroustrup"
// End:
