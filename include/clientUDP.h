#pragma once
#include "resources.h"

class ClientUDP{
public:
    int sock;
    struct sockaddr_in server_addr;
    struct hostent *host;
    char send_data[1024];
    char recv_data[1024];
    int n;
    int bytes_read;
    socklen_t addr_len;
    ClientUDP(int _port);

};