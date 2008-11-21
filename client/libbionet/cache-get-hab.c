//
// Copyright (C) 2008, Regents of the University of Colorado.
//


#include <string.h>
#include <glib.h>
#include "bionet.h"

extern GSList *bionet_habs;


unsigned int bionet_cache_get_num_habs(void)
{
    if (NULL == bionet_habs)
    {
	return 0;
    }

    return g_slist_length(bionet_habs);
} /* bionet_cache_get_num_habs() */


bionet_node_t *bionet_cache_get_hab_by_index(unsigned int index)
{
    if ((NULL == bionet_habs) || (index >= g_slist_length(bionet_habs)))
    {
	/* either there are no nodes or this is past the bounds of the list */
	return NULL;
    }
	
    return g_slist_nth_data(bionet_habs, index);
} /* bionet_cache_get_hab_by_index() */


// Emacs cruft
// Local Variables:
// mode: C
// c-file-style: "Stroustrup"
// End: