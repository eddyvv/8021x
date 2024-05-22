#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lldp.h"
#include "lldp-tlv.h"
#include "lldp/ports.h"
#include "lldp/agent.h"
#include "l2_packet.h"
#include "lldp_util.h"

/* IEEE 802.1AB-2009 - Table 7-1: group MAC addresses used by LLDP */
const u8 nearest_bridge[ETH_ALEN] = {0x01,0x80,0xc2,0x00,0x00,0x0e};
const u8 nearest_nontpmr_bridge[ETH_ALEN] = {0x01,0x80,0xc2,0x00,0x00,0x03};
const u8 nearest_customer_bridge[ETH_ALEN] = {0x01,0x80,0xc2,0x00,0x00,0x00};

struct port *port = NULL;
struct lldp_agent *agent = NULL;

// static const u8 * agent_groupmacs[AGENT_MAX] = {
// 	nearest_bridge,
// 	nearest_nontpmr_bridge,
// 	nearest_customer_bridge,
// };

void lldp_init_agent(struct port *port, struct lldp_agent *agent, int type)
{
    char macstring[30];
    
    memset(agent, 0, sizeof(struct lldp_agent));
    memcpy(&agent->dst_mac_addr, nearest_bridge, ETH_ALEN);
    mac2str(agent->dst_mac_addr, macstring, 30);
    printf("%s: creating new agent for %s (%s).\n", __func__,
		   port->ifname, macstring);
    
    /* Initialize relevant agent variables */
	agent->tx.state  = TX_LLDP_INITIALIZE;
	agent->rx.state = LLDP_WAIT_PORT_OPERATIONAL;
	agent->type = type;
}

int lldp_add_agent(const char *ifname, enum agent_type type)
{
    agent = malloc(sizeof(struct lldp_agent));
	if (!agent) {
		printf("%s: creation of new agent failed !.\n", __func__);
		return -1;
	}
    port = malloc(sizeof(struct port));
    if (port == NULL) {
		printf("run_tx_sm: Failed to malloc struct port\n");
		return false;
	}

    lldp_init_agent(port, agent, type);
    
    return 0;
}


void lldp_ifup(char *ifname, struct lldp_agent *agent)
{
    set_lldp_agent_admin(ifname,NEAREST_BRIDGE,1);
}

void oper_add_dev(char *ifname)
{
    lldp_add_agent(ifname, NEAREST_BRIDGE);
    lldp_ifup(ifname, agent);
}