/* -*- mode: c; c-file-style: "openbsd" -*- */
/*
 * Copyright (c) 2012 Vincent Bernat <bernat@luffy.cx>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _LLDP_TLV_H
#define _LLDP_TLV_H

#include <asm/types.h>
#include <linux/if_ether.h>
#include <string.h>
#include "types.h"

#define LLDP_ADDR_NEAREST_BRIDGE       \
  {                                    \
    0x01, 0x80, 0xc2, 0x00, 0x00, 0x0e \
  }
#define LLDP_ADDR_NEAREST_NONTPMR_BRIDGE \
  {                                      \
    0x01, 0x80, 0xc2, 0x00, 0x00, 0x03   \
  }
#define LLDP_ADDR_NEAREST_CUSTOMER_BRIDGE \
  {                                       \
    0x01, 0x80, 0xc2, 0x00, 0x00, 0x00    \
  }

#define LLDP_TLV_END 0
#define LLDP_TLV_CHASSIS_ID 1
#define LLDP_TLV_PORT_ID 2
#define LLDP_TLV_TTL 3
#define LLDP_TLV_PORT_DESCR 4
#define LLDP_TLV_SYSTEM_NAME 5
#define LLDP_TLV_SYSTEM_DESCR 6
#define LLDP_TLV_SYSTEM_CAP 7
#define LLDP_TLV_MGMT_ADDR 8

#define LLDP_TLV_ORG_DOT1 \
  {                       \
    0x00, 0x80, 0xc2      \
  }
#define LLDP_TLV_ORG_DOT3 \
  {                       \
    0x00, 0x12, 0x0f      \
  }
#define LLDP_TLV_ORG_MED \
  {                      \
    0x00, 0x12, 0xbb     \
  }
#define LLDP_TLV_ORG_DCBX \
  {                       \
    0x00, 0x1b, 0x21      \
  }

#define LLDP_TLV_DOT1_PVID 1
#define LLDP_TLV_DOT1_PPVID 2
#define LLDP_TLV_DOT1_VLANNAME 3
#define LLDP_TLV_DOT1_PI 4

#define LLDP_TLV_DOT3_MAC 1
#define LLDP_TLV_DOT3_POWER 2
#define LLDP_TLV_DOT3_LA 3
#define LLDP_TLV_DOT3_MFS 4

#define LLDP_TLV_MED_CAP 1
#define LLDP_TLV_MED_POLICY 2
#define LLDP_TLV_MED_LOCATION 3
#define LLDP_TLV_MED_MDI 4
#define LLDP_TLV_MED_IV_HW 5
#define LLDP_TLV_MED_IV_FW 6
#define LLDP_TLV_MED_IV_SW 7
#define LLDP_TLV_MED_IV_SN 8
#define LLDP_TLV_MED_IV_MANUF 9
#define LLDP_TLV_MED_IV_MODEL 10
#define LLDP_TLV_MED_IV_ASSET 11


#define UNUSED __attribute__((__unused__))

#define MIN(x,y) \
	({	\
		typeof (x) __x = (x);	\
		typeof (y) __y = (y);	\
		__x < __y ? __x : __y;	\
	 })

#define MAX(x,y) \
	({	\
		typeof (x) __x = (x);	\
		typeof (y) __y = (y);	\
		__x > __y ? __x : __y;	\
	 })

/*
 * Organizationally Unique Identifier (OUI)
 * http://standards.ieee.org/regauth/oui/oui.txt
 */
#define OUI_SIZE	3
#define SUB_SIZE	1
#define OUI_SUB_SIZE	(OUI_SIZE + SUB_SIZE)
#define OUI_INTEL_CORP	0x001b21
#define OUI_CEE_DCBX	OUI_INTEL_CORP
#define OUI_IEEE_8021	0x0080c2
#define OUI_IEEE_8023	0x00120f
#define OUI_IEEE_80211	0x000fac
/* Telecommunications Industry Association TR-41 Committee */
#define OUI_TIA_TR41	0x0012bb
/* Ciso OUI */
#define OUI_CISCO	0x000142

