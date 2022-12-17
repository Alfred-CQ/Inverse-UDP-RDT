#include "../include/udp_client.h"

/* Constructors and Destructor */
UDP_Client::UDP_Client(string _ip_server, uint _port)
{
    host     = (HOST *)gethostbyname((char *)_ip_server.c_str());
    port     = _port;
    addr_len = sizeof(struct sockaddr);

    if ((sockFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(port);
    server_addr.sin_addr   = *((IN_ADDR *)host->h_addr);

    bzero(&(server_addr.sin_zero), 8);
}

UDP_Client::~UDP_Client()
{

}

/* Methods */
// Senders
void UDP_Client::send_Message(string message)
{
    bytes_send = sendto(sockFD, &(message.front()), message.size(), 0, (SOCK_ADDR*)& server_addr, SOCK_ADDR_SIZE);

    cout << "Send to (" << bytes_send << ") bytes\n";
}

// Receivers
void UDP_Client::recv_Response()
{
    bytes_recv = recvfrom(sockFD, recv_buffer, 1024, MSG_WAITALL, (SOCK_ADDR *)& server_addr, &addr_len);
    
    recv_buffer[bytes_recv] = '\0';

    cout << "[ Msg Server ] " << recv_buffer << "\n\n";
}

// Utilities
void UDP_Client::print_Information()
{
    system("clear");

    cout << "         ░░█▀▀░█░░░▀█▀░█▀▀░█▀█░▀█▀░░░█░█░█▀▄░█▀█░░\n" 
         << "         ░░█░░░█░░░░█░░█▀▀░█░█░░█░░░░█░█░█░█░█▀▀░░\n"
         << "         ░░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░▀░░▀░░░░▀▀▀░▀▀░░▀░░░░\n"
         << "************************************************************\n"
         << "                PORT          : " << port
         << endl;
}