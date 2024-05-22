#ifndef LLDP_TLV_H
#define LLDP_TLV_H

#include "types.h"
#include "lldp.h"


struct packed_tlv {
	u16 size;     /* Of the entire tlv block */
	u8 *tlv;      /* tlv block */
};

struct unpacked_tlv {
	u8  type;
	u16 length;
	u8  *info;
};

struct lldp_dev_id {
	u8 sub_type;
	u8 data;
} __attribute__ ((__packed__));

struct lldp_port_id {
	u8 sub_type;
	u8 data;
} __attribute__ ((__packed__));

struct lldp_time_to_live {
	u8 data[2];
} __attribute__ ((__packed__));

struct lldp_system_name {
	u8 data[10];
} __attribute__ ((__packed__));

struct lldp_system_description {
	u8 reserved[2];
	u8 manufacturer[4];				/* 厂家信息 */
	u8 hard_num[8];					/* 硬件编号 */
	u8 hard_stand_version[4];		/* 硬件标准版本 */
	u8 hard_date_version[4];		/* 硬件日期版本 */
	u8 logic_stand_version[4];		/* 逻辑标准版本 */
	u8 logic_date_version[4];		/* 逻辑日期版本 */
	u8 firmware_stand_version[4];	/* 固件标准版本 */
	u8 firmware_date_version[4];	/* 固件日期版本 */
	u8 mcu_software_stand_version[4];	/* mcu软件标准版本 */
	u8 mcu_software_date_version[4];	/* mcu软件日期版本 */

	u8 reserved0_stand_version[4];
	u8 reserved0_date_version[4];
	u8 reserved1_stand_version[4];
	u8 reserved1_date_version[4];
	u8 reserved2_stand_version[4];
	u8 reserved2_date_version[4];
	u8 reserved3_stand_version[4];
	u8 reserved3_date_version[4];

	u8 reserved_1[2];

	u8 network_management_roles;	/* 网络管理角色 0：待定，1：主，其他：从*/
	u8 clock_synchronizationroles;	/* 时钟同步角色 0：client，1：master，default：0 */
	u8 static_calendar[8];			/* 静态日历 */
	u8 static_system_rtc_s[8];		/* 静态系统RTC秒 */
	u8 static_system_rtc_ns[4];		/* 静态系统RTC纳秒 */
	u8 reserved_2[28];
} __attribute__ ((__packed__));



struct unpacked_tlv *free_unpkd_tlv(struct unpacked_tlv *tlv);
struct packed_tlv *free_pkd_tlv(struct packed_tlv *tlv);
struct unpacked_tlv *create_tlv();
struct packed_tlv *create_ptlv(void);
struct packed_tlv *pack_tlv(struct unpacked_tlv *tlv);
struct unpacked_tlv *bld_end_tlv();

int tlv_ok(struct unpacked_tlv *tlv);
struct packed_tlv *pack_end_tlv();
#endif