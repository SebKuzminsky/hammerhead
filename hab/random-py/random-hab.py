#!/usr/bin/python

import sys
import optparse
import logging
import time
import random

parser = optparse.OptionParser()
parser.add_option("-i", "--id", dest="hab_id", default="python",
                  help="Use ID as the HAB-ID (defaults to hostname if omitted)",
                  metavar="ID")
parser.add_option("-n", "--min-nodes", dest="min_nodes", default=5,
                  help="If there are fewer than N nodes, make up some more.",
                  metavar="N")
parser.add_option("-x", "--max-delay", dest="max_delay",
                  help="After taking each action (adding or removing a Node, or updating a Resource), the random-hab sleeps up to this long (seconds)",
                  default=1,
                  metavar="X")

(options, args) = parser.parse_args()

logger = logging.getLogger("Bionet Random HAB")
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
ch.setFormatter(formatter)
logger.addHandler(ch)

from hab import *
import add_node
import destroy_node
import update_node

#connect to bionet
hab = bionet_hab_new("RANDOM-py", options.hab_id)
hab_register_callback_set_resource(cb_set_resource);
bionet_fd = hab_connect(hab)
if (0 > bionet_fd):
    logger.warning("problem connection to Bionet, exiting\n")
    exit(1)

#make nodes
while(1):
    while(bionet_hab_get_num_nodes(hab) < options.min_nodes):
        add_node.Add(hab)

    while(bionet_hab_get_num_nodes(hab) > (2 * options.min_nodes)):
        destroy_node.Destroy(hab)

    rnd = random.randint(0,100)

    if (rnd < 10):
        destroy_node.Destroy(hab)
    elif (rnd < 20):
        add_node.Add(hab)
    else:
        update_node.Update(hab)
        
    time.sleep(options.max_delay)
    