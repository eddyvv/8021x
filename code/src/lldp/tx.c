#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "ports.h"
#include "l2_packet.h"
#include "agent.h"
#include "lldp_util.h"
// #include "states.h"
// #include "messages.h"
// #include "lldpad.h"
#include "lldp-tlv.h"
// #include "lldp_mod.h"
// #include "lldp_mand.h"
// #include "config.h"
// #include "lldp_mand_clif.h"

extern struct port *port;
extern struct lldp_agent *agent;

void txInitializeLLDP(struct port *port, struct lldp_agent *agent)
{

}

struct packed_tlv *test_gettlv(struct port *port, int size)
{
    // int size;
    struct packed_tlv *ptlv = NULL;

    ptlv = create_ptlv();
	if (!ptlv)
		goto out_err;

	ptlv->tlv = malloc(size);
	if (!ptlv->tlv)
		goto out_free;

    ptlv->size = 0;
    return ptlv;
out_free:
	free_pkd_tlv(ptlv);
out_err:
	printf("%s:%s: failed\n", __func__, port->ifname);
	return NULL;
}


void config_tlv(struct unpacked_tlv *ptlv, int tlv_type)
{

    switch(tlv_type)
    {
        case CHASSIS_ID_TLV:
        {
            ptlv->type = CHASSIS_ID_TLV;
            ptlv->length = sizeof(struct lldp_dev_id);
        }
        break;

        case PORT_ID_TLV:
        {
            ptlv->type = PORT_ID_TLV;
            ptlv->length = sizeof(struct lldp_port_id);
        }
        break;

        case TIME_TO_LIVE_TLV:
        {
            // ptlv->type = TIME_TO_LIVE_TLV;
            ptlv->type = 6;
            ptlv->length = sizeof(struct lldp_time_to_live);
        }
        break;

        case SYSTEM_NAME_TLV:
        {
            // ptlv->type = SYSTEM_NAME_TLV;
            ptlv->type = 10;
            ptlv->length = sizeof(struct lldp_system_name);
        }
        break;

        default:
            ptlv->type = 4;
            ptlv->length = 2;
            printf("ERR:tlv_type err\r\n");
        break;
    }
}


#define htonl__(x)        (u32)((((x) & 0x000000ff) << 24) | \
                                 (((x) & 0x0000ff00) <<  8) | \
                                 (((x) & 0x00ff0000) >>  8) | \
                                 (((x) & 0xff000000) >> 24))

int config_tlv_buf(struct port *port, struct lldp_agent *agent, int tlv_type)
{
    struct unpacked_tlv *ptlv =  NULL;
    struct packed_tlv *tlv;

    switch(tlv_type)
    {
        case CHASSIS_ID_TLV:
        {
            ptlv = test_gettlv(port, 2);
            if (ptlv)
            {
                // config_tlv(ptlv, CHASSIS_ID_TLV);
                config_tlv(ptlv, 2);
                tlv = pack_tlv(ptlv);
                tlv->tlv[2] = 6;
                memcpy(agent->tx.frameout + agent->tx.sizeout, tlv->tlv, tlv->size);
            }
        }
        break;

        case PORT_ID_TLV:
        {
            ptlv = test_gettlv(port, 2);
            if (ptlv)
            {
                // config_tlv(ptlv, PORT_ID_TLV);
                config_tlv(ptlv, 4);
                tlv = pack_tlv(ptlv);
                tlv->tlv[2] = 5;
                memcpy(agent->tx.frameout + agent->tx.sizeout, tlv->tlv, tlv->size);
            }
        }
        break;

        case TIME_TO_LIVE_TLV:
        {
            ptlv = test_gettlv(port, 4);
            if (ptlv)
            {
                config_tlv(ptlv, TIME_TO_LIVE_TLV);
                tlv = pack_tlv(ptlv);
                *(short *)&tlv->tlv[2] = 0xFFFF;
                memcpy(agent->tx.frameout + agent->tx.sizeout, tlv->tlv, tlv->size);
            }
        }
        break;

        case SYSTEM_NAME_TLV:
        {
            ptlv = test_gettlv(port, 4);
            if (ptlv)
            {
                // char *str = "IOM1";
                config_tlv(ptlv, SYSTEM_NAME_TLV);
                tlv = pack_tlv(ptlv);
                *(u64 *)&tlv->tlv[2] = 'IOM1';
                *(u32 *)&tlv->tlv[2] = htonl__(*(u32 *)&tlv->tlv[2]);
                // str_2_hex_str(&tlv->tlv[2], str);
                memcpy(agent->tx.frameout + agent->tx.sizeout, tlv->tlv, tlv->size);
            }
        }
        break;

        default:
            printf("ERR:tlv_type err\r\n");
        break;
    }
    return tlv->size;
}

bool mibConstrInfoLLDPDU(struct port *port, struct lldp_agent *agent)
{
    struct l2_ethhdr eth;
    char macstring[30];
    u32 fb_offset = 0;
    
    if (agent->tx.frameout) {
		free(agent->tx.frameout);
		agent->tx.frameout = NULL;
	}

    memset(&eth, 0, sizeof(struct l2_ethhdr));

    mac2str(agent->dst_mac_addr, macstring, 30);
	printf("%s: port %s mac %s type %i.\n", __func__, port->ifname,
		   macstring, agent->type);

    /* mac头 */
    memcpy(eth.h_dest, agent->dst_mac_addr, ETH_ALEN);
    memcpy(eth.h_source, agent->sou_mac_addr, ETH_ALEN);

    /* type 0x88cc */
    eth.h_proto = htons(ETH_P_LLDP);

    agent->tx.frameout =  (u8 *)malloc(ETH_FRAME_LEN);
	if (agent->tx.frameout == NULL) {
		printf("InfoLLDPDU: Failed to malloc frame buffer \n");
		return false;
	}

    memset(agent->tx.frameout, 0, ETH_FRAME_LEN);

    memcpy(agent->tx.frameout, (void *)&eth, sizeof(struct l2_ethhdr));
	fb_offset += sizeof(struct l2_ethhdr);
    agent->tx.sizeout = fb_offset;

    fb_offset += config_tlv_buf(port, agent, CHASSIS_ID_TLV);
    agent->tx.sizeout = fb_offset;

    fb_offset += config_tlv_buf(port, agent, PORT_ID_TLV);
    agent->tx.sizeout = fb_offset;

    fb_offset += config_tlv_buf(port, agent, TIME_TO_LIVE_TLV);
    agent->tx.sizeout = fb_offset;

    fb_offset += config_tlv_buf(port, agent, SYSTEM_NAME_TLV);
    agent->tx.sizeout = fb_offset;
}


/* 发送 */
u8 txFrame(struct port *port, struct lldp_agent *agent)
{
    l2_packet_send(port->l2, agent->dst_mac_addr,
		htons(ETH_P_LLDP), agent->tx.frameout, agent->tx.sizeout);
}

void process_tx_info_frame(struct port *port, struct lldp_agent *agent)
{
    mibConstrInfoLLDPDU(port, agent);

    // txFrame(port, agent);
}

int run_tx_sm(struct port *port, struct lldp_agent *agent)
{
    process_tx_info_frame(port, agent);
}

void set_lldp_agent_admin(const char *ifname, int type, int admin)
{
    run_tx_sm(port, agent);
    
}
