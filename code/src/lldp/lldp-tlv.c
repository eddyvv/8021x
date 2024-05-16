#include "lldp-tlv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int tlv_ok(struct unpacked_tlv *tlv)
{
	if (!tlv || (!tlv->length && tlv->type))
		return 0;
	else
		return 1;
}

struct packed_tlv *pack_tlv(struct unpacked_tlv *tlv)
{
	u16 tl = 0;
	struct packed_tlv *pkd_tlv = NULL;

	if (!tlv_ok(tlv))
		return NULL;

	tl = tlv->type;
	tl = tl << 9;
	tl |= tlv->length & 0x01ff;
	tl = htons(tl);

	pkd_tlv = (struct packed_tlv *)malloc(sizeof(struct packed_tlv));
	if(!pkd_tlv) {
		printf("pack_tlv: Failed to malloc pkd_tlv\n");
		return NULL;
	}
	memset(pkd_tlv,0,sizeof(struct packed_tlv));
	pkd_tlv->size = tlv->length + sizeof(tl);
	pkd_tlv->tlv = (u8 *)malloc(pkd_tlv->size);
	if(pkd_tlv->tlv) {
		memset(pkd_tlv->tlv,0, pkd_tlv->size);
		memcpy(pkd_tlv->tlv, &tl, sizeof(tl));
		if (tlv->length)
			memcpy(&pkd_tlv->tlv[sizeof(tl)], tlv->info,
				tlv->length);
	} else {
		printf("pack_tlv: Failed to malloc tlv\n");
		free(pkd_tlv);
		pkd_tlv = NULL;
		return NULL;
	}
	return pkd_tlv;
}

struct unpacked_tlv *free_unpkd_tlv(struct unpacked_tlv *tlv)
{
	if (tlv != NULL) {
		if (tlv->info != NULL) {
			free(tlv->info);
			tlv->info = NULL;
		}
		free(tlv);
		tlv = NULL;
	}
	return NULL;
}

struct packed_tlv *free_pkd_tlv(struct packed_tlv *tlv)
{
	if (tlv != NULL) {
		if (tlv->tlv != NULL) {
			free(tlv->tlv);
			tlv->tlv = NULL;
		}
		free(tlv);
		tlv = NULL;
	}
	return NULL;
}

struct packed_tlv *create_ptlv()
{
	struct packed_tlv *ptlv =
		(struct packed_tlv *)malloc(sizeof(struct packed_tlv));

	if(ptlv)
		memset(ptlv, 0, sizeof(struct packed_tlv));
	return ptlv;
}

struct unpacked_tlv *create_tlv()
{
	struct unpacked_tlv *tlv =
		(struct unpacked_tlv *)malloc(sizeof(struct unpacked_tlv));

	if(tlv) {
		memset(tlv,0, sizeof(struct unpacked_tlv));
		return tlv;
	} else {
		printf("create_tlv: Failed to malloc tlv\n");
		return NULL;
	}
}

struct unpacked_tlv *bld_end_tlv()
{
	struct unpacked_tlv *tlv = create_tlv();

	if(tlv) {
		tlv->type = END_OF_LLDPDU_TLV;
		tlv->length = 0;
		tlv->info = NULL;
	}
	return tlv;
}


struct packed_tlv *pack_end_tlv()
{
	struct unpacked_tlv *tlv;
	struct packed_tlv *ptlv;

	tlv = bld_end_tlv();
	ptlv = pack_tlv(tlv);
	free(tlv);
	return ptlv;
}