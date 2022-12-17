#include "../include/udp_server.h"
#include "../include/utils.h"

/* Constructors */
UDP_Server::UDP_Server(string _ip, uint _port)
{
    ip          = _ip;
    port        = _port;
    addr_len    = sizeof(struct sockaddr);
    recv_buffer = new char[REQUEST_NAME_SIZE];
    stream      = 0;
    streams_status  = vector<bool>(NUMBER_STREAMS, true);
    streams         = vector<Response*>(NUMBER_STREAMS, nullptr);
    
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

    thread(&UDP_Server::recv_Requests, this).detach();
}

/* Methods */
// Senders
void UDP_Server::send_Response(string resource_name)
{
    string   resource_path { resources_path + resource_name };
    ifstream resource      { resource_path, std::ios::in };

    int      size_resource;
    uint     number_segments, padding{0};
    char     data_read[MAX_DATA_SIZE];

    resource.seekg(0, resource.end);
    size_resource = resource.tellg();
    resource.seekg(0, resource.beg);

    number_segments = (size_resource + MAX_DATA_SIZE - 1) / MAX_DATA_SIZE;
    
    Response* response = new Response(resource_name, stream++, number_segments);
    
    response->set_Source(sockFD);
    response->set_Destination((SOCK_ADDR*)& client_addr);

    streams[response->get_Stream()] = response;
    streams_status[response->get_Stream()] = UNAVAILABLE;

    cout << "******************************************************\n"
         << " 📨 Sending resource " << resource_name << " in the stream "<< response->get_Stream() << endl;

    for (size_t i = 0; i < number_segments; ++i)
    {
        resource.read(data_read,MAX_DATA_SIZE);
        
        size_resource -= MAX_DATA_SIZE;

        if (size_resource < 0)
        {
            padding = (size_resource * -1);
            utils::fill_Zeros(data_read, padding);
        }

        response->insert_Segment( i + 10, padding, data_read);
        response->send_Segment(i + 10);
    }

    response->print_Head();
    response->print_Tail();
    
    cout << " 📪 Resource sent \n"
         << "******************************************************" << endl;
         
    resource.close();
}

void UDP_Server::resend_Segment(string request)
{
    string srt_stream(request, 0, 1),
           str_sequence_number(request, 1, 5);
    
    streams[atoi(srt_stream.c_str())]->send_Segment(atoi(str_sequence_number.c_str()));
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

        if (atoi(size_Request))
        {
            if (find_Resource(resource_name))
            {
                cout << " Resource " << resource_name << " found ✨ in " << resources_path << " 🗃\n";
                thread(&UDP_Server::send_Response, this, resource_name).detach();
            }
            else
                cout << " Resource " << resource_name << " not found 🚫\n";
        }
        else
        {
            resend_Segment(string(recv_buffer, 2, 6));
        }
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