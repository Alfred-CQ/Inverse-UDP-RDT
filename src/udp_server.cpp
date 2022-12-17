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

    recv_buffer = new char[REQUEST_NAME_SIZE];

    thread(&UDP_Server::recv_Requests, this).detach();
}

/* Methods */
// Senders
void UDP_Server::send_Responses(string resource_name)
{

}

// Receivers
void UDP_Server::recv_Requests()
{
    char size_Request[3];
    string resource_name;
    
    while (1)
    {
        bytes_recv = recvfrom(sockFD, recv_buffer, REQUEST_NAME_SIZE, 0, (SOCK_ADDR *)& client_addr, &addr_len);
        
        strncpy(size_Request, recv_buffer, 2);

        resource_name = string(recv_buffer, 2, atoi(size_Request));

        if (find_Resource(resource_name))
            cout << " Resource " << resource_name << " found ✨ in " << resources_path << " 🗃\n";
        else
            cout << " Resource " << resource_name << " not found 🚫\n";

        bytes_recv = 0;
    }
}

// Utilities
bool UDP_Server::find_Resource(string resource_name)
{
    for (const auto & entry : fs::directory_iterator(resources_path))
    {
        if ( entry.path().filename() == resource_name )
            return true;
    }

    return false;
}

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