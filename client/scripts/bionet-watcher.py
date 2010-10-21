#!/usr/bin/python
#
# Copyright (c) 2008-2010, Regents of the University of Colorado.
# This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
# NNC07CB47C.
#


import sys
import optparse
import logging
from select import select
import time

# parse options 
parser = optparse.OptionParser()
parser.add_option("-d", "--hab", "--habs", dest="hab_name",
                  help="Subscribe to a HAB list.", 
                  metavar="HAB-Type.HAB-ID")
parser.add_option("-n", "--node", "--nodes", dest="node_name",
                  help="Subscribe to a Node list.", 
                  metavar="HAB-Type.HAB-ID.Node-ID")
parser.add_option("-r", "--resource", "--resources", dest="resource_name",
                  help="Subscribe to a Resource list.", 
                  metavar="HAB-Type.HAB-ID.Node-ID:Resource-ID")
parser.add_option("-s", "--security-dir", dest="security_dir",
                  help="Directory containing security certificates.",
                  metavar="dir", default=None)
parser.add_option("-e", "--require-security", dest="require_security",
                  help="Require secured connections.",
                  action="store_true", default=False)
parser.add_option("--bdm-only", dest="bdm_only",
                  help="Subscribe to BDMs only.",
                  action="store_true", default=False)
parser.add_option("-T", "--datapoint-start", dest="datapoint_start",
                  help="Datapoint Start Time for BDM subscriptions", 
                  metavar="YYYY-MM-DD HH:MM:SS")
parser.add_option("-t", "--datapoint-end", dest="datapoint_end",
                  help="Datapoint End Time for BDM subscriptions", 
                  metavar="YYYY-MM-DD HH:MM:SS")

(options, args) = parser.parse_args()

logger = logging.getLogger("Bionet Watcher")
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
ch.setFormatter(formatter)
logger.addHandler(ch)

# since bionet wants times in UTC...
def gmt_convert(loctime):
    if (time.daylight):
        return loctime - time.altzone
    else:
        return loctime - time.timezone


from bionet import *
from bdm_client import *

if (options.datapoint_start != None):
    datapoint_start = timeval()
    datapoint_start.tv_sec = gmt_convert(int(time.mktime(time.strptime(options.datapoint_start, 
                                                             "%Y-%m-%d %H:%M:%S"))))
    datapoint_start.tv_usec = 0
else:
    datapoint_start = None

if (options.datapoint_end != None):
    datapoint_end = timeval()
    datapoint_end.tv_sec = gmt_convert(int(time.mktime(time.strptime(options.datapoint_end, 
                                                             "%Y-%m-%d %H:%M:%S"))))
    datapoint_end.tv_usec = 0
else:
    datapoint_end = None

if (options.bdm_only) and (datapoint_start == None) and (datapoint_end == None):
    logger.error("A datapoint start and/or end time must be specified when in BDM-Only mode")
    exit(1)

# start the meat and potatoes

#bionet callbacks
def cb_lost_hab(hab):
    print("lost hab: " + hab.name())


def cb_new_hab(hab):
    print("new hab: " + hab.name())


def cb_new_node(node):
    hab = node.hab()
	
    print("new node: " + node.name())
	
    if (node.numResources()):
        print("    Resources:")
	    
        for i in range(node.numResources()):
            resource = node.resource(i)
            datapoint = resource.datapoint(0)
		
            if (datapoint == None):
                print("        " + resource.datatypeToString() + " " + resource.flavorToString() + " " + resource.id() + ": (no known value)")
            
            else:
                value = datapoint.value()
                #%s %s %s = %s @ %s 
                print("        " + resource.datatypeToString() + " " + resource.flavorToString() + " " + resource.id() + " = " + str(value) + " @ " + datapoint.timestampToString())

# TODO: streams are not yet implemented in OO bindings
#    if (bionet_node_get_num_streams(node)):
#        print("    Streams:")
#    
#        for i in range(bionet_node_get_num_streams(node)):
#            stream = bionet_node_get_stream_by_index(node, i)
#            print("        " + bionet_stream_get_id(stream) + " " + bionet_stream_get_type(stream) + " " + bionet_stream_direction_to_string(bionet_stream_get_direction(stream)))


def cb_lost_node(node):
    print("lost node: " + node.name())


def cb_datapoint(datapoint):
    value = datapoint.value()
    resource = datapoint.resource()
    node = resource.node()
    hab = node.hab()
    
    #"%s.%s.%s:%s = %s %s %s @ %s"    
    print(resource.name() + " = " + resource.datatypeToString() + " " + resource.flavorToString() + " " + str(value) + " @ " + datapoint.timestampToString())


#bdm callbacks
def cb_bdm_lost_bdm(bdm):
    print("BDM lost bdm: " + bdm.id())


def cb_bdm_new_bdm(bdm):
    print("BDM new bdm: " + bdm.id())


def cb_bdm_lost_hab(hab):
    print("BDM lost hab: " + hab.name())


def cb_bdm_new_hab(hab):
    print("BDM new hab: " + hab.name())


