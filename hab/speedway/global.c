
#include "ltkc.h"
#include "hardware-abstractor.h"


bionet_hab_t *hab = NULL;
bionet_node_t *reader_node = NULL;

int show_messages = 0;

LLRP_tSConnection *pConn;
LLRP_tSTypeRegistry *pTypeRegistry;

