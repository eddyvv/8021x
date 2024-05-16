#include <8021x.h>
#include <stdio.h>
Ip_u8 ip8021x_pae_grp_addr[6] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x03 };


IP_GLOBAL Ip8021x_pkt* ip8021x_pkt_new(int len, int offset, Ip_u8 version, Ip_u8 type)
{
    Ip8021x_pkt* pkt;
    Ip8021x_eapol_key_msg* key_msg;

    pkt = malloc(sizeof(struct Ip8021x_pkt_st));
    if (!pkt)
        return IP_NULL;

    pkt->start = offset;
    pkt->end = offset;
    pkt->len = len;
    pkt->ref = 1;

    pkt->buf = malloc(len);
    if (!pkt->buf)
    {
        free(pkt);
        return IP_NULL;
    }
    memset(pkt->buf, 0 , pkt->len);

    key_msg = (Ip8021x_eapol_key_msg*)&pkt->buf[pkt->start+14];
    key_msg->version = version;
    key_msg->type = type;

    return pkt;
}

IP_GLOBAL Ip_bool
ip8021x_is_eth_addr_set(Ip_u8* eth_addr)
{
    int i;

    for(i=0; i<6; i++)
    {
        if (eth_addr[i] != 0)
            return IP_TRUE;
    }

    return IP_FALSE;
}

void show_eap_buf_x(Ip8021x_pkt* pkt, Ip8021x_eap_enum pkt_type)
{
    int i = 0;
    char *eap_str[] = EAP_ARRAY;

    printf("%s %d\n", eap_str[pkt_type], pkt_type);
    for(i = 0; i < 60; i++)
        printf("%.2x ", pkt->buf[i]);
    printf("\n");
    printf("\n");
}

IP_GLOBAL void ip8021x_pkt_free(Ip8021x_pkt* pkt)
{
    pkt->ref--;
    if (pkt->ref == 0)
    {
        free(pkt->buf);
        memset(pkt, 0, sizeof(Ip8021x_pkt));
        free(pkt);
    }
}