def cb_bdm_new_node(node):
    hab = node.hab()
	
    print("BDM new node: " + node.name())
	
    if (node.numResources()):
        print("    BDM Resources:")
	    
        for i in range(node.numResources()):
            resource = node.resource(i)
            datapoint = resource.datapoint(0)
		
            if (datapoint == None):
                print("        BDM " + resource.datatypeToString() + " " + resource.flavorToString() + " " + resource.id() + ": (no known value)")
            
            else:
                value = datapoint.value()
                #%s %s %s = %s @ %s 
                print("        BDM " + resource.datatypeToString() + " " + resource.flavorToString() + " " + resource.id() + " = " + str(value) + " @ " + datapoint.timestampToString())

# TODO: streams are not yet implemented in OO bindings
#    if (bionet_node_get_num_streams(node)):
#        print("    BDM Streams:")
#    
#        for i in range(bionet_node_get_num_streams(node)):
#            stream = bionet_node_get_stream_by_index(node, i)
#            print("        BDM " + bionet_stream_get_id(stream) + " " + bionet_stream_get_type(stream) + " " + bionet_stream_direction_to_string(bionet_stream_get_direction(stream)))


def cb_bdm_lost_node(node):
    print("BDM lost node: " + node.name())


def cb_bdm_datapoint(datapoint):
    value = datapoint.value()
    resource = datapoint.resource()
    node = resource.node()
    hab = node.hab()
    
    #"BDM %s.%s.%s:%s = %s %s %s @ %s"    
    print("BDM " + resource.name() + " = " + resource.datatypeToString() + " " + resource.flavorToString() + " " + str(value) + " @ " + datapoint.timestampToString())

    
if (options.security_dir != None):
    bionet_init_security(options.security_dir, options.require_security)

bn = None
bdm_sub = None

if (options.bdm_only == False):
    bn = Bionet()
    if (None == bn):
        logger.error("error connecting to Bionet")
        exit(1)
    else:
        logger.info("connected to Bionet")

if (datapoint_start != None) or (datapoint_end != None):
    bdm_sub = BdmSubscriber() 
    if (None == bdm_sub):
        logger.error("Failed to connect to Bionet Data Manager network.")
        exit(1)
    else:
        logger.info("connected to Bionet Data Manager network")


if (None == bn) and (None == bdm_sub):
    logger.error("Failed to connect to both Bionet and Bionet Data Manager.")
    sys.exit(1)

# register Bionet callbacks
if (None != bn):
    bn.newHabCallback = cb_new_hab;
    bn.lostHabCallback = cb_lost_hab
    bn.newNodeCallback = cb_new_node
    bn.lostNodeCallback = cb_lost_node
    bn.datapointCallback = cb_datapoint

    bionet_subscribed_to_something = 0;

    if (options.hab_name):
        bn.subscribe(options.hab_name)
        bionet_subscribed_to_something = 1
    if (options.node_name):
        bn.subscribe(options.node_name)
        bionet_subscribed_to_something = 1
    if (options.resource_name):
        bn.subscribe(options.resource_name)
        bionet_subscribed_to_something = 1

    if (0 == bionet_subscribed_to_something):
        bn.subscribe("*.*")
        bn.subscribe("*.*.*")
        bn.subscribe("*.*.*:*")


# register BDM callbacks
if (None != bdm_sub):
    bdm_sub.newBdmCallback = cb_bdm_new_bdm
    bdm_sub.lostBdmCallback = cb_bdm_lost_bdm
    bdm_sub.newHabCallback = cb_bdm_new_hab
    bdm_sub.lostHabCallback = cb_bdm_lost_hab
    bdm_sub.newNodeCallback = cb_bdm_new_node
    bdm_sub.lostNodeCallback = cb_bdm_lost_node
    bdm_sub.datapointCallback = cb_bdm_datapoint

    bdm_subscribed_to_something = 0;

    if (options.hab_name):
        bdm_sub.subscribe(options.hab_name)
        bdm_subscribed_to_something = 1
    if (options.node_name):
        bdm_sub.subscribe(options.node_name)
        bdm_subscribed_to_something = 1
    if (options.resource_name):
        bdm_sub.subscribe(options.resource_name, datapoint_start, datapoint_end)
        bdm_subscribed_to_something = 1

    if (0 == bdm_subscribed_to_something):
        bdm_sub.subscribeToHab("*.*")
        bdm_sub.subscribeToNode("*.*.*")
        bdm_sub.subscribeToDatapoints("*.*.*:*", datapoint_start, datapoint_end)

fd_list = []
if (None != bn):
    fd_list.append(bn.fd)
if (None != bdm_sub):
    fd_list.append(bdm_sub.fd)

while(1):
     (rr, wr, er) = select(fd_list, [], [])
     for fd in rr:
         if (None != bn and fd == bn.fd):
             #logger.info("Reading from Bionet...");
             bn.read()
         if (None != bdm_sub and fd == bdm_sub.fd):
             #logger.info("Reading from BDM...");
             bdm_sub.read()
