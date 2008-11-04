/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "BionetProtocol0"
 * 	found in "bionet-protocol-0.asn1"
 * 	`asn1c -Werror -fnative-types -fskeletons-copy -pdu=auto`
 */

#ifndef	_Datapoint_H_
#define	_Datapoint_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Value.h"
#include <GeneralizedTime.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Datapoint */
typedef struct Datapoint {
	Value_t	 value;
	GeneralizedTime_t	 timestamp;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Datapoint_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Datapoint;

#ifdef __cplusplus
}
#endif

#endif	/* _Datapoint_H_ */