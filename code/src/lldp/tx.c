#include <stdlib.h>
#include <stdio.h>
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
            printf("CHASSIS_ID_TLV len :%d\n", ptlv->length);
        }
        break;

        case PORT_ID_TLV:
        {
            ptlv->type = PORT_ID_TLV;
            ptlv->length = sizeof(struct lldp_port_id);
            printf("PORT_ID_TLV len :%d\n", ptlv->length);
        }
        break;

        case TIME_TO_LIVE_TLV:
        {
            // ptlv->type = TIME_TO_LIVE_TLV;
            ptlv->type = 6;
            ptlv->length = sizeof(struct lldp_time_to_live);
            printf("TIME_TO_LIVE_TLV len :%d\n", ptlv->length);
        }
        break;

        case SYSTEM_NAME_TLV:
        {
            // ptlv->type = SYSTEM_NAME_TLV;
            ptlv->type = 10;
            ptlv->length = sizeof(struct lldp_system_name);
            printf("SYSTEM_NAME_TLV len :%d\n", ptlv->length);
        }
        break;

        case SYSTEM_DESCRIPTION_TLV:
        {
            // ptlv->type = SYSTEM_DESCRIPTION_TLV;
            ptlv->type = 0x0C;
            ptlv->length = sizeof(struct lldp_system_description);
            printf("SYSTEM_DESCRIPTION_TLV len :%d\n", ptlv->length);
        }
        break;

        case 0xff:
        {
            ptlv->type = 0xFF;
            ptlv->length = sizeof(struct lldp_local_port_status);
            printf("0xff len :%d\n", ptlv->length);
        }
        break;

        case 0xfe:
        {
            ptlv->type = 0xFE;
            ptlv->length = sizeof(struct lldp_bit_status);
            printf("0xff len :%d\n", ptlv->length);
        }
        break;

        default:
            ptlv->type = 4;
            ptlv->length = 2;
            printf("ERR:tlv_type err\r\n");
        break;
    }
}

void config_sys_description_tlv(struct packed_tlv *ptlv)
{
    struct packed_tlv *tlv = ptlv;
    struct lldp_system_description *sdt = (struct lldp_system_description *)(tlv->tlv + 2);

    sdt->manufacturer = htonl(0x631);
    sdt->hard_num = INT64_TO_NET(0x0230908001);
    sdt->hard_stand_version = 0;
    sdt->hard_date_version = 0;
    sdt->logic_stand_version = htonl(0x02010000);
    sdt->logic_date_version = htonl(0x24010501);
    sdt->firmware_stand_version = htonl(0x02010000);
    sdt->firmware_date_version = htonl(0x24010501);
    sdt->mcu_software_stand_version = htonl(0x02010000);
    sdt->mcu_software_date_version = htonl(0x24010501);
    sdt->network_management_roles = 1;
    sdt->clock_synchronizationroles = 1;
    sdt->static_calendar = 0;
    sdt->static_system_rtc_s = 0;
    sdt->static_system_rtc_ns = 0;
}

void config_local_iom_port_tlv(struct packed_tlv *ptlv)
{
    int i = 0, j = 0;
    struct packed_tlv *tlv = ptlv;
    struct lldp_local_port_status *sdt = (struct lldp_local_port_status *)(tlv->tlv + 2);
    
    *(u16 *)sdt->organization_identity = 0x631;
    sdt->customize_type = 0x10;
    for(i = 0; i < 48; i++)
    {
        sdt->iom_port[i].port_speed = 0;
        sdt->iom_port[i].port_status = 0;
        sdt->iom_port[i].port_device_certification_status = 0;
        sdt->iom_port[i].port_device_Internet_status = 0;
        for(j = 0; j < 4; j++)
            sdt->iom_port[i].port_device_id[j] = 0;
    }
}

void config_bit_status_tlv(struct packed_tlv *ptlv)
{
    int i = 0;
    struct packed_tlv *tlv = ptlv;
    struct lldp_bit_status *sdt = (struct lldp_bit_status *)(tlv->tlv + 2);
    
    *(u16 *)sdt->organization_identity = 0x631;
    sdt->customize_type = 0x12;
    for(i = 0; i < 16; i++)
        sdt->data[i] = 0;
}

int config_tlv_buf(struct port *port, struct lldp_agent *agent, int tlv_type)
{
    struct unpacked_tlv *ptlv =  NULL;
    struct packed_tlv *tlv;
    char *str = 'IOM1';

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
                config_tlv(ptlv, SYSTEM_NAME_TLV);
                tlv = pack_tlv(ptlv);
                *(u64 *)&tlv->tlv[2] = str;
                *(u32 *)&tlv->tlv[2] = htonl(*(u32 *)&tlv->tlv[2]);
                memcpy(agent->tx.frameout + agent->tx.sizeout, tlv->tlv, tlv->size);
            }
        }
        break;

        case SYSTEM_DESCRIPTION_TLV:
        {
            ptlv = test_gettlv(port, 130);
            if (ptlv)
            {
                config_tlv(ptlv, SYSTEM_DESCRIPTION_TLV);
                tlv = pack_tlv(ptlv);
                config_sys_description_tlv(tlv);
                memcpy(agent->tx.frameout + agent->tx.sizeout, tlv->tlv, tlv->size);
            }
        }
        break;

        case 0xff:
        {
            ptlv = test_gettlv(port, 390);
            if (ptlv)
            {
                config_tlv(ptlv, 0xff);
                tlv = pack_tlv(ptlv);
                config_local_iom_port_tlv(tlv);
                memcpy(agent->tx.frameout + agent->tx.sizeout, tlv->tlv, tlv->size);
            }
        }
        break;

        case 0xfe:
        {
            ptlv = test_gettlv(port, 22);
            if (ptlv)
            {
                config_tlv(ptlv, 0xfe);
                tlv = pack_tlv(ptlv);
                config_bit_status_tlv(tlv);
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
    struct packed_tlv *tlv =  NULL;
    
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

    fb_offset += config_tlv_buf(port, agent, SYSTEM_DESCRIPTION_TLV);
    agent->tx.sizeout = fb_offset;

    /* 本地端口状态（本地IOM端口状态） */
    fb_offset += config_tlv_buf(port, agent, 0xff);
    agent->tx.sizeout = fb_offset;

    /* 远端口状态（本IOM级联的IOM1的端口状态） */
    fb_offset += config_tlv_buf(port, agent, 0xff);
    agent->tx.sizeout = fb_offset;

    /* 远端口状态（本IOM级联的IOM2的端口状态） */
    fb_offset += config_tlv_buf(port, agent, 0xff);
    agent->tx.sizeout = fb_offset;

    fb_offset += config_tlv_buf(port, agent, 0xfe);
    agent->tx.sizeout = fb_offset;


    tlv = pack_end_tlv();
    memcpy(agent->tx.frameout + fb_offset, tlv->tlv, tlv->size);
    fb_offset += tlv->size;
    agent->tx.sizeout = fb_offset;
    free_pkd_tlv(tlv);

    return true;
}


/* 发送 */
// u8 txFrame(struct port *port, struct lldp_agent *agent)
// {
//     l2_packet_send(port->l2, agent->dst_mac_addr,
// 		htons(ETH_P_LLDP), agent->tx.frameout, agent->tx.sizeout);
// }

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
