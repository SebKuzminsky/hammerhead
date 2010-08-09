/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "BionetProtocol1"
 * 	found in "bionet-protocol-0.asn1"
 * 	`asn1c -Werror -fnative-types -fskeletons-copy -pdu=auto -gen-PER`
 */

#ifndef	_BDM_Datapoint_H_
#define	_BDM_Datapoint_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Datapoint.h"
#include "BDM-Event.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BDM-Datapoint */
typedef struct BDM_Datapoint {
	Datapoint_t	 datapoint;
	BDM_Event_t	 event;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BDM_Datapoint_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BDM_Datapoint;

#ifdef __cplusplus
}
#endif

#endif	/* _BDM_Datapoint_H_ */