#ifndef AGENT_H
#define AGENT_H

#include <linux/if_ether.h>

#include "../types.h"



#ifndef ETH_ALEN
#define ETH_ALEN    6
#endif
#ifndef IFNAMSIZ
#define IFNAMSIZ    16  /* must match MAX_DEVICE_NAME_LEN */
#endif
#ifndef ETH_P_ALL
#define ETH_P_ALL   0x0003
#endif


enum agent_type {
	NEAREST_BRIDGE = 0,
	NEAREST_NONTPMR_BRIDGE,
	NEAREST_CUSTOMER_BRIDGE,
	AGENT_MAX,
};

struct agenttx {
	u8 *frameout;
	u32 sizeout;
	u8 state;
	u8 localChange;
	u16 txTTL;
	bool txNow;
	u16 txFast;
};


typedef struct rxmanifest{
	struct unpacked_tlv *chassis;
	struct unpacked_tlv *portid;
	struct unpacked_tlv *ttl;
	struct unpacked_tlv *portdesc;
	struct unpacked_tlv *sysname;
	struct unpacked_tlv *sysdesc;
	struct unpacked_tlv *syscap;
	struct unpacked_tlv *mgmtadd;
} rxmanifest;

struct agentrx {
	u8 *framein;
	u16 sizein;
	u8 state;
	u8 badFrame;
	u8 rcvFrame;
	u8 rxInfoAge;
	u8 remoteChange;
	u8 tooManyNghbrs;
	u8 dupTlvs;
	u8 dcbx_st;
	bool newNeighbor;
	rxmanifest *manifest;
};

/* lldp agent specific structure as in chapter 9.2.5
 * of IEEE 802.1AB-2009 */
struct lldp_agent {
	int	adminStatus;

	int	pad;

	u8	dst_mac_addr[ETH_ALEN];
	u8	sou_mac_addr[ETH_ALEN];

	struct	agentrx rx;
	struct	agenttx tx;
	// struct	agentstats stats;
	// struct	agenttimers timers;
	// u8	rxChanges;
	// u16	lldpdu;
	// struct	msap msap;

	enum	agent_type type;

	// LIST_ENTRY(lldp_agent) entry;
};

int lldp_add_agent(const char *ifname, enum agent_type type);
void oper_add_dev(char *ifname);
#endif