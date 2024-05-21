#ifndef LLDP_UTIL_H
#define LLDP_UTIL_H


#include "types.h"

int mac2str(const u8 *mac, char *dst, size_t size);
int bin2hexstr(const u8 *hex, size_t hexlen, char *buf, size_t buflen);
int str_2_hex_str(char *dest, char *src);

#endif