#define OUI_IEEE_8021Qbg	0x001b3f	/* Draft 0.1 value */
#define OUI_IEEE_8021Qbg22	0x0080c2	/* Standardized value */

/* IEEE 802.3AB Clause 9: TLV Types */
#define CHASSIS_ID_TLV    1
#define PORT_ID_TLV       2
#define TIME_TO_LIVE_TLV  3
#define PORT_DESCRIPTION_TLV	4
#define SYSTEM_NAME_TLV		5
#define SYSTEM_DESCRIPTION_TLV	6
#define SYSTEM_CAPABILITIES_TLV	7
#define MANAGEMENT_ADDRESS_TLV	8
#define ORG_SPECIFIC_TLV  127
#define END_OF_LLDPDU_TLV 0

/* IEEE 802.3AB Clause 9.5.2: Chassis subtypes */
#define CHASSIS_ID_RESERVED	     0
#define CHASSIS_ID_CHASSIS_COMPONENT 1
#define CHASSIS_ID_INTERFACE_ALIAS   2
#define CHASSIS_ID_PORT_COMPONENT    3
#define CHASSIS_ID_MAC_ADDRESS       4
#define CHASSIS_ID_NETWORK_ADDRESS   5
#define CHASSIS_ID_INTERFACE_NAME    6
#define CHASSIS_ID_LOCALLY_ASSIGNED  7
#define CHASSIS_ID_INVALID(t)	(((t) == 0) || ((t) > 7))

/* IEEE 802.3AB Clause 9.5.3: Port subtype */
#define PORT_ID_RESERVED	 0
#define PORT_ID_INTERFACE_ALIAS  1
#define PORT_ID_PORT_COMPONENT   2
#define PORT_ID_MAC_ADDRESS      3
#define PORT_ID_NETWORK_ADDRESS  4
#define PORT_ID_INTERFACE_NAME   5
#define PORT_ID_AGENT_CIRCUIT_ID 6
#define PORT_ID_LOCALLY_ASSIGNED 7
#define PORT_ID_INVALID(t)	(((t) == 0) || ((t) > 7))

/* IEEE 802.1AB: Annex E, Table E.1: Organizationally Specific TLVs */
#define ORG_SPEC_PVID		1
#define ORG_SPEC_PPVID		2
#define ORG_SPEC_VLAN_NAME	3
#define ORG_SPEC_PROTO_ID	4
#define ORG_SPEC_VID_USAGE	5
#define ORG_SPEC_MGMT_VID	6
#define ORG_SPEC_LINK_AGGR	7
#define ORG_SPEC_INVALID(t)	(((t) == 0) || ((t) > 7))

/* IEEE 802.1AB: 8.5.8, Table 8-4: System Capabilities */
#define SYSCAP_OTHER	(1 << 0)
#define SYSCAP_REPEATER (1 << 1)
#define SYSCAP_BRIDGE	(1 << 2)
#define SYSCAP_WLAN	(1 << 3)
#define SYSCAP_ROUTER	(1 << 4)
#define SYSCAP_PHONE	(1 << 5)
#define SYSCAP_DOCSIS	(1 << 6)
#define SYSCAP_STATION	(1 << 7)
#define SYSCAP_CVLAN	(1 << 8)	/* TPID: 0x8100 */
#define SYSCAP_SVLAN	(1 << 9)	/* TPID: 0x88a8 */
#define SYSCAP_TPMR	(1 << 10)
#define SYSCAP_BITMASK	0x0fff

/*
 * IETF RFC 3232:
 * http://www.iana.org/assignments/ianaaddressfamilynumbers-mib
 */
