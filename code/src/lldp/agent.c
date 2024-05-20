#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lldp-tlv.h"
#include "lldp/ports.h"
#include "lldp/agent.h"


/* IEEE 802.1AB-2009 - Table 7-1: group MAC addresses used by LLDP */
const u8 nearest_bridge[ETH_ALEN] = LLDP_ADDR_NEAREST_BRIDGE;
const u8 nearest_nontpmr_bridge[ETH_ALEN] = LLDP_ADDR_NEAREST_NONTPMR_BRIDGE;
const u8 nearest_customer_bridge[ETH_ALEN] = LLDP_ADDR_NEAREST_CUSTOMER_BRIDGE;


static const u8 * agent_groupmacs[AGENT_MAX] = {
	nearest_bridge,
	nearest_nontpmr_bridge,
	nearest_customer_bridge,
};

void lldp_init_agent(struct port *port, struct lldp_agent *agent, int type)
{
    char macstring[30];

    memset(agent, 0, sizeof(struct lldp_agent));
    memcpy(&agent->mac_addr, agent_groupmacs[type], ETH_ALEN);
    mac2str(agent->mac_addr, macstring, 30);
    printf("%s: creating new agent for %s (%s).\n", __func__,
		   port->ifname, macstring);

    /* Initialize relevant agent variables */
	agent->tx.state  = TX_LLDP_INITIALIZE;
	agent->rx.state = LLDP_WAIT_PORT_OPERATIONAL;
	agent->type = type;
}

int lldp_add_agent(const char *ifname, enum agent_type type)
{
    struct lldp_agent *newagent;
    struct port *port;

    newagent = malloc(sizeof(*newagent));
	if (!newagent) {
		printf("%s: creation of new agent failed !.\n", __func__);
		return -1;
	}

    lldp_init_agent(port, newagent, type);

    return 0;
}
