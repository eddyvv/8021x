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


#endif