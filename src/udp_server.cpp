#include "../include/udp_server.h"

/* Constructors */
UDP_Server::UDP_Server(string _ip, uint _port)
{
    ip       = _ip;
    port     = _port;
    addr_len = sizeof(struct sockaddr);
    
    if ((sockFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(addr.sin_zero), 8);

    if (bind(sockFD, (SOCK_ADDR *)&addr, sizeof(SOCK_ADDR)) == -1)
    {
        perror("Bind");
        exit(1);
    }

    current_path   = fs::current_path();
    resources_path = "../" + string(RESOURCES_DIR) + "/";

    for (const auto & entry : fs::directory_iterator(current_path))
    {
        if (entry.is_directory() && entry.path().filename() == RESOURCES_DIR)
                resources_path = string(RESOURCES_DIR) + "/";
    }
}

/* Methods */
// Senders
void UDP_Server::send_Response(string message)
{
    sendto(sockFD, &(message.front()), message.size(), MSG_CONFIRM, (SOCK_ADDR*)& client_addr, addr_len);

    cout << message << " message sent.\n\n";

    cout.flush();
}

// Receivers
void UDP_Server::recv_Requests()
{
    bytes_recv = recvfrom(sockFD, recv_buffer, 1024, MSG_WAITALL,  (SOCK_ADDR*)& client_addr, &addr_len);
    recv_buffer[bytes_recv] = '\0';

    cout << "Received (Client) : " << inet_ntoa(client_addr.sin_addr) << " - " << ntohs(client_addr.sin_port) << "\n";
    cout << " [ Msg ] " << recv_buffer << endl;
}

// Utilities
void UDP_Server::print_Information()
{   
    system("clear");
    
    cout << "      ░░█▀▀░█▀▀░█▀▄░█░█░█▀▀░█▀▄░░░█░█░█▀▄░█▀█░░\n" 
         << "      ░░▀▀█░█▀▀░█▀▄░▀▄▀░█▀▀░█▀▄░░░█░█░█░█░█▀▀░░\n"
         << "      ░░▀▀▀░▀▀▀░▀░▀░░▀░░▀▀▀░▀░▀░░░▀▀▀░▀▀░░▀░░░░\n"
         << "******************************************************\n"
         << "        IP            : " << ip << "\n"
         << "        PORT          : " << port << "\n"
         << "        RESOURCES_PATH: " << resources_path << "\n"
         << "******************************************************"
         << endl;
}