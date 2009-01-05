
//
// Copyright (C) 2008, Regents of the University of Colorado.
//

#include <stdlib.h>
#include <errno.h>

#include "bionet-util-2.1.h"
#include "internal.h"


/* function declarations */
static bionet_value_t * bionet_value_new_internal(bionet_resource_t *resource,
						  const void *content,
						  bionet_resource_data_type_t datatype);


/* function implementations */
bionet_value_t *bionet_value_new_binary(bionet_resource_t *resource,
					int content)
{
    return bionet_value_new_internal(resource, 
				     &content, 
				     BIONET_RESOURCE_DATA_TYPE_BINARY);
} /* bionet_value_new_binary() */


bionet_value_t *bionet_value_new_uint8(bionet_resource_t *resource,
				       uint8_t content)
{
    return bionet_value_new_internal(resource, 
				     &content, 
				     BIONET_RESOURCE_DATA_TYPE_UINT8);
} /* bionet_value_new_uint8() */


bionet_value_t *bionet_value_new_int8(bionet_resource_t *resource,
				      int8_t content)
{
    return bionet_value_new_internal(resource, 
				     &content, 
				     BIONET_RESOURCE_DATA_TYPE_INT8);
} /* bionet_value_new_int8() */


bionet_value_t *bionet_value_new_uint16(bionet_resource_t *resource,
					uint16_t content)
{
    return bionet_value_new_internal(resource, 
				     &content, 
				     BIONET_RESOURCE_DATA_TYPE_UINT16);
} /* bionet_value_new_uint16() */


bionet_value_t *bionet_value_new_int16(bionet_resource_t *resource,
				       int16_t content)
{
    return bionet_value_new_internal(resource, 
				     &content, 
				     BIONET_RESOURCE_DATA_TYPE_INT16);
} /* bionet_value_new_int16() */


bionet_value_t *bionet_value_new_uint32(bionet_resource_t *resource,
					uint32_t content)
{
    return bionet_value_new_internal(resource, 
				     &content, 
				     BIONET_RESOURCE_DATA_TYPE_UINT32);
} /* bionet_value_new_uint32() */


bionet_value_t *bionet_value_new_int32(bionet_resource_t *resource,
				       int32_t content)
{
    return bionet_value_new_internal(resource, 
				     &content, 
				     BIONET_RESOURCE_DATA_TYPE_INT32);
} /* bionet_value_new_int32() */


bionet_value_t *bionet_value_new_float(bionet_resource_t *resource,
				       float content)
{
    return bionet_value_new_internal(resource, 
				     &content, 
				     BIONET_RESOURCE_DATA_TYPE_FLOAT);
} /* bionet_value_new_float() */


bionet_value_t *bionet_value_new_double(bionet_resource_t *resource,
				        double content)
{
    return bionet_value_new_internal(resource, 
				     &content, 
				     BIONET_RESOURCE_DATA_TYPE_DOUBLE);
} /* bionet_value_new_double() */


bionet_value_t *bionet_value_new_str(bionet_resource_t *resource,
				     const char * content)
{
    return bionet_value_new_internal(resource, 
				     content, 
				     BIONET_RESOURCE_DATA_TYPE_STRING);
} /* bionet_value_new_float() */


static bionet_value_t * bionet_value_new_internal(bionet_resource_t *resource,
						  const void *content,
						  bionet_resource_data_type_t datatype)
{
    bionet_value_t * value;

    /* sanity */
    if (NULL == resource)
    {
	g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_WARNING, 
	      "bionet_value_new_*(): NULL resource passed in");
	errno = EINVAL;
	return NULL;
    }

    if (NULL == content)
    {
	g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_WARNING, 
	      "bionet_value_new_*(): NULL content passed in");
	errno = EINVAL;
	return NULL;
    }

    if ((BIONET_RESOURCE_DATA_TYPE_MIN > datatype) 
	|| (BIONET_RESOURCE_DATA_TYPE_MAX < datatype))
    {
	g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_WARNING, 
	      "bionet_value_new_*(): Invalid datatype passed in");
	errno = EINVAL;
	return NULL;
    }

    if (resource->data_type != datatype)
    {
	g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_WARNING, 
	      "bionet_value_set_*(): resource/datatype mismatch");
	errno = EINVAL;
	return NULL;
    }

    /* allocation */
    value = calloc(1, sizeof(bionet_value_t));
    if (NULL == value)
    {
	g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_ERROR, 
	      "bionet_value_new_binary(): out of memory!");
        return NULL;
    }

    /* set resource */
    value->resource = resource;
    
    /* set content */
    switch (datatype)
    {
    case BIONET_RESOURCE_DATA_TYPE_BINARY:
    {
	int * pContent = (int *)content;
	value->content.binary_v = *pContent;
	break;
    }
    case BIONET_RESOURCE_DATA_TYPE_UINT8:
    {
	uint8_t * pContent = (uint8_t *)content;
	value->content.uint8_v = *pContent;
	break;
    }
    case BIONET_RESOURCE_DATA_TYPE_INT8:
    {
	int8_t * pContent = (int8_t *)content;
	value->content.int8_v = *pContent;
	break;
    }
    case BIONET_RESOURCE_DATA_TYPE_UINT16:
    {
	uint16_t * pContent = (uint16_t *)content;
	value->content.uint16_v = *pContent;
	break;
    }
    case BIONET_RESOURCE_DATA_TYPE_INT16:
    {
	int16_t * pContent = (int16_t *)content;
	value->content.int16_v = *pContent;
	break;
    }
    case BIONET_RESOURCE_DATA_TYPE_UINT32:
    {
	uint32_t * pContent = (uint32_t *)content;
	value->content.uint32_v = *pContent;
	break;
    }
    case BIONET_RESOURCE_DATA_TYPE_INT32:
    {
	int32_t * pContent = (int32_t *)content;
	value->content.int32_v = *pContent;
	break;
    }
    case BIONET_RESOURCE_DATA_TYPE_FLOAT:
    {
	float * pContent = (float *)content;
	value->content.float_v = *pContent;
	break;
    }
    case BIONET_RESOURCE_DATA_TYPE_DOUBLE:
    {
	double * pContent = (double *)content;
	value->content.double_v = *pContent;
	break;
    }
    case BIONET_RESOURCE_DATA_TYPE_STRING:
    {
	value->content.string_v = (char *)content;
	break;	
    }
    default:
	g_log(BIONET_LOG_DOMAIN, G_LOG_LEVEL_WARNING, 
	      "bionet_value_new_*(): Invalid datatype");
	errno = EINVAL;
	free(value);
	return NULL;
    }

    return value;
} /* bionet_value_new_internal() */


// Emacs cruft
// Local Variables:
// mode: C
// c-file-style: "Stroustrup"
// End: