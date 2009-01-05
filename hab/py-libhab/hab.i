%module hab
%{
#include "hardware-abstractor-2.1.h"
#include "bionet-util-2.1.h"
#include "bionet-hab-2.1.h"
#include "bionet-node-2.1.h"
#include "bionet-resource-2.1.h"
%}

%include "hardware-abstractor.h"
%include "bionet-util-2.1.h"
%include "bionet-hab-2.1.h"
%include "bionet-node-2.1.h"
%include "bionet-resource-2.1.h"

%constant void cb_set_resource(bionet_resource_t *resource, const bionet_value_t *value);


%inline %{
void cb_set_resource(bionet_resource_t *resource, 
     const bionet_value_t *value) {
    printf(
        "callback: should set %s:%s to '%s'\n",
        bionet_node_get_id(bionet_resource_get_node(resource)),
        bionet_resource_get_id(resource),
        bionet_value_to_str(value)
    );
}
%}
