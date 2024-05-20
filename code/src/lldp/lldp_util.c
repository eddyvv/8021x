#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if_arp.h>
// #include <netlink/msg.h>
#include <arpa/inet.h>
#include <linux/wireless.h>
#include <linux/sockios.h>
#include <dirent.h>

#include "types.h"

/* MAC_ADDR_STRLEN = strlen("00:11:22:33:44:55") */
#define MAC_ADDR_STRLEN	17
int mac2str(const u8 *mac, char *dst, size_t size)
{
	if (dst && size > MAC_ADDR_STRLEN) {
		snprintf(dst, size, "%02X:%02X:%02X:%02X:%02X:%02X",
			 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		return 0;
	}
	return -1;
}