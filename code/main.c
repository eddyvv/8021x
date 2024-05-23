#include "8021x.h"
#include "lldp.h"

int main(int argc, char  *argv[])
{
    ip8021x_send_start();
    ip8021x_send_request();
    ip8021x_eapol_input();
    ip8021x_send_success();
    ip8021x_send_failure();
    ip8021x_send_logoff();
    
    send_lldp();

    return 0;
}