#ifndef LLDP_UTIL_H
#define LLDP_UTIL_H

#include "types.h"

#define INT64_TO_NET(val)	((u64) ( \
      (((u64) (val) &						\
	(u64) (0x00000000000000ffU)) << 56) |	\
      (((u64) (val) &						\
	(u64) (0x000000000000ff00U)) << 40) |	\
      (((u64) (val) &						\
	(u64) (0x0000000000ff0000U)) << 24) |	\
      (((u64) (val) &						\
	(u64) (0x00000000ff000000U)) <<  8) |	\
      (((u64) (val) &						\
	(u64) (0x000000ff00000000U)) >>  8) |	\
      (((u64) (val) &						\
	(u64) (0x0000ff0000000000U)) >> 24) |	\
      (((u64) (val) &						\
	(u64) (0x00ff000000000000U)) >> 40) |	\
      (((u64) (val) &						\
	(u64) (0xff00000000000000U)) >> 56)))

int mac2str(const u8 *mac, char *dst, size_t size);
int bin2hexstr(const u8 *hex, size_t hexlen, char *buf, size_t buflen);
int str_2_hex_str(char *dest, char *src);

#endif
