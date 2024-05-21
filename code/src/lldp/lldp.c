#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>


#include "lldp-tlv.h"
#include "agent.h"
#include "ports.h"

#define INTERFACE "ens33"
extern struct port *port;
extern struct lldp_agent *agent;

int l2_packet_send(struct l2_packet_data *l2, const u8 *dst_addr, u16 proto,
		   const u8 *buf, size_t len)
{
	int ret;
    struct sockaddr_ll ll;

	if (l2 == NULL)
		return -1;

	memset(&ll, 0, sizeof(ll));
	ll.sll_family = AF_PACKET;
	ll.sll_ifindex = l2->ifindex;
	ll.sll_protocol = htons(proto);
	ll.sll_halen = ETH_ALEN;

	memcpy(ll.sll_addr, dst_addr, ETH_ALEN);

	ret = sendto(l2->fd, buf, len, 0, (struct sockaddr *) &ll,
			     sizeof(ll));
	if (ret < 0)
			perror("l2_packet_send - sendto");
	
	return ret;
}

void show_lldp_buf(char *buf, int len)
{
	int i;

	printf("len = %d\n", len);

	for(i = 0; i < len; i++)
		printf("%.2x ", buf[i] & 0xff);

    printf("\n");
    printf("\n");
}

void send_lldp()
{
	int sockfd;
    struct ifreq ifr;
    struct sockaddr_ll saddr;
	char *dev_name = INTERFACE;

	oper_add_dev(dev_name);

	sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, INTERFACE, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1)
    {
        perror("ioctl SIOCGIFINDEX err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("nic name: %s\n", ifr.ifr_name);
	memset(&saddr, 0, sizeof(saddr));
    saddr.sll_family = AF_PACKET;
    saddr.sll_ifindex = ifr.ifr_ifindex;
    saddr.sll_halen = ETH_ALEN;
    memcpy(saddr.sll_addr, agent->dst_mac_addr, 6);
    get_mac(sockfd, &ifr);

    struct ether_header *eh = (struct ether_header *)(agent->tx.frameout);
    
    memcpy(eh->ether_shost, ifr.ifr_netmask.sa_data, 6);

	int numbytes = sendto(sockfd, agent->tx.frameout, agent->tx.sizeout, 0, (struct sockaddr *)&saddr, sizeof(saddr));
    if (numbytes == -1) 
    {
        perror("sendto err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);
	show_lldp_buf(agent->tx.frameout, agent->tx.sizeout);
}

