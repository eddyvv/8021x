#ifndef IP8021X_H
#define IP8021X_H

#include <string.h>
#include <stdlib.h>
#include "ipcom_type.h"
#include "ip8021x_h.h"
#include "ipcom_err.h"
#include "ipcom_cstyle.h"
#include "ipeap.h"



#define IP_GLOBAL
#define IP8021X_EAPOL_MODE_WIRED     1





#define IP_SET_HTONS(xxaddr, yyval) do { \
   ((Ip_u8 *)(xxaddr))[0] = (Ip_u8)(((yyval) >>  8) & 0xff); \
   ((Ip_u8 *)(xxaddr))[1] = (Ip_u8)((yyval) & 0xff); \
} while((0))
#define IP_SET_NTOHS(xxaddr, yyval) IP_SET_HTONS(xxaddr,yyval)

#define IP_GET_HTONS(xxaddr) \
  ((Ip_u16)(((Ip_u16)((Ip_u8 *)(xxaddr))[0]) << 8 | \
            ((Ip_u16)((Ip_u8 *)(xxaddr))[1])))
#define IP_GET_NTOHS(xxaddr)   IP_GET_HTONS(xxaddr)

#define IP8021X_EAP_REQ_MAX_RETRIES 5
/*
 *===========================================================================
 *                         Ip8021x_pkt
 *===========================================================================
 * This type represents an 802.1x packet
 */
typedef struct Ip8021x_pkt_st
{
    int start; /* Index to first used byte */
    int end;   /* Index to first free byte */
    int len;   /* Length of allocated buffer */
    int ref;   /* No of references to this packet */
    Ip_u8* buf;
} Ip8021x_pkt;



typedef struct Ip8021x_eapol_key_msg_st
{
    /* Generic EAPOL field */
    Ip_u8 version;
    Ip_u8 type;
    Ip_u16 len;

    /* EAPOL key fields */
    Ip_u8   descriptor_type;
    Ip_u8   key_info[2];
    Ip_u8   key_len[2];
    Ip_u8   replay_counter[8];
    Ip_u8   key_nonce[32];
    Ip_u8   key_iv[16];
    Ip_u8   key_rsc[8];
    Ip_u8   reserved[8];
    Ip_u8   key_mic[16];
    Ip_u8   key_data_len[2];
    Ip_u8   key_data[1];
}  Ip8021x_eapol_key_msg;

typedef struct Ip8021x_ctx_st
{
    Ip_u32 mode;

    Ip_u8 aa[6];    /* Authenticator MAC address */
    Ip_u8 spa[6];   /* Supplicant MAC address */

    Ip8021x_pkt* pkt;
    struct Ipeap_ctx_st *eap_ctx;
    Ipeap_pkt last_sent_pkt;  /* This is the last packet we sent to the peer */
    Ip_u8 req_retry_count;
} Ip8021x_ctx;


typedef enum Ip8021x_eap_en
{
    EAPOL_START,        /* 认证触发 */
    EAP_REQUEST,        /* 认证请求 */
    EAPOL_RESPONSE,     /* 认证响应 */
    EAPOL_SUCCESS,      /* 认证成功 */
    EAP_FAILURE,        /* 认证失败 */
    EAPOL_LOGOFF,       /* 认证下线请求 */
} Ip8021x_eap_enum;

#define EAP_ARRAY   {"EAPOL_Start", "EAP_Request", "EAPOL_Response", "EAPOL_Success", "EAP_Failure", "EAPOL_Logoff"}


void show_eap_buf_x(Ip8021x_pkt* pkt, Ip8021x_eap_enum pkt_type);
void show_eap_buf_h(Ip8021x_pkt* pkt, Ip8021x_eap_enum pkt_type);
IP_GLOBAL Ip8021x_pkt* ip8021x_pkt_new(int len, int offset, Ip_u8 version, Ip_u8 type);
IP_GLOBAL Ip_err ip8021x_send_eapol_msg(Ip8021x_ctx* ctx, Ip8021x_pkt* pkt, Ip_u8 version, Ip_u8 pkt_type);
Ip_err xt_8021x_send_eapol_start(Ip8021x_ctx* ctx);
int ip8021x_send_identity_request(struct Ip8021x_ctx_st *ctx);
IP_GLOBAL Ip_err ip8021x_eapol_msg_input(Ip8021x_ctx* ctx, Ip8021x_pkt* in_pkt);
IP_GLOBAL Ip_s32 ip8021x_auth_req_tmo_cb(struct Ip8021x_ctx_st *ctx);
IP_GLOBAL Ip_err ip8021x_tx_eapol_logoff(Ip8021x_ctx* ctx);
IP_GLOBAL Ip_err ip8021x_tx_eapol_success(Ip8021x_ctx* ctx);

#endif