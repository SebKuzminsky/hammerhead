
// Copyright (c) 2008-2009, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#ifndef BIONET_UTIL_H
#define BIONET_UTIL_H

/**
 * @file bionet-util.h
 * Helper functions for dealing with Bionet datatypes
 */

#include <stdint.h>
#include <time.h>

#include <sys/time.h>

#include <glib.h>

typedef struct bionet_bdm_opaque_t       bionet_bdm_t;
typedef struct bionet_hab_opaque_t       bionet_hab_t;
typedef struct bionet_node_opaque_t      bionet_node_t;
typedef struct bionet_stream_opaque_t    bionet_stream_t;
typedef struct bionet_resource_opaque_t  bionet_resource_t;
typedef struct bionet_datapoint_opaque_t bionet_datapoint_t;
typedef struct bionet_value_opaque_t     bionet_value_t;

#include "bionet-bdm.h"
#include "bionet-hab.h"
#include "bionet-node.h"
#include "bionet-stream.h"
#include "bionet-resource.h"
#include "bionet-datapoint.h"
#include "bionet-value.h"
#include "bionet-version.h"

/**
 * The bionet code uses glib's logging facility to log internal messages,
 * and this is the domain.
 */
#define  BIONET_LOG_DOMAIN  "bionet"


/**
 * name components are no more than this long (including the terminating NULL)
 */
#define BIONET_NAME_COMPONENT_MAX_LEN (100)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the latest system error
 *
 * OS convergence layer for error reporting
 *
 * @return OS-specific error number
 */
int bionet_get_network_error(void);


/**
 * @brief Get the latest system error as a string
 *
 * OS convergence layer for error reporting
 *
 * @return OS-specific error string
 */
const char *bionet_get_network_error_string(void);


// 
// misc helpers
//

/**
 * @brief Ensure the name component is a valid name or a wildcard
 *
 * @param[in] str
 *
 * @retval 0 Invalid
 * @retval 1 Valid
 */
int bionet_is_valid_name_component_or_wildcard(const char *str);


/**
 * @brief Ensure the name component is a valid name
 *
 * @param[in] str
 *
 * @retval 0 Invalid
 * @retval 1 Valid
 *
 * @note Wildcards are not allow in this function. Use 
 * bionet_is_valid_name_component_or_wildcard()
 */
int bionet_is_valid_name_component(const char *str);


/**
 * @brief Check if a name component matches a specified pattern
 * 
 * @param[in] name_component Name component to check
 * @param[in] pattern Pattern to check against
 *
 * @retval 0 Does not match
 * @retval 1 Match
 *
 * @note pattern may be the wildcard "*", which matches all name components
 */
int bionet_name_component_matches(const char *name_component, const char *pattern);


/**
 * @brief Check if a resource name matches a specified pattern
 * 
 * @param[in] resource_name Resource name to check
 * @param[in] pattern Pattern to check against
 *
 * @retval 0 Does not match
 * @retval 1 Match
 *
 * @note pattern may be the wildcard "*", which matches all name components
 */
int bionet_resource_name_matches(const char *resource_name, const char *pattern);


//
// puts the glib log messages where you want them
//

typedef struct {
    enum {
	BIONET_LOG_TO_STDOUT = 0, /**< log msgs to STDOUT */
        BIONET_LOG_TO_SYSLOG = 1  /**< log msgs to SYSLOG */
    } destination;

    // messages with log_level *below* log_limit are logged, all others are dropped
    // FIXME: really we want a default log_limit and then optional per-domain limits
    GLogLevelFlags log_limit;
} bionet_log_context_t;


/**
 * @brief Glib log handler for Bionet
 *
 * @param[in] log_domain Name of log domain
 * @param[in] log_level Level of this message
 * @param[in] message Message to log
 * @param[in] log_context Other logging context or NULL
 */
void bionet_glib_log_handler(
    const gchar *log_domain,
    GLogLevelFlags log_level,
    const gchar *message,
    gpointer log_context
);




/**
 * @brief Take a resource name string and split the name up into its components
 *
 * Resource name string shall be in the format 
 *    <HAB-type>.<HAB-ID>.<Node-ID>:<Resource-ID>
 * 
 * The passed-in char-pointer-pointers will be assigned the addresses of
 * statically allocated strings internal to the library.  The caller must
 * not free or assign to these strings.  The values will change on the next
 * call to this function.
 *
 * If the caller passes in NULL for any of the "out" arguments, that part
 * of the name component will be skipped.
 *
 * @param[in] resource_name Resource name to split
 * @param[out] hab_type HAB-type from resource name
 * @param[out] hab_id HAB-ID from resource name
 * @param[out] node_id Node-ID from resource name
 * @param[out] resource_id Resource-ID from resource name
 *
 * @retval 0 Success
 * @retval -1 Failure
 */
