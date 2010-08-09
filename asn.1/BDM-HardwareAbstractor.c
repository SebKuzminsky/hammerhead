/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "BionetProtocol1"
 * 	found in "bionet-protocol-0.asn1"
 * 	`asn1c -Werror -fnative-types -fskeletons-copy -pdu=auto -gen-PER`
 */

#include <asn_internal.h>

#include "BDM-HardwareAbstractor.h"

static asn_TYPE_member_t asn_MBR_nodes_4[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_BDM_Node,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_nodes_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_nodes_specs_4 = {
	sizeof(struct BDM_HardwareAbstractor__nodes),
	offsetof(struct BDM_HardwareAbstractor__nodes, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_nodes_4 = {
	"nodes",
	"nodes",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	SEQUENCE_OF_decode_uper,
	SEQUENCE_OF_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_nodes_tags_4,
	sizeof(asn_DEF_nodes_tags_4)
		/sizeof(asn_DEF_nodes_tags_4[0]) - 1, /* 1 */
	asn_DEF_nodes_tags_4,	/* Same as above */
	sizeof(asn_DEF_nodes_tags_4)
		/sizeof(asn_DEF_nodes_tags_4[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_nodes_4,
	1,	/* Single element */
	&asn_SPC_nodes_specs_4	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_events_6[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_BDM_Event,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_events_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_events_specs_6 = {
	sizeof(struct BDM_HardwareAbstractor__events),
	offsetof(struct BDM_HardwareAbstractor__events, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_events_6 = {
	"events",
	"events",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	SEQUENCE_OF_decode_uper,
	SEQUENCE_OF_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_events_tags_6,
	sizeof(asn_DEF_events_tags_6)
		/sizeof(asn_DEF_events_tags_6[0]) - 1, /* 1 */
	asn_DEF_events_tags_6,	/* Same as above */
	sizeof(asn_DEF_events_tags_6)
		/sizeof(asn_DEF_events_tags_6[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_events_6,
	1,	/* Single element */
	&asn_SPC_events_specs_6	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_BDM_HardwareAbstractor_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct BDM_HardwareAbstractor, type),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PrintableString,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"type"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BDM_HardwareAbstractor, id),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PrintableString,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"id"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BDM_HardwareAbstractor, nodes),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_nodes_4,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"nodes"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct BDM_HardwareAbstractor, events),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_events_6,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"events"
		},
};
static ber_tlv_tag_t asn_DEF_BDM_HardwareAbstractor_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_BDM_HardwareAbstractor_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* type at 331 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* id at 332 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* nodes at 333 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* events at 335 */
};
static asn_SEQUENCE_specifics_t asn_SPC_BDM_HardwareAbstractor_specs_1 = {
	sizeof(struct BDM_HardwareAbstractor),
	offsetof(struct BDM_HardwareAbstractor, _asn_ctx),
	asn_MAP_BDM_HardwareAbstractor_tag2el_1,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_BDM_HardwareAbstractor = {
	"BDM-HardwareAbstractor",
	"BDM-HardwareAbstractor",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_BDM_HardwareAbstractor_tags_1,
	sizeof(asn_DEF_BDM_HardwareAbstractor_tags_1)
		/sizeof(asn_DEF_BDM_HardwareAbstractor_tags_1[0]), /* 1 */
	asn_DEF_BDM_HardwareAbstractor_tags_1,	/* Same as above */
	sizeof(asn_DEF_BDM_HardwareAbstractor_tags_1)
		/sizeof(asn_DEF_BDM_HardwareAbstractor_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_BDM_HardwareAbstractor_1,
	4,	/* Elements count */
	&asn_SPC_BDM_HardwareAbstractor_specs_1	/* Additional specs */
};
