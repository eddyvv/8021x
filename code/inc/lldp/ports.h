#ifndef PORTS_H
#define PORTS_H

#include "types.h"
#include "agent.h"

/* lldp port specific structure */
struct port {
	struct port *next;
	int ifindex;
	u8 hw_resetting;
	u8 portEnabled;
	u8 prevPortEnabled;
	u8 bond_master;		/* True if a bond master */
	// struct porttimers *timers;

	// u16 dormantDelay;

	// LIST_HEAD(agent_head, lldp_agent) agent_head;
	// struct l2_packet_data *l2;
	char ifname[IFNAMSIZ];
};

#endif