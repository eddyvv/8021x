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

static int hex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

static int hex2byte(const char *hex)
{
	int a, b;
	a = hex2num(*hex++);
	if (a < 0)
		return -1;
	b = hex2num(*hex++);
	if (b < 0)
		return -1;
	return (a << 4) | b;
}

/**
 * hexstr2bin - Convert ASCII hex string into binary data
 * @hex: ASCII hex string (e.g., "01ab")
 * @buf: Buffer for the binary data
 * @len: Length of the text to convert in bytes (of buf); hex will be double
 * this size
 * Returns: 0 on success, -1 on failure (invalid hex string)
 */
int hexstr2bin(const char *hex, u8 *buf, size_t len)
{
	size_t i;
	int a;
	const char *ipos = hex;
	u8 *opos = buf;

	for (i = 0; i < len; i++) {
		a = hex2byte(ipos);
		if (a < 0)
			return -1;
		*opos++ = a;
		ipos += 2;
	}
	return 0;
}

#define BYTE2CHAR(b)	(((b) > 9) ? ((b) - 0xa + 'A') : ((b) + '0'))
int bin2hexstr(const u8 *hex, size_t hexlen, char *buf, size_t buflen)
{
	u8 b;
	size_t i, j;

	for (i = j = 0; (i < hexlen) && (j < buflen); i++, j +=2) {
		b = (hex[i] & 0xf0) >> 4;
		buf[j] = BYTE2CHAR(b);
		b = hex[i] & 0x0f;
		buf[j + 1] = BYTE2CHAR(b);
	}
	return 0;
}

int str_2_hex_str(char *dest, char *src)
{
    int len = strlen(src); // 获取接收数据长度
    int i,j;

    for (i = 0, j = 0; i < len; i++) {
    	// %02X: 是格式化字符串，意思是以大写的形式（通过X大小写控制）转换成16进制，长度不足2的自动补0
        sprintf(&dest[j], "%02X", src[i]);
        j+=2; // 每个16进制占2个长度
    }

    dest[j] = '\0'; // 添加字符串结束符

    return j; // 返回字符串长度
}