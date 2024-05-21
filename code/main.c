#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <linux/if.h>
#include <sys/ioctl.h>

#include "8021x.h"


#define INTERFACE "ens33"
extern Ip_u8 ip8021x_pae_grp_addr[];

void get_mac(int sockfd, struct ifreq *ifr)
{
    char mac[18];
    if(ioctl(sockfd, SIOCGIFHWADDR, ifr) == -1)
    {
        perror("ioctl SIOCGIFHWADDR err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
    (unsigned char)ifr->ifr_netmask.sa_data[0],
    (unsigned char)ifr->ifr_netmask.sa_data[1],
    (unsigned char)ifr->ifr_netmask.sa_data[2],
    (unsigned char)ifr->ifr_netmask.sa_data[3],
    (unsigned char)ifr->ifr_netmask.sa_data[4],
    (unsigned char)ifr->ifr_netmask.sa_data[5]);
    printf("Mac address is %s\n", mac);
}


void ip8021x_send_start()
{
    int i = 0;
    Ip8021x_ctx* ctx;
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_ll saddr;

    ctx = malloc(sizeof(struct Ip8021x_ctx_st));

    for(i = 0; i < 6; i++)
        ctx->aa[i] = i;

    ctx->mode = IP8021X_EAPOL_MODE_WIRED;

    for(i = 0; i < 6; i++)
        ctx->spa[i] = i+1;

    xt_8021x_send_eapol_start(ctx);

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, INTERFACE, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1)
    {
        perror("ioctl SIOCGIFINDEX err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("nic name: %s\n", ifr.ifr_name);
    struct ether_header *eh = (struct ether_header *)(ctx->pkt->buf);

    memset(&saddr, 0, sizeof(saddr));
    saddr.sll_family = AF_PACKET;
    saddr.sll_ifindex = ifr.ifr_ifindex;
    saddr.sll_halen = ETH_ALEN;
    memcpy(saddr.sll_addr, ip8021x_pae_grp_addr, 6);

    get_mac(sockfd, &ifr);
    memcpy(eh->ether_shost, ifr.ifr_netmask.sa_data, 6);
    memcpy(eh->ether_dhost, ip8021x_pae_grp_addr, 6);

    int numbytes = sendto(sockfd, ctx->pkt->buf, ctx->pkt->len, 0, (struct sockaddr *)&saddr, sizeof(saddr));
    if (numbytes == -1) {
        perror("sendto err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);
    show_eap_buf_h(ctx->pkt, EAPOL_START);
}

void ip8021x_send_request()
{
    int i = 0;
    Ip8021x_ctx* ctx;
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_ll saddr;
    
    ctx = malloc(sizeof(struct Ip8021x_ctx_st));

    for(i = 0; i < 6; i++)
        ctx->aa[i] = i;

    ctx->mode = IP8021X_EAPOL_MODE_WIRED;

    for(i = 0; i < 6; i++)
        ctx->spa[i] = i+1;

    ip8021x_send_identity_request(ctx);

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, INTERFACE, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1)
    {
        perror("ioctl SIOCGIFINDEX err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("nic name: %s\n", ifr.ifr_name);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sll_family = AF_PACKET;
    saddr.sll_ifindex = ifr.ifr_ifindex;
    saddr.sll_halen = ETH_ALEN;
    memcpy(saddr.sll_addr, ip8021x_pae_grp_addr, 6);
    
    get_mac(sockfd, &ifr);
    struct ether_header *eh = (struct ether_header *)(ctx->pkt->buf);
    
    memcpy(eh->ether_shost, ifr.ifr_netmask.sa_data, 6);
    memcpy(eh->ether_dhost, ip8021x_pae_grp_addr, 6);

    int numbytes = sendto(sockfd, ctx->pkt->buf, ctx->pkt->len, 0, (struct sockaddr *)&saddr, sizeof(saddr));
    if (numbytes == -1) {
        perror("sendto err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);
    show_eap_buf_h(ctx->pkt, EAP_REQUEST);
}

void ip8021x_eapol_input()
{
    int i = 0;
    Ip8021x_ctx* ctx;
    // int sockfd;
    // struct ifreq ifr;
    // struct sockaddr_ll saddr;
    Ip8021x_pkt in_pkt;
    
    ctx = malloc(sizeof(struct Ip8021x_ctx_st));

    for(i = 0; i < 6; i++)
        ctx->aa[i] = i;

    ctx->mode = IP8021X_EAPOL_MODE_WIRED;

    for(i = 0; i < 6; i++)
        ctx->spa[i] = i+1;

    ip8021x_eapol_msg_input(ctx, &in_pkt);

    show_eap_buf_h(ctx->pkt, EAPOL_RESPONSE);
}

void ip8021x_send_failure()
{
    int i = 0;
    Ip8021x_ctx* ctx;
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_ll saddr;
    
    ctx = malloc(sizeof(struct Ip8021x_ctx_st));

    for(i = 0; i < 6; i++)
        ctx->aa[i] = i;

    // ctx->mode = IP8021X_EAPOL_MODE_WIRED;

    for(i = 0; i < 6; i++)
        ctx->spa[i] = i+1;
    
    ip8021x_auth_req_tmo_cb(ctx);

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, INTERFACE, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1)
    {
        perror("ioctl SIOCGIFINDEX err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("nic name: %s\n", ifr.ifr_name);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sll_family = AF_PACKET;
    saddr.sll_ifindex = ifr.ifr_ifindex;
    saddr.sll_halen = ETH_ALEN;
    memcpy(saddr.sll_addr, ip8021x_pae_grp_addr, 6);
    
    get_mac(sockfd, &ifr);
    struct ether_header *eh = (struct ether_header *)(ctx->pkt->buf);
    
    memcpy(eh->ether_shost, ifr.ifr_netmask.sa_data, 6);
    // memcpy(eh->ether_dhost, ip8021x_pae_grp_addr, 6);

    int numbytes = sendto(sockfd, ctx->pkt->buf, ctx->pkt->len, 0, (struct sockaddr *)&saddr, sizeof(saddr));
    if (numbytes == -1) {
        perror("sendto err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    show_eap_buf_h(ctx->pkt, EAP_FAILURE);
}

void ip8021x_send_logoff()
{
    int i = 0;
    Ip8021x_ctx* ctx;
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_ll saddr;
    
    ctx = malloc(sizeof(struct Ip8021x_ctx_st));

    for(i = 0; i < 6; i++)
        ctx->aa[i] = i;

    ctx->mode = IP8021X_EAPOL_MODE_WIRED;

    for(i = 0; i < 6; i++)
        ctx->spa[i] = i+1;
    
    ip8021x_tx_eapol_logoff(ctx);

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, INTERFACE, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1)
    {
        perror("ioctl SIOCGIFINDEX err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("nic name: %s\n", ifr.ifr_name);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sll_family = AF_PACKET;
    saddr.sll_ifindex = ifr.ifr_ifindex;
    saddr.sll_halen = ETH_ALEN;
    memcpy(saddr.sll_addr, ip8021x_pae_grp_addr, 6);
    
    get_mac(sockfd, &ifr);
    struct ether_header *eh = (struct ether_header *)(ctx->pkt->buf);
    
    memcpy(eh->ether_shost, ifr.ifr_netmask.sa_data, 6);
    memcpy(eh->ether_dhost, ip8021x_pae_grp_addr, 6);

    int numbytes = sendto(sockfd, ctx->pkt->buf, ctx->pkt->len, 0, (struct sockaddr *)&saddr, sizeof(saddr));
    if (numbytes == -1) {
        perror("sendto err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);
    show_eap_buf_h(ctx->pkt, EAPOL_LOGOFF);
}

void ip8021x_send_success()
{
    int i = 0;
    Ip8021x_ctx* ctx;
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_ll saddr;
    
    ctx = malloc(sizeof(struct Ip8021x_ctx_st));

    for(i = 0; i < 6; i++)
        ctx->aa[i] = i;

    ctx->mode = 0;

    for(i = 0; i < 6; i++)
        ctx->spa[i] = i+1;

    ip8021x_tx_eapol_success(ctx);

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, INTERFACE, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1)
    {
        perror("ioctl SIOCGIFINDEX err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("nic name: %s\n", ifr.ifr_name);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sll_family = AF_PACKET;
    saddr.sll_ifindex = ifr.ifr_ifindex;
    saddr.sll_halen = ETH_ALEN;
    memcpy(saddr.sll_addr, ip8021x_pae_grp_addr, 6);
    
    get_mac(sockfd, &ifr);
    struct ether_header *eh = (struct ether_header *)(ctx->pkt->buf);
    
    memcpy(eh->ether_shost, ifr.ifr_netmask.sa_data, 6);

    int numbytes = sendto(sockfd, ctx->pkt->buf, ctx->pkt->len, 0, (struct sockaddr *)&saddr, sizeof(saddr));
    if (numbytes == -1) 
    {
        perror("sendto err\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);
    show_eap_buf_h(ctx->pkt, EAPOL_SUCCESS);
}

int main(int argc, char  *argv[])
{
    // ip8021x_send_start();
    // ip8021x_send_request();
    // ip8021x_eapol_input();
    // ip8021x_send_success();
    // ip8021x_send_failure();
    // ip8021x_send_logoff();
    
    send_lldp();

    return 0;
}