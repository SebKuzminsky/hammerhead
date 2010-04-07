#!/usr/bin/python
#
# Copyright (c) 2008-2010, Regents of the University of Colorado.
# This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
# NNC07CB47C.
#


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
parser.add_option("-t", "--test", dest="test", default=None,
                  help="Output all data to a file formatted for testing against BDM.",
                  metavar="FILE")
parser.add_option("-l", "--loop", dest="loops", default=0,
                  help="Number of times to do node updates before quitting.")
parser.add_option("-s", "--security-dir", dest="security_dir", default=None,
                  help="Directory containing security certificates.")

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


def pycb_set_resource(resource, value):
    print "callback: should set " + bionet_resource_get_local_name(resource) + " to " + bionet_value_to_str(value)


#initialize bionet security
if (options.security_dir != None):
    if (hab_init_security(options.security_dir, 1)):
        logger.warning("Failed to initialize security.")


#connect to bionet
hab = bionet_hab_new("RANDOM", options.hab_id)
pyhab_register_callback_set_resource(pycb_set_resource);
bionet_fd = hab_connect(hab)
if (0 > bionet_fd):
    logger.warning("problem connection to Bionet, exiting\n")
    exit(1)

loops = 0

#test mode. open the output file and sleep to let subscribers catch up
if (options.test):
    f = open(options.test, "w")
    time.sleep(10)
else:
    f = None;


hab_read()

#make nodes
while (int(options.loops) == 0) or (loops < int(options.loops)):
    while(bionet_hab_get_num_nodes(hab) < options.min_nodes):
        add_node.Add(hab, f)

    while(bionet_hab_get_num_nodes(hab) > (2 * options.min_nodes)):
        destroy_node.Destroy(hab, f)

    rnd = random.randint(0,100)

    if (rnd < 10):
        destroy_node.Destroy(hab, f)
    elif (rnd < 20):
        add_node.Add(hab, f)
    else:
        update_node.Update(hab, f)

    hab_read()
    loops = loops + 1
    time.sleep(options.max_delay)

