/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "BionetProtocol0"
 * 	found in "bionet-protocol-0.asn1"
 * 	`asn1c -Werror -fnative-types -fskeletons-copy -pdu=auto`
 */

#include <asn_internal.h>

#include "C2H-Message.h"

static asn_TYPE_member_t asn_MBR_C2H_Message_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct C2H_Message, choice.subscribeNode),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PrintableString,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"subscribeNode"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct C2H_Message, choice.subscribeDatapoints),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PrintableString,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"subscribeDatapoints"
		},
};
static asn_TYPE_tag2member_t asn_MAP_C2H_Message_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* subscribeNode at 109 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* subscribeDatapoints at 110 */
};
static asn_CHOICE_specifics_t asn_SPC_C2H_Message_specs_1 = {
	sizeof(struct C2H_Message),
	offsetof(struct C2H_Message, _asn_ctx),
	offsetof(struct C2H_Message, present),
	sizeof(((struct C2H_Message *)0)->present),
	asn_MAP_C2H_Message_tag2el_1,
	2,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
static asn_per_constraints_t asn_PER_C2H_Message_constr_1 = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 }
};
asn_TYPE_descriptor_t asn_DEF_C2H_Message = {
	"C2H-Message",
	"C2H-Message",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	CHOICE_decode_uper,
	CHOICE_encode_uper,
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	&asn_PER_C2H_Message_constr_1,
	asn_MBR_C2H_Message_1,
	2,	/* Elements count */
	&asn_SPC_C2H_Message_specs_1	/* Additional specs */
};

