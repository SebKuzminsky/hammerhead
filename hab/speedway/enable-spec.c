
#include <errno.h>
#include <stdio.h>

#include <glib.h>

#include "ltkc.h"
#include "speedway.h"


/*
 * Enable our ROSpec using ENABLE_ROSPEC message.
 *
 * The message we send is:
 * <ENABLE_ROSPEC MessageID='202'>
 * 	<ROSpecID>123</ROSpecID>
 * </ENABLE_ROSPEC>
 */

int enableROSpec (void)
{
    LLRP_tSENABLE_ROSPEC Cmd = {
        .hdr.elementHdr.pType   = &LLRP_tdENABLE_ROSPEC,
        .hdr.MessageID          = 202,
        .ROSpecID               = 123,
    };

    LLRP_tSMessage *pRspMsg;
    LLRP_tSENABLE_ROSPEC_RESPONSE *pRsp;

    /*
     * Send the message, expect the response of certain type
     */
    pRspMsg = transact(&Cmd.hdr);

    if(pRspMsg == NULL) {
        return -1;
    }

    /*
     * Cast to a ENABLE_ROSPEC_RESPONSE message.
     */
    pRsp = (LLRP_tSENABLE_ROSPEC_RESPONSE *) pRspMsg;

    /*
     * Check the LLRPStatus parameter.
     */
    if(checkLLRPStatus(pRsp->pLLRPStatus, "enableROSpec") != 0) {
        freeMessage(pRspMsg);

        return -1;
    }

    /*
     * Done with the response message.
     */
    freeMessage(pRspMsg);

    if(g_verbose) {
        printf("INFO: ROSpec enabled\n");
    }

    return 0;
}

