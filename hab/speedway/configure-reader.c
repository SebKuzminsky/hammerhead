
// Copyright (c) 2008-2010, Gary Grobe and the Regents of the University of
// Colorado.
// This work was supported by NASA contracts NNJ05HE10G, NNC06CB40C, and
// NNC07CB47C.


#include <errno.h>
#include <stdio.h>

#include <glib.h>

#include "ltkc.h"
#include "speedway.h"

extern LLRP_tSGET_READER_CAPABILITIES_RESPONSE *pRspCapabilities;

//
// This function configures the reader for the JSC "Trashcan" and "Portal" demos.
//
// enable GPI1, and enable GPI Event Notification
//

int configure_reader(void) {

    // enable all the GPI pins
    LLRP_tSGPIPortCurrentState gpi_port_state[] = {
        { 
            .hdr.elementHdr.pType = &LLRP_tdGPIPortCurrentState,
            .GPIPortNum = 1,
            .Config = 1  // enabled
        },

        { 
            .hdr.elementHdr.pType = &LLRP_tdGPIPortCurrentState,
            .GPIPortNum = 2,
            .Config = 1  // enabled
        },

        { 
            .hdr.elementHdr.pType = &LLRP_tdGPIPortCurrentState,
            .GPIPortNum = 3,
            .Config = 1  // enabled
        },

        { 
            .hdr.elementHdr.pType = &LLRP_tdGPIPortCurrentState,
            .GPIPortNum = 4,
            .Config = 1  // enabled
        }
    };

    LLRP_tSReceiveSensitivityTableEntry * cur_table_entry =
	pRspCapabilities->pGeneralDeviceCapabilities->listReceiveSensitivityTableEntry;
    llrp_u16_t rf_sensitivity_index = 0;
    while (cur_table_entry) {
	if (cur_table_entry->ReceiveSensitivityValue == rf_sensitivity) {
	    rf_sensitivity_index = cur_table_entry->Index;
	    break;
	}
	cur_table_entry = LLRP_GeneralDeviceCapabilities_nextReceiveSensitivityTableEntry(cur_table_entry); 
    }

    LLRP_tSRFReceiver rfreceiver_sensitivity;
    rfreceiver_sensitivity.hdr.elementHdr.pType = &LLRP_tdRFReceiver;
    if (0 == use_sense_index) {
	rfreceiver_sensitivity.ReceiverSensitivity = rf_sensitivity_index;
    } else {
	rfreceiver_sensitivity.ReceiverSensitivity = rf_sense_index;
    }

    //Pick out the transmit power from the returned capabilities table
#if 0
    LLRP_tSTransmitPowerLevelTableEntry * cur_tx_table_entry =
	pRspCapabilities->pGeneralDeviceCapabilities->listTransmitPowerLevelTableEntry;
    while (cur_tx_table_entry && (0 == use_txpower_index)) {
	if (cur_tx_table_entry->TransmitPowerValue == rf_transmitpower) {
	    rf_txpower_index = cur_tx_table_entry->Index;
	    break;
	}
	cur_tx_table_entry = LLRP_GeneralDeviceCapabilities_nextReceiveSensitivityTableEntry(cur_tx_table_entry); 
    }
#endif

    //Assign the transmit power parameter in the following
    LLRP_tSRFTransmitter rftransmit_param = {
	.hdr.elementHdr.pType = &LLRP_tdRFTransmitter,
	.TransmitPower = rf_txpower_index,
	.HopTableID    = 1,
	.ChannelIndex  = 0
    };

    // Assign the antenna config in the following (includes RX Sens. & TX power)
    LLRP_tSAntennaConfiguration antenna_config = {
	    .hdr.elementHdr.pType = &LLRP_tdAntennaConfiguration,
	    .AntennaID = antenna_id,
	    .pRFReceiver = &rfreceiver_sensitivity,
	    .pRFTransmitter = &rftransmit_param,
	    .listAirProtocolInventoryCommandSettings = NULL
    };

    LLRP_tSEventNotificationState notifications[] = {
        {
            .hdr.elementHdr.pType = &LLRP_tdEventNotificationState,
            .eEventType = LLRP_NotificationEventType_GPI_Event,
            .NotificationState = 1  // enabled
        }
    };

    LLRP_tSReaderEventNotificationSpec events = {
        .hdr.elementHdr.pType = &LLRP_tdReaderEventNotificationSpec,
        .listEventNotificationState = notifications
    };

    LLRP_tSSET_READER_CONFIG set_reader_config = {
        .hdr.elementHdr.pType   = &LLRP_tdSET_READER_CONFIG,
        .ResetToFactoryDefault = 0,
        .pReaderEventNotificationSpec = &events,
        .listAntennaProperties = NULL,
        .listAntennaConfiguration = &antenna_config,
        .pROReportSpec = NULL,
        .pAccessReportSpec = NULL,
        .pKeepaliveSpec = NULL,
        .listGPOWriteData = NULL,
        .listGPIPortCurrentState = gpi_port_state,
        .pEventsAndReports = NULL,
        .listCustom = NULL
    };

    LLRP_tSMessage *pRspMsg;
    LLRP_tSSET_READER_CONFIG_RESPONSE *pRsp;


    // Send the message, expect the response of certain type
    pRspMsg = transact(&set_reader_config.hdr);
    if(pRspMsg == NULL) {
        g_warning("error with SetReaderConfig transaction to configure GPIO");
        return -1;
    }

    // Check the LLRPStatus parameter
    pRsp = (LLRP_tSSET_READER_CONFIG_RESPONSE *)pRspMsg;
    if(checkLLRPStatus(pRsp->pLLRPStatus, "addROSpec") != 0) {
        g_warning("unexpected response in SetReaderConfig transaction to configure GPIO");
        freeMessage(pRspMsg);
        return -1;
    }

    freeMessage(pRspMsg);

    return 0;
}