void show_eap_buf_h(Ip8021x_pkt* pkt, Ip8021x_eap_enum pkt_type)
{
    int i = 0;
    char *eap_str[] = EAP_ARRAY;
    Ip_u8 *ptr = (Ip_u8 *)&pkt->buf[pkt->start];

    printf("%s %d\n", eap_str[pkt_type], pkt_type);
    printf("Destination MAC address(%.2d): %.2x %.2x %.2x %.2x %.2x %.2x\n",6,ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
    printf("Source MAC address(%.2d):      %.2x %.2x %.2x %.2x %.2x %.2x\n",6,ptr[6], ptr[7], ptr[8], ptr[9], ptr[10], ptr[11]);
    printf("Type(%.2d):                    0x%.2x%.2x\n",2,ptr[12], ptr[13]);
    printf("Version(%.2d):                 0x%.2x\n",1,ptr[14]);
    printf("Type(%.2d):                    0x%.2x\n",1,ptr[15]);
    printf("Length(%.2d):                  0x%.2x%.2x\n",2,ptr[17], ptr[16]);

    if(pkt_type != EAPOL_START && pkt_type != EAPOL_LOGOFF)
    {
        printf("Code(%.2d):                    0x%.2x\n", 1, ptr[18]);
        printf("Id(%.2d):                      0x%.2x\n", 1, ptr[19]);
        printf("Length(%.2d):                  0x%.2x%.2x\n", 2, ptr[21], ptr[20]);
    }

    if(pkt_type == EAP_REQUEST || pkt_type== EAPOL_RESPONSE)
        printf("Type(%.2d):                    0x%.2x\n", 1, ptr[22]);

    switch(pkt_type)
    {
        case EAPOL_START:
            printf("Padding(%.2d):                 ",42);
            for(i = 0; i < 42; i++)
                printf("%.2x ",ptr[18+i]);
            printf("\n");
        break;

        case EAP_REQUEST:
            printf("Type-Data(%.2d):               ", 4);
            for(i = 0; i < 4; i++)
                printf("%.2x ", ptr[23+i]);
            printf("\n");
            printf("Pad(%.2d):                     ", 33);
            for(i = 0; i < 33; i++)
                printf("%.2x ", ptr[27+i]);
            printf("\n");
        break;

        case EAPOL_RESPONSE:
            printf("Type-Data(%.2d):               ", 16);
            for(i = 0; i < 16; i++)
                printf("%.2x ", ptr[23+i]);
                        printf("\n");
            printf("Pad(%.2d):                     ", 21);
            for(i = 0; i < 21; i++)
                printf("%.2x ", ptr[27+i]);
            printf("\n");
        break;

        case EAPOL_SUCCESS:
        case EAP_FAILURE:
            printf("Pad(%.2d):                     ", 38);
            for(i = 0; i < 38; i++)
                printf("%.2x ", ptr[22+i]);
            printf("\n");
        break;

        case EAPOL_LOGOFF:
            printf("Pad(%.2d):                     ", 42);
            for(i = 0; i < 42; i++)
                printf("%.2x ", ptr[18+i]);
            printf("\n");
        break;

        default:

        break;
    }
    printf("CRC(%.2d):                     %.2x %.2x %.2x %.2x\n", 4, ptr[60],ptr[61],ptr[62],ptr[63]);
    printf("\n");
}

IP_GLOBAL Ip_err ip8021x_send_eapol_msg(Ip8021x_ctx* ctx, Ip8021x_pkt* pkt, Ip_u8 version, Ip_u8 pkt_type)
{
    Ip_u8* ptr;
    Ip_err rc = 0;

    ptr = &pkt->buf[pkt->start];

    /* Ethernet header */
    if (ctx->mode ==  IP8021X_EAPOL_MODE_WIRED)
    {
        // printf("eeeeeee\n");
        memcpy(&pkt->buf[pkt->start], ip8021x_pae_grp_addr, 6);
    }
    else
    {	
        if (ip8021x_is_eth_addr_set(ctx->aa))
            memcpy(&pkt->buf[pkt->start], ctx->aa, 6);
        else
            memcpy(&pkt->buf[pkt->start], ip8021x_pae_grp_addr, 6);
    }
    // printf("qqqq\n");
    memcpy(&ptr[6], ctx->spa, 6);
    IP_SET_HTONS(&ptr[12], IP8021X_ETH_P_EAPOL);

    /* eapol header */
    ptr[14] = version;
    ptr[15] = pkt_type;
    // ptr[16] = 4; /* Length */
    IP_SET_HTONS(&ptr[16], pkt->end-pkt->start-14); /* Length */
    // ptr[16] = pkt->end - pkt->start-14;

    ctx->pkt = pkt;

    return rc;
}

Ip_err xt_8021x_send_eapol_start(Ip8021x_ctx* ctx)
{
    Ip8021x_pkt* pkt;
    int len = 64;

    pkt = ip8021x_pkt_new(len, 0, 2, IP8021X_EAPOL_TYPE_START);
    if (!pkt)
        return IPCOM_ERR_FAILED;
    pkt->end += 14;
    pkt->len = len;

    return ip8021x_send_eapol_msg(ctx, pkt, 2, IP8021X_EAPOL_TYPE_START);
}


IP_GLOBAL void ip8021x_pkt_map_to_eap(Ip8021x_pkt* pkt, Ipeap_pkt* eap_pkt)
{
    eap_pkt->data = pkt->buf;
    eap_pkt->start = pkt->start+14; /* leave room for/proceed past 14 byte eth and 4 byte eapol header */
    eap_pkt->end = pkt->end;
    eap_pkt->max_len = pkt->len;
}

int ip8021x_send_identity_request(struct Ip8021x_ctx_st *ctx)
{
    Ip_err rc;
    Ipeap_pkt out_eap_pkt;
    Ipeap_pkt_hdr* eap_hdr;
    Ip8021x_pkt* out_pkt;

    out_pkt = ip8021x_pkt_new(64, 0, 2, IP8021X_EAPOL_TYPE_PACKET);
    if (!out_pkt)
        return IPCOM_ERR_NO_MEMORY;
    out_pkt->end = 14;

    ip8021x_pkt_map_to_eap(out_pkt, &out_eap_pkt);

    eap_hdr = (Ipeap_pkt_hdr*)&out_eap_pkt.data[out_eap_pkt.start];
    eap_hdr->code = IPEAP_PKT_REQUEST;
    // eap_hdr->id = 1;
    eap_hdr->len_0 = 13;
    // eap_hdr->len_1 = 8;
    IP_SET_HTONS(&eap_hdr->len_1, 8);
    eap_hdr->type = IPEAP_TYPE_IDENTITY;
    
    out_eap_pkt.end += 13;
    out_pkt->end += out_eap_pkt.end-out_eap_pkt.start;
    
    rc = ip8021x_send_eapol_msg(ctx, out_pkt, 2, IP8021X_EAPOL_TYPE_PACKET);
	if (rc != IPCOM_SUCCESS)
    {
		printf("ip8021x_send_identity_request :: ip8021x_send_eapol_msg() failed, rc = %d", rc);
	}

    return 0;
}

IP_GLOBAL Ip_err ip8021x_eapol_msg_input(Ip8021x_ctx* ctx, Ip8021x_pkt* in_pkt)
{
    Ip_err rc = 0;
    Ipeap_pkt in_eap_pkt;
    Ipeap_pkt out_eap_pkt;
    Ipeap_pkt_hdr* eap_hdr;
    Ip8021x_pkt* out_pkt;

    out_pkt = ip8021x_pkt_new(64, 0, 2, IP8021X_EAPOL_TYPE_PACKET);
    if (!out_pkt)
        return IPCOM_ERR_NO_MEMORY;
    out_pkt->end = 14;

    ip8021x_pkt_map_to_eap(out_pkt, &out_eap_pkt);
    ip8021x_pkt_map_to_eap(in_pkt, &in_eap_pkt);

    eap_hdr = (Ipeap_pkt_hdr*)&in_eap_pkt.data[in_eap_pkt.start];

    if (eap_hdr->code == IPEAP_PKT_REQUEST)
    {

    }

    ctx->pkt = out_pkt;

    return rc;
}

IP_STATIC Ip_err ip8021x_send_eap_failure(struct Ip8021x_ctx_st *ctx, Ip_u8 id)
{
    Ip_err rc;
    Ipeap_pkt out_eap_pkt;
    Ipeap_pkt_hdr* eap_hdr;
    Ip8021x_pkt* out_pkt;

    out_pkt = ip8021x_pkt_new(64, 0, 1, IP8021X_EAPOL_TYPE_PACKET);
    if (!out_pkt)
        return IPCOM_ERR_NO_MEMORY;
    out_pkt->end = 14;

    ip8021x_pkt_map_to_eap(out_pkt, &out_eap_pkt);
    eap_hdr = (Ipeap_pkt_hdr*)&out_eap_pkt.data[out_eap_pkt.start];
    eap_hdr->code = IPEAP_PKT_FAILURE;
    eap_hdr->id = id;
    eap_hdr->len_0 = 8;
    IP_SET_HTONS(&eap_hdr->len_1,IPEAP_PKT_HDR_LEN-1);
    eap_hdr->type = IPEAP_TYPE_UNDEFINED;
    out_eap_pkt.end += 8;
    out_pkt->end += out_eap_pkt.end-out_eap_pkt.start;

    rc = ip8021x_send_eapol_msg(ctx, out_pkt, 2, IP8021X_EAPOL_TYPE_PACKET);
    return rc;
}

IP_PUBLIC Ipeap_pkt* ipeap_get_last_sent_pkt(struct Ipeap_ctx_st* ctx)
{
    return &ctx->last_sent_pkt;
}

IP_STATIC Ip_err ip8021x_send_last_eap_pkt(struct Ip8021x_ctx_st *ctx)
{
    Ip_err err;
    // Ipeap_pkt *last_sent_eap_pkt;
    Ip8021x_pkt *out_pkt;

    out_pkt = ip8021x_pkt_new(64, 0, 1, IP8021X_EAPOL_TYPE_PACKET);
    if (!out_pkt) {
        printf("ip8021x_pkt_new Err");
        return IPCOM_ERR_FAILED;
    }
    out_pkt->end = 14;

    // if((last_sent_eap_pkt = ipeap_get_last_sent_pkt(ctx->eap_ctx)) == IP_NULL) {
    //     printf("ERR:ip8021x_auth_req_tmo_cb :: Could not get last sent eap"" packet");
    //     ip8021x_pkt_free(out_pkt);
    //     return IPCOM_ERR_FAILED;
    // }

    // memcpy(&(out_pkt->buf[out_pkt->start+14]),
    //         &(last_sent_eap_pkt->data[last_sent_eap_pkt->start]),
    //         last_sent_eap_pkt->end-last_sent_eap_pkt->start);

    err = ip8021x_send_eapol_msg(ctx, out_pkt, 2, IP8021X_EAPOL_TYPE_PACKET);
    if (err != IPCOM_SUCCESS)
    {
        printf("ERR:ip8021x_send_last_eap_pkt ::"
                " ip8021x_send_eapol_msg failed");
        return IPCOM_ERR_FAILED;
    }

    return err;
}

IP_GLOBAL Ip_s32 ip8021x_auth_req_tmo_cb(struct Ip8021x_ctx_st *ctx)
{
    // Ipeap_pkt *last_sent_pkt;
    /* 什么时间发需修改 */
    // if(ctx->req_retry_count > IP8021X_EAP_REQ_MAX_RETRIES)
    {
        // Ipeap_pkt_hdr* last_sent_hdr;
        Ip_u8 last_sent_id = 1;

        // last_sent_pkt = ipeap_get_last_sent_pkt(ctx->eap_ctx);
        // if(last_sent_pkt == IP_NULL)
        // {
        //     printf("ERR:ip8021x_auth_req_tmo_cb :: last_sent_pkt is NULL");
        //     return -1;
        // }
        // last_sent_hdr =
        //     (Ipeap_pkt_hdr*)&last_sent_pkt->data[last_sent_pkt->start];
        // last_sent_id = last_sent_hdr->id++;

        ip8021x_send_eap_failure(ctx,last_sent_id);

        ctx->req_retry_count = 0;
    }

    ctx->req_retry_count++;

    // ip8021x_send_last_eap_pkt(ctx);
    return 1;
}

IP_GLOBAL Ip_err ip8021x_tx_eapol_logoff(Ip8021x_ctx* ctx)
{
    Ip8021x_pkt* pkt;
    Ipeap_pkt out_eap_pkt;
    Ipeap_pkt_hdr* eap_hdr;

    pkt = ip8021x_pkt_new(64, 0, 2, IP8021X_EAPOL_TYPE_LOGOFF);
    if (!pkt)
        return IPCOM_ERR_FAILED;
    pkt->end += 14;

    ip8021x_pkt_map_to_eap(pkt, &out_eap_pkt);

    eap_hdr = (Ipeap_pkt_hdr*)&out_eap_pkt.data[out_eap_pkt.start];
    eap_hdr->len_0 = 8;
    eap_hdr->type = IPEAP_TYPE_UNDEFINED;
    // eap_hdr->frame_type = 02;
    out_eap_pkt.end += 4;
    pkt->end += out_eap_pkt.end-out_eap_pkt.start;

    return ip8021x_send_eapol_msg(ctx, pkt, 2, IP8021X_EAPOL_TYPE_LOGOFF);
}

IP_GLOBAL Ip_err ip8021x_tx_eapol_success(Ip8021x_ctx* ctx)
{
    Ip_err rc;
    Ip8021x_pkt* out_pkt;
    Ipeap_pkt out_eap_pkt;
    Ipeap_pkt_hdr* eap_hdr;

    out_pkt = ip8021x_pkt_new(64, 0, 2, IP8021X_EAPOL_TYPE_LOGOFF);
    if (!out_pkt)
        return IPCOM_ERR_FAILED;
    out_pkt->end += 14;

    ip8021x_pkt_map_to_eap(out_pkt, &out_eap_pkt);
    eap_hdr = (Ipeap_pkt_hdr*)&out_eap_pkt.data[out_eap_pkt.start];
    eap_hdr->code = IPEAP_PKT_SUCCESS;
    eap_hdr->id = 1;
    eap_hdr->len_0 = 8;
    IP_SET_HTONS(&eap_hdr->len_1,IPEAP_PKT_HDR_LEN-1);
    eap_hdr->type = IPEAP_TYPE_UNDEFINED;
    out_eap_pkt.end += 8;
    out_pkt->end += out_eap_pkt.end-out_eap_pkt.start;

    rc = ip8021x_send_eapol_msg(ctx, out_pkt, 2, IP8021X_EAPOL_TYPE_PACKET);
    return rc;
}