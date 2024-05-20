#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

#include "lldp-tlv.h"



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


void send_lldp()
{

}

