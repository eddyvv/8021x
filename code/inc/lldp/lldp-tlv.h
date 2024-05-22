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
	u16 reserved;
	u32 manufacturer;				/* 厂家信息 */
	u64 hard_num;					/* 硬件编号 */
	u32 hard_stand_version;			/* 硬件标准版本 */
	u32 hard_date_version;			/* 硬件日期版本 */
	u32 logic_stand_version;		/* 逻辑标准版本 */
	u32 logic_date_version;			/* 逻辑日期版本 */
	u32 firmware_stand_version;		/* 固件标准版本 */
	u32 firmware_date_version;		/* 固件日期版本 */
	u32 mcu_software_stand_version;	/* mcu软件标准版本 */
	u32 mcu_software_date_version;	/* mcu软件日期版本 */

	u32 reserved0_stand_version;
	u32 reserved0_date_version;
	u32 reserved1_stand_version;
	u32 reserved1_date_version;
	u32 reserved2_stand_version;
	u32 reserved2_date_version;
	u32 reserved3_stand_version;
	u32 reserved3_date_version;

	u16 reserved_1;

	u8  network_management_roles;	/* 网络管理角色 0：待定，1：主，其他：从*/
	u8  clock_synchronizationroles;	/* 时钟同步角色 0：client，1：master，default：0 */
	u64 static_calendar;			/* 静态日历 */
	u64 static_system_rtc_s;		/* 静态系统RTC秒 */
	u32 static_system_rtc_ns;		/* 静态系统RTC纳秒 */
	u8  reserved_2[28];
} __attribute__ ((__packed__));

struct lldp_iom_port {
	u8 port_speed;						/* 端口速率 */
	u8 port_status;						/* 端口状态 */
	u8 port_device_certification_status;	/* 端口设备认证状态 */
	u8 port_device_Internet_status;		/* 端口设备上网状态 */
	u8 port_device_id[4];				/* 端口设备ID1 */
} __attribute__ ((__packed__));

struct lldp_local_port_status {
	u8 organization_identity[3];	/* 组织标识 */
	u8 customize_type;				/* 自定义类型 */
	u8 reserved[2];

	struct lldp_iom_port iom_port[48];
} __attribute__ ((__packed__));

struct lldp_bit_status {
	u16 reserved;
	u8  organization_identity[3];	/* 组织标识 */
	u8  customize_type;				/* 自定义类型 */
	u8  data[16];
} __attribute__ ((__packed__));

struct lldp_recording_device_ip {
	u16 reserved;
	u8  organization_identity[3];	/* 组织标识 */
	u8  customize_type;				/* 自定义类型 */
	u32 mide_dev_ip;				/* MIDE设备IP */
	u32 tde_dev_ip;					/* TDE设备IP */
} __attribute__ ((__packed__));

struct lldp_ip_assignment {
	u16 reserved;
	u8  organization_identity[3];	/* 组织标识 */
	u8  customize_type;				/* 自定义类型 */
	u8  data[24];
	u8  reserved_1[3];
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