enum {
	MANADDR_OTHER = 0,
	MANADDR_IPV4 = 1,
	MANADDR_IPV6 = 2,
	MANADDR_NSAP = 3,
	MANADDR_HDLC = 4,
	MANADDR_BBN1822 = 5,
	MANADDR_ALL802 = 6,
	MANADDR_E163 = 7,
	MANADDR_E164 = 8,
	MANADDR_F69 = 9,
	MANADDR_X121 = 10,
	MANADDR_IPX = 11,
	MANADDR_APPLETALK = 12,
	MANADDR_DECNETIV = 13,
	MANADDR_BANYANVINES = 14,
	MANADDR_E164WITHNSAP = 15,
	MANADDR_DNS = 16,
	MANADDR_DISTINGUISHEDNAME = 17,
	MANADDR_ASNUMBER = 18,
	MANADDR_XTPOVERIPV4 = 19,
	MANADDR_XTPOVERIPV6 = 20,
	MANADDR_XTPNATIVEMODEXTP = 21,
	MANADDR_FIBRECHANNELWWPN = 22,
	MANADDR_FIBRECHANNELWWNN = 23,
	MANADDR_GWID = 24,
	MANADDR_AFI = 25,
	MANADDR_RESERVED = 65535
}; 
#define MANADDR_MAX	(MANADDR_AFI - MANADDR_OTHER + 2)

#define IFNUM_UNKNOWN      1
#define IFNUM_IFINDEX      2
#define IFNUM_SYS_PORT_NUM 3

/* LLDP-MED subtypes */
#define LLDP_MED_RESERVED	0
#define LLDP_MED_CAPABILITIES	1
#define LLDP_MED_NETWORK_POLICY	2
#define LLDP_MED_LOCATION_ID	3
#define LLDP_MED_EXTENDED_PVMDI	4
#define LLDP_MED_INV_HWREV	5
#define LLDP_MED_INV_FWREV	6
#define LLDP_MED_INV_SWREV	7
#define LLDP_MED_INV_SERIAL	8
#define LLDP_MED_INV_MANUFACTURER	9
#define LLDP_MED_INV_MODELNAME	10
#define LLDP_MED_INV_ASSETID	11

/* LLDP-MED Capability Values: ANSI/TIA-1057-2006, 10.2.2.1, Table 10 */
#define LLDP_MED_CAPABILITY_CAPAPILITIES	(1 << 0)
#define LLDP_MED_CAPABILITY_NETWORK_POLICY	(1 << 1)
#define LLDP_MED_CAPABILITY_LOCATION_ID		(1 << 2)
#define LLDP_MED_CAPABILITY_EXTENDED_PSE	(1 << 3)
#define LLDP_MED_CAPABILITY_EXTENDED_PD		(1 << 4)
#define LLDP_MED_CAPABILITY_INVENTORY		(1 << 5)

/* LLDP-MED Device Type Values: ANSI/TIA-1057-2006, 10.2.2.2, Table 11 */
#define LLDP_MED_DEVTYPE_NOT_DEFINED		0
#define LLDP_MED_DEVTYPE_ENDPOINT_CLASS_I	1
#define LLDP_MED_DEVTYPE_ENDPOINT_CLASS_II	2
#define LLDP_MED_DEVTYPE_ENDPOINT_CLASS_III	3
#define LLDP_MED_DEVTYPE_NETWORK_CONNECTIVITY	4
#define LLDP_MED_DEVTYPE_INVALID(t)	((t) > 4)
#define LLDP_MED_DEVTYPE_DEFINED(t)	\
	(((t) == LLDP_MED_DEVTYPE_ENDPOINT_CLASS_I) || 	\
	 ((t) == LLDP_MED_DEVTYPE_ENDPOINT_CLASS_II) ||	\
	 ((t) == LLDP_MED_DEVTYPE_ENDPOINT_CLASS_III) ||\
	 ((t) == LLDP_MED_DEVTYPE_NETWORK_CONNECTIVITY))

