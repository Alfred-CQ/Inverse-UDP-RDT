#include "../include/udp_client.h"
#include "../include/utils.h"

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
void UDP_Client::send_Request(string resource_request)
{
    if (resource_request != START)
        cout << "\n 🗂️ Request: " << resource_request << " successfully sent ✅\n";
    else
    {
        cout << "\n 🗳️ End of requests, command [ " << resource_request << " ] found \n\n";
        return;
    }

    string request    = utils::complete_Bytes(resource_request.size(), 2) + resource_request;

    bytes_send = sendto(sockFD, &(request.front()), request.size(), 0, (SOCK_ADDR*)& server_addr, SOCK_ADDR_SIZE);
}

// Receivers
void UDP_Client::recv_Responses()
{
    
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