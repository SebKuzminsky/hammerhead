/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "BionetProtocol0"
 * 	found in "bionet-protocol-0.asn1"
 * 	`asn1c -Werror -fnative-types -fskeletons-copy -pdu=auto`
 */

#ifndef	_ResourceDatapointsReply_H_
#define	_ResourceDatapointsReply_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct HardwareAbstractor;

/* ResourceDatapointsReply */
typedef struct ResourceDatapointsReply {
	A_SEQUENCE_OF(struct HardwareAbstractor) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ResourceDatapointsReply_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ResourceDatapointsReply;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "HardwareAbstractor.h"

#endif	/* _ResourceDatapointsReply_H_ */