int bionet_split_resource_name(
    const char *resource_name,
    char **hab_type,
    char **hab_id,
    char **node_id,
    char **resource_id
);

/**
 * @brief Take a resource name string and split the name up into its components
 *
 * Resource name string shall be in the format 
 *    \<HAB-type\>.\<HAB-ID\>.\<Node-ID\>:\<Resource-ID\>[?\<Topic Query params\>]
 * 
 * If the caller passes in NULL for any of the "out" arguments, that part
 * of the name component will be skipped.
 *
 * @param[in] resource_name Resource name to split
 * @param[out] hab_type HAB-type from resource name
 * @param[out] hab_id HAB-ID from resource name
 * @param[out] node_id Node-ID from resource name
 * @param[out] resource_id Resource-ID from resource name
 *
 * @retval N The offest of the Topic Query Parameters, or 0 if none
 * @retval -1 Failure
 */
int bionet_split_resource_name_r(
    const char *resource_name,
    char hab_type[BIONET_NAME_COMPONENT_MAX_LEN],
    char hab_id[BIONET_NAME_COMPONENT_MAX_LEN],
    char node_id[BIONET_NAME_COMPONENT_MAX_LEN],
    char resource_id[BIONET_NAME_COMPONENT_MAX_LEN]);


/**
 * @brief Take a hab name string and split the name up into its components
 *
 * Hab name string shall be in the format:
 *    [<BDM-ID>/]<HAB-type>.<HAB-ID>
 * 
 * If the caller passes in NULL for any of the "out" arguments, that part
 * of the name component will be skipped.
 *
 * If BDM-ID/ is not present it defaults to *
 *
 * @param[in] topic Hab name to split
 * @param[out] peer_id Peer BDM-ID from resource name
 * @param[out] bdm_id Recording BDM-ID from resource name
 * @param[out] hab_type HAB-type from resource name
 * @param[out] hab_id HAB-ID from resource name
 *
 * @retval N The offest of the Topic Query Parameters, or 0 if none
 * @retval -1 Failure
 */
int bdm_split_hab_name_r(
        const char * topic,
        char peer_id[BIONET_NAME_COMPONENT_MAX_LEN],
        char bdm_id[BIONET_NAME_COMPONENT_MAX_LEN],
        char hab_type[BIONET_NAME_COMPONENT_MAX_LEN],
        char hab_id[BIONET_NAME_COMPONENT_MAX_LEN]);


/**
 * @brief Take a node name string and split the name up into its components
 *
 * Node name string shall be in the format:
 *    [<BDM-ID>/]<HAB-type>.<HAB-ID>.<Node-ID>
 * 
 * If the caller passes in NULL for any of the "out" arguments, that part
 * of the name component will be skipped.

 * If BDM-ID/ is not present it defaults to *
 *
 * @param[in] topic Node name to split
 * @param[out] peer_id Peer BDM-ID from resource name
 * @param[out] bdm_id Recording BDM-ID from resource name
 * @param[out] hab_type HAB-type from resource name
 * @param[out] hab_id HAB-ID from resource name
 * @param[out] node_id Node-ID from resource name
 *
 * @retval N The offest of the Topic Query Parameters, or 0 if none
 * @retval -1 Failure
 */
int bdm_split_node_name_r(
        const char * topic,
        char peer_id[BIONET_NAME_COMPONENT_MAX_LEN],
        char bdm_id[BIONET_NAME_COMPONENT_MAX_LEN],
        char hab_type[BIONET_NAME_COMPONENT_MAX_LEN],
        char hab_id[BIONET_NAME_COMPONENT_MAX_LEN],
        char node_id[BIONET_NAME_COMPONENT_MAX_LEN]);

/**
 * @brief Take a resource name string and split the name up into its components
 *
 * Resource name string shall be in the format:
 *    [\<BDM-ID\>/]\<HAB-type\>.\<HAB-ID\>.\<Node-ID\>:\<Resource-ID\>[?\<Topic Query params\>]
 * 
 * If the caller passes in NULL for any of the "out" arguments, that part
 * of the name component will be skipped.

 * If BDM-ID/ is not present it defaults to *
 *
 * @param[in] topic Resource name to split
 * @param[out] peer_id Peer BDM-ID from resource name
 * @param[out] bdm_id Recording BDM-ID from resource name
 * @param[out] hab_type HAB-type from resource name
 * @param[out] hab_id HAB-ID from resource name
 * @param[out] node_id Node-ID from resource name
 * @param[out] resource_id Resource-ID from resource name
 *
 * @retval N The offest of the Topic Query Parameters, or 0 if none
 * @retval -1 Failure
 */
