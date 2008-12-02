/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "BionetProtocol0"
 * 	found in "bionet-protocol-0.asn1"
 * 	`asn1c -Werror -fnative-types -fskeletons-copy -pdu=auto`
 */

#ifndef	_SetResourceValue_H_
#define	_SetResourceValue_H_


#include <asn_application.h>

/* Including external dependencies */
#include <PrintableString.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SetResourceValue */
typedef struct SetResourceValue {
	PrintableString_t	 nodeId;
	PrintableString_t	 resourceId;
	PrintableString_t	 value;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SetResourceValue_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SetResourceValue;

#ifdef __cplusplus
}
#endif

#endif	/* _SetResourceValue_H_ */
