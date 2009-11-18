/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "BionetProtocol0"
 * 	found in "bionet-protocol-0.asn1"
 * 	`asn1c -Werror -fnative-types -fskeletons-copy -pdu=auto`
 */

#ifndef	_BDMResourceDatapoints_H_
#define	_BDMResourceDatapoints_H_


#include <asn_application.h>

/* Including external dependencies */
#include <PrintableString.h>
#include <NativeInteger.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct Datapoint;

/* BDMResourceDatapoints */
typedef struct BDMResourceDatapoints {
	PrintableString_t	 bdmId;
	PrintableString_t	 habType;
	PrintableString_t	 habId;
	PrintableString_t	 nodeId;
	PrintableString_t	 resourceId;
	struct newDatapointsBDM {
		A_SEQUENCE_OF(struct Datapoint) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} newDatapointsBDM;
	long	 entrySeq;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BDMResourceDatapoints_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BDMResourceDatapoints;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "Datapoint.h"

#endif	/* _BDMResourceDatapoints_H_ */
