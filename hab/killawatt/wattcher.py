#!/usr/bin/env python
import serial, time, datetime, sys
from xbee import xbee
import sensorhistory
import logging
import optparse

parser = optparse.OptionParser()
parser.add_option("-i", "--interval", dest="interval", default="30",help="Choose the number of seconds desired between data collections (defaults to 30 seconds)", metavar = "INT")

(options, args) = parser.parse_args()

LEVELS = {'debug':logging.DEBUG,
            'info':logging.INFO,
            'warning':logging.WARNING,
            'error':logging.ERROR,
            'critical':logging.CRITICAL}

logger = logging.getLogger("Bionet Kill-A-Watt HAB")
logger.setLevel(logging.WARNING)
ch = logging.StreamHandler()
ch.setLevel(logging.WARNING)
formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
ch.setFormatter(formatter)
logger.addHandler(ch)

# use App Engine? or log file? comment out next line if appengine
LOGFILENAME = "powerdatalog.csv"   # where we will store our flatfile data

SERIALPORT = "/dev/ttyUSB0"    # the com/serial port the XBee is connected to
BAUDRATE = 9600      # the baud rate we talk to the xbee
CURRENTSENSE = 4       # which XBee ADC has current draw data
VOLTSENSE = 0          # which XBee ADC has mains voltage data
MAINSVPP = 170 * 2     # +-170V is what 120Vrms ends up being (= 120*2sqrt(2))
vrefcalibration = [492,  # Calibration for sensor #0
                   487,  # Calibration for sensor #1
                   489,  # Calibration for sensor #2
                   492,  # Calibration for sensor #3
                   501,  # Calibration for sensor #4
                   493]  # etc... approx ((2.4v * (10Ko/14.7Ko)) / 3
CURRENTNORM = 15.5  # conversion to amperes from ADC
NUMWATTDATASAMPLES = 1800 # how many samples to watch in the plot window, 1 hr @ 2s samples


# open up the FTDI serial port to get data transmitted to xbee
ser = serial.Serial(SERIALPORT, BAUDRATE)
ser.open()

from hab import *

#connect to bionet
hab = bionet_hab_new("Kill-A-Watt", None)
bionet_fd = hab_connect(hab)
if (0 > bionet_fd):
    logger.critical("Problem connection to Bionet, exiting\n")
    exit(1)

# open our datalogging file
logfile = None
try:
    logfile = open(LOGFILENAME, 'r+')
except IOError:
    # didn't exist yet
    logfile = open(LOGFILENAME, 'w+')
    logfile.write("#Date, time, sensornum, avgWatts\n");
    logfile.flush()
            
DEBUG = False
if (sys.argv and len(sys.argv) > 1):
    if sys.argv[1] == "-d":
        DEBUG = True
            
sensorhistories = sensorhistory.SensorHistories(logfile)
print sensorhistories

