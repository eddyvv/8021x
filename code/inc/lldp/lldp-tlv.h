#ifndef LLDP_TLV_H
#define LLDP_TLV_H

#include "types.h"
#include "lldp.h"


struct packed_tlv {
	u16 size;     /* Of the entire tlv block */
	u8 *tlv;      /* tlv block */
};

struct unpacked_tlv {
	u8  type;
	u16 length;
	u8  *info;
};


struct unpacked_tlv *free_unpkd_tlv(struct unpacked_tlv *tlv);
struct packed_tlv *free_pkd_tlv(struct packed_tlv *tlv);
struct unpacked_tlv *create_tlv();
struct packed_tlv *create_ptlv(void);

struct unpacked_tlv *bld_end_tlv();

int tlv_ok(struct unpacked_tlv *tlv);
struct packed_tlv *pack_end_tlv();
#endif