int bdm_split_resource_name_r(
        const char * topic,
        char peer_id[BIONET_NAME_COMPONENT_MAX_LEN],
        char bdm_id[BIONET_NAME_COMPONENT_MAX_LEN],
        char hab_type[BIONET_NAME_COMPONENT_MAX_LEN],
        char hab_id[BIONET_NAME_COMPONENT_MAX_LEN],
        char node_id[BIONET_NAME_COMPONENT_MAX_LEN],
        char resource_id[BIONET_NAME_COMPONENT_MAX_LEN]);

/**
 * @brief Take a node name string and split the name up into its components
 *
 * Node name string shall be in the format 
 *    <HAB-type>.<HAB-ID>.<Node-ID>
 *
 * The passed-in char-pointer-pointers will be assigned the addresses of
 * statically allocated strings internal to the library.  The caller must
 * not free or assign to these strings.  The values will change on the next
 * call to this function.
 *
 * If the caller passes in NULL for any of the "out" arguments, that part
 * of the name component will be skipped.
 *
 * @param[in] node_name Node name to split
 * @param[out] hab_type HAB-type
 * @param[out] hab_id HAB-ID 
 * @param[out] node_id Node-ID
 *
 * @retval 0 Success
 * @retval -1 Failure
 */
int bionet_split_node_name(
    const char *node_name,
    char **hab_type,
    char **hab_id,
    char **node_id
);

/**
 * @brief Take a node name string and split the name up into its components
 *
 * Node name string shall be in the format 
 *    <HAB-type>.<HAB-ID>.<Node-ID>
 *
 * If the caller passes in NULL for any of the "out" arguments, that part
 * of the name component will be skipped.
 *
 * @param[in] node_name Node name to split
 * @param[out] hab_type HAB-type
 * @param[out] hab_id HAB-ID 
 * @param[out] node_id Node-ID
 *
 * @retval 0 Success
 * @retval -1 Failure
 */
int bionet_split_node_name_r(
    const char *node_name,
    char hab_type[BIONET_NAME_COMPONENT_MAX_LEN],
    char hab_id[BIONET_NAME_COMPONENT_MAX_LEN],
    char node_id[BIONET_NAME_COMPONENT_MAX_LEN]
);

/**
 * @brief Take a HAB name string and split the name up into its components
 *
 * HAB name string shall be in the format 
 *    <HAB-type>.<HAB-ID>
 *
 * The passed-in char-pointer-pointers will be assigned the addresses of
 * statically allocated strings internal to the library.  The caller must
 * not free or assign to these strings.  The values will change on the next
 * call to this function.
 *
 * If the caller passes in NULL for any of the "out" arguments, that part
 * of the name component will be skipped.
 *
 * @param[in] hab_name HAB name to split
 * @param[out] hab_type HAB-type
 * @param[out] hab_id HAB-ID
 *
 * @retval 0 Success
 * @retval -1 Failure
 */
int bionet_split_hab_name(
    const char *hab_name,
    char **hab_type,
    char **hab_id
);


/**
 * @brief Take a HAB name string and split the name up into its components
 *
 * Resource name string shall be in the format 
 *    <HAB-type>.<HAB-ID>
 *
 * @param[in] hab_name HAB name to split
 * @param[out] hab_type HAB-type 
 * @param[out] hab_id HAB-ID 
 *
 * @retval 0 Success
 * @retval -1 Failure
 *
 * @note This function is re-entrant.
 */
int bionet_split_hab_name_r(
    const char *hab_name,
    char hab_type[BIONET_NAME_COMPONENT_MAX_LEN],
    char hab_id[BIONET_NAME_COMPONENT_MAX_LEN]
);


/**
 * @brief Take a Node-ID:Resource-ID string and split the name up into its 
 *        components
 *
 * Resource name string shall be in the format 
 *     <Node-ID>:<Resource-ID>
 *
 * @param[in] node_and_resource Node and Resource name to split
 * @param[out] node_id Node-ID
 * @param[out] resource_id Resource-ID
 *
 * @retval 0 Success
 * @retval -1 Failure
 *
 * @note This function is re-entrant.
 */
int bionet_split_nodeid_resourceid_r(
    const char *node_and_resource,
    char node_id[BIONET_NAME_COMPONENT_MAX_LEN],
    char resource_id[BIONET_NAME_COMPONENT_MAX_LEN]
); 

#ifdef __cplusplus
}
#endif

#endif //  BIONET_UTIL_H

// Emacs cruft
// Local Variables:
// mode: C
// c-file-style: "Stroustrup"
// End:
