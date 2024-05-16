#ifndef IPEAP_H
#define IPEAP_H



#define IPEAP_PKT_REQUEST  1
#define IPEAP_PKT_RESPONSE 2
#define IPEAP_PKT_SUCCESS  3
#define IPEAP_PKT_FAILURE  4


/* Use these instead of sizeof(Ipeap_pkt_hdr) and sizeof(Ipeap_expanded_pkt_hdr) */
#define IPEAP_PKT_HDR_LEN 		5
#define IPEAP_EXP_PKT_HDR_LEN 	12
/* Use these instead of sizeof(Ipeap_vendor_id) and sizeof(Ipeap_vendor_type) */
#define IPEAP_VENDOR_ID_LEN		3	
#define IPEAP_VENDOR_TYPE_LEN	4	

    /* EAP mechanism IDs as defined in RFCs */
#define IPEAP_TYPE_UNDEFINED      0
#define IPEAP_TYPE_IDENTITY       1
#define IPEAP_TYPE_NOTIFICATION   2
#define IPEAP_TYPE_NAK            3
#define IPEAP_TYPE_MD5_CHALLENGE  4
#define IPEAP_TYPE_GTC            6
#define IPEAP_TYPE_TLS           13
#define IPEAP_TYPE_LEAP          17
#define IPEAP_TYPE_SIM           18
#define IPEAP_TYPE_TTLS          21
#define IPEAP_TYPE_AKA           23
#define IPEAP_TYPE_PEAP          25
#define IPEAP_TYPE_MSCHAPV2      26
#define IPEAP_TYPE_EXTENSIONS    33
#define IPEAP_TYPE_EXPANDED      254

struct Ipeap_vendor_id_st {
    Ip_u8 smi[3];
};
typedef struct Ipeap_vendor_id_st Ipeap_vendor_id;
typedef Ip_u32 Ipeap_vendor_type;

// typedef struct Ipeap_methods_st
//     {
//         Ipeap_process_pkt  process_pkt;
//         Ipeap_init         init;
//         Ipeap_free         free;
//         Ipeap_get_key      get_key;
//         Ipeap_handle_user_input handle_user_input;
//         Ipeap_display_status    display_status;
//         Ipeap_get_eap_session_id get_eap_session_id;
//     } Ipeap_methods;

typedef struct Ipeap_pkt_hdr_st
{
    Ip_u8 code;
    Ip_u8 id;
    Ip_u16 len_0;
    Ip_u16 len_1;
    Ip_u8 type; /* Only present in requests and responses */
}
Ipeap_pkt_hdr;

struct Ipeap_pkt_st
{
    int start;
    int end;
    int max_len;
    Ip_u8* data;
};
typedef struct Ipeap_pkt_st Ipeap_pkt;

#define IPEAP_IDENTITY_MAX_LEN 128


struct Ipeap_ctx_st
{
    int mode;
    int state;
    char identity[IPEAP_IDENTITY_MAX_LEN];
    char syslog_prefix[16];
    Ip_u8 meth_id;
    Ipeap_vendor_id vendor_id; /* for expanded type EAP methods */
    Ipeap_vendor_type vendor_type; /* for expanded type EAP methods */
    // Ipeap_methods* cb;
    struct Ipeap_mech_entry_st *mech_map;
    Ip_bool method_locked; /* No other method other than already selected one can be negotiated */

    /* Resend data */
    Ip_u8  last_recv_pkt_hdr[5]; /* This is the header from the last packet we received from the peer */
    Ipeap_pkt last_sent_pkt;  /* This is the last packet we sent to the peer */

    void* mech_data;
};
typedef struct Ipeap_ctx_st Ipeap_ctx;

#endif