#include "lldp-tlv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

void send_lldp()
{

}