#define LLDP_MED_LOCID_INVALID		0
#define LLDP_MED_LOCID_COORDINATE	1 
#define LLDP_MED_LOCID_CIVIC_ADDRESS	2
#define LLDP_MED_LOCID_ECS_ELIN		3
#define LLDP_MED_LOCID_FORMAT_INVALID(t) (((t) == 0) || ((t) > 3))

/* IEEE 802.1Qaz Organizationally Specific TLV Subtypes */
#define LLDP_8021QAZ_ETSCFG	9
#define LLDP_8021QAZ_ETSREC	10
#define LLDP_8021QAZ_PFC	11
#define LLDP_8021QAZ_APP	12

/* IEEE 802.3 Organizationally Specific TLV Subtypes: 802.1AB-2005 Annex G */
#define LLDP_8023_RESERVED		0
#define LLDP_8023_MACPHY_CONFIG_STATUS	1
#define LLDP_8023_POWER_VIA_MDI		2	
#define LLDP_8023_LINK_AGGREGATION	3
#define LLDP_8023_MAXIMUM_FRAME_SIZE	4
/* 802.3AB-2005 G.2.1 Table G-2 */
#define LLDP_8023_MACPHY_AUTONEG_SUPPORT	(1 << 0)
#define LLDP_8023_MACPHY_AUTONEG_ENABLED	(1 << 1)
/* 802.3AB-2005 G.4.1 Table G-4 */
#define LLDP_8023_LINKAGG_CAPABLE	(1 << 0)
#define LLDP_8023_LINKAGG_ENABLED	(1 << 1)

/* IEEE 802.1Qbg subtype */
#define LLDP_EVB_SUBTYPE		0	/* Draft 0.1 value */
#define LLDP_EVB22_SUBTYPE		0xd	/* Standardized value */
#define LLDP_VDP_SUBTYPE		0x2

/* forwarding mode */
#define LLDP_EVB_CAPABILITY_FORWARD_STANDARD		(1 << 7)
#define LLDP_EVB_CAPABILITY_FORWARD_REFLECTIVE_RELAY	(1 << 6)

/* EVB supported protocols */
#define LLDP_EVB_CAPABILITY_PROTOCOL_RTE		(1 << 2)
#define LLDP_EVB_CAPABILITY_PROTOCOL_ECP		(1 << 1)
#define LLDP_EVB_CAPABILITY_PROTOCOL_VDP		(1 << 0)

/* EVB specific values */
#define LLDP_EVB_DEFAULT_MAX_VSI			65535
#define LLDP_EVB_DEFAULT_SVSI				3295
#define LLDP_EVB_DEFAULT_RTE				15
#define LLDP_EVB_DEFAULT_MAX_RTE			31

#ifndef _MSC_VER
#define STRUCT_PACKED(STRUCT) STRUCT __attribute__((__packed__))
#else
#define STRUCT_PACKED(STRUCT) __pragma(pack(push, 1)) STRUCT __pragma(pack(pop))
#endif

/* Tx States */
enum {
	TX_LLDP_INITIALIZE,
	TX_IDLE,
	TX_SHUTDOWN_FRAME,
	TX_INFO_FRAME 
};

/******************************************************************************/
/* Rx States */
enum {
	LLDP_WAIT_PORT_OPERATIONAL = 4,
	DELETE_AGED_INFO,
	RX_LLDP_INITIALIZE,
	RX_WAIT_FOR_FRAME,
	RX_FRAME,
	DELETE_INFO,
	UPDATE_INFO
};

struct l2_packet_data {
	int fd;	/* socketfd */
	int ifindex;	/* 接口号 */
	u8 dst_addr[ETH_ALEN];
	// u8 sou_addr[ETH_ALEN];

};

void set_lldp_agent_admin(const char *ifname, int type, int admin);
void show_lldp_buf(u8 *buf, int len);

#endif