interval = 0
TotalWattHour=0
# the 'main loop' runs once a second or so
def update_graph(idleevent):
    global avgwattdataidx, sensorhistories, DEBUG, interval
    packet = xbee.find_packet(ser)
    if not packet:
        return
    xb = xbee(packet)
    sensorhistory = sensorhistories.find(xb.address_16)
    interval += time.time() - sensorhistory.lasttime
    sensorhistory.lasttime = time.time()
    if (int(options.interval) < interval): 

        hab_read()

        # grab one packet from the xbee, or timeout
        #packet = xbee.find_packet(ser)
        #if not packet:
        #    return        # we timedout
    
        #xb = xbee(packet)             # parse the packet
        #print xb.address_16
        if DEBUG:       # for debugging sometimes we only want one
            print xb.address_16
    
        #check if there is a new node    
        node = bionet_hab_get_node_by_id(hab, str(xb.address_16))
        if (node == None):
            #create and report node
            node = bionet_node_new(hab, str(xb.address_16))
            if (node == None):
                logger.critical("Critical - Error getting new node")
                sys.exit(1)
            if (bionet_hab_add_node(hab, node)):
                logger.critical("Critical - Error adding node to hab")
                sys.exit(1)
            resource = bionet_resource_new(node, BIONET_RESOURCE_DATA_TYPE_FLOAT, BIONET_RESOURCE_FLAVOR_SENSOR, "Amps")
            if (resource == None):
                logger.critical("Critical - Error creating Resource - AMPS")
                sys.exit(1)
            if (bionet_node_add_resource(node, resource)):
                logger.critical("Error adding resource to node")
                sys.exit(1)
            if (hab_report_new_node(node)):
                logger.error("Critical - Error reporting node")
            resource = bionet_resource_new(node, BIONET_RESOURCE_DATA_TYPE_FLOAT, BIONET_RESOURCE_FLAVOR_SENSOR, "Watts")
            if (resource == None):
                logger.critical("Critical - Error creating Resource - WATTS")
                sys.exit(1)
            if (bionet_node_add_resource(node, resource)):
                logger.critical("Critical - Error adding resource - Watts - to node")
                sys.exit(1)
            if (hab_report_new_node(node)):
                logger.error("Critical - Error reporting node")
            resource = bionet_resource_new(node, BIONET_RESOURCE_DATA_TYPE_FLOAT, BIONET_RESOURCE_FLAVOR_SENSOR, "TotalWHour")
            if (resource == None):
                logger.critical("Critical - Error creating Resource - TOTALWHOUR")
                sys.exit(1)
            if (bionet_node_add_resource(node, resource)):
                logger.critical("Critical - Error adding resource - TotalWHour - to node")
                sys.exit(1)
            if (hab_report_new_node(node)):
                logger.error("Critical - Error reporting node")
            resource = bionet_resource_new(node, BIONET_RESOURCE_DATA_TYPE_FLOAT, BIONET_RESOURCE_FLAVOR_SENSOR, "WHour")
            if (resource == None):
                logfile.write("Critical - Error creating Resource - WHOUR")
                sys.exit(1)
            if (bionet_node_add_resource(node, resource)):
                logger.critical("Critical - Error adding resource - WHour - to node")
                sys.exit(1)
            if (hab_report_new_node(node)):
                logger.error("Critical - Error reporting node")
        else:
            None

        # we'll only store n-1 samples since the first one is usually messed up
        voltagedata = [-1] * (len(xb.analog_samples) - 1)
        ampdata = [-1] * (len(xb.analog_samples ) -1)
        # grab 1 thru n of the ADC readings, referencing the ADC constants
        # and store them in nice little arrays
        for i in range(len(voltagedata)):
            voltagedata[i] = xb.analog_samples[i+1][VOLTSENSE]
            ampdata[i] = xb.analog_samples[i+1][CURRENTSENSE]

        if DEBUG:
            print "ampdata: "+str(ampdata)
            logfile.write("Debug - ampdata: "+str(ampdata))
            print "voltdata: "+str(voltagedata)
            logfile.write("Debug - voltdata: "+str(voltagedata))
        logger.debug("Debug - ampdata: "+str(ampdata))
        logger.debug("Debug - voltdata: "+str(voltagedata))

        # get max and min voltage and normalize the curve to '0'
        # to make the graph 'AC coupled' / signed
        min_v = 1024     # XBee ADC is 10 bits, so max value is 1023
        max_v = 0
        for i in range(len(voltagedata)):
            if (min_v > voltagedata[i]):
                min_v = voltagedata[i]
            if (max_v < voltagedata[i]):
                max_v = voltagedata[i]

        # figure out the 'average' of the max and min readings
        avgv = (max_v + min_v) / 2
        # also calculate the peak to peak measurements
        vpp =  max_v-min_v

        for i in range(len(voltagedata)):
            #remove 'dc bias', which we call the average read
            voltagedata[i] -= avgv
            # We know that the mains voltage is 120Vrms = +-170Vpp
            voltagedata[i] = (voltagedata[i] * MAINSVPP) / vpp


        # normalize current readings to amperes
        for i in range(len(ampdata)):
            # VREF is the hardcoded 'DC bias' value, its
            # about 492 but would be nice if we could somehow get this data once in a while maybe using xbeeAPI
            if vrefcalibration[xb.address_16]:
                ampdata[i] -= vrefcalibration[xb.address_16]
            else:
                ampdata[i] -= vrefcalibration[0]
            # the CURRENTNORM is our normalizing constant
            # that converts the ADC reading to Amperes
            ampdata[i] /= CURRENTNORM

        # calculate instant. watts, by multiplying V*I for each sample point
        wattdata = [0] * len(voltagedata)
        for i in range(len(wattdata)):
            wattdata[i] = voltagedata[i] * ampdata[i]

        # sum up the current drawn over one 1/60hz cycle
        avgamp = 0
        # 16.6 samples per second, one cycle = ~17 samples
        # close enough for govt work :(
        for i in range(17):
            avgamp += abs(ampdata[i])
        avgamp /= 17.0

        # sum up power drawn over one 1/60hz cycle
        avgwatt = 0
        # 16.6 samples per second, one cycle = ~17 samples
        for i in range(17):         
            avgwatt += abs(wattdata[i])
        avgwatt /= 17.0


        # Print out our most recent measurements
        logger.info(str(xb.address_16)+"Current draw, in amperes: "+str(avgamp))


        logger.info("Watt draw, in VA: "+str(avgwatt))
        resource = bionet_node_get_resource_by_id(node, "Amps")
        if (resource == None):
            logger.error("Error no such resource - Amps")
        else:
            bionet_resource_set_float(resource, avgamp, None)
    
        resource = bionet_node_get_resource_by_id(node, "Watts")
        if (resource == None):
            logger.error("Error no such resource - Watts")
        else:
            bionet_resource_set_float(resource, avgwatt, None)
 
        if (avgamp > 13):
            return            # hmm, bad data

        # retreive the history for this sensor
        #sensorhistory = sensorhistories.find(xb.address_16)
        #print sensorhistory
    
        # add up the delta-watthr used since last reading
        # Figure out how many watt hours were used since last reading
        elapsedseconds = time.time() - sensorhistory.lasttime
        dwatthr = (avgwatt * elapsedseconds) / (60.0 * 60.0)  # 60 seconds in 60 minutes = 1 hr
        sensorhistory.lasttime = time.time()
        logger.info("Wh used in last "+str(elapsedseconds)+" seconds: "+str(dwatthr))
        sensorhistory.addwatthr(dwatthr)
    
        resource = bionet_node_get_resource_by_id(node, "WHour")
        if (resource == None):
            logger.error("Error no such resource - WHour")
        else:
            bionet_resource_set_float(resource, dwatthr, None)
    
        global TotalWattHour
        TotalWattHour += dwatthr

        resource = bionet_node_get_resource_by_id(node, "TotalWHour")
        if (resource == None):
            logger.error("Error no such resource - TotalWHour")
        else:
            bionet_resource_set_float(resource, TotalWattHour, None)
               
            # Lets log it! Seek to the end of our log file
            if logfile:
                logfile.seek(0, 2) # 2 == SEEK_END. ie, go to the end of the file
                logfile.write(time.strftime("%Y %m %d, %H:%M")+", "+
                              str(sensorhistory.sensornum)+", "+
                              str(sensorhistory.avgwattover5min())+"\n")
                logfile.flush()
             
            # Reset our 5 minute timer
            sensorhistory.reset5mintimer()
        
        hab_report_datapoints (node)
        interval = 0

while True:
    update_graph(None)
	

