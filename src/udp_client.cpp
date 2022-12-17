#include "../include/udp_client.h"
#include "../include/utils.h"

/* Constructors and Destructor */
UDP_Client::UDP_Client(string _ip_server, uint _port)
{
    host            = (HOST *)gethostbyname((char *)_ip_server.c_str());
    port            = _port;
    addr_len        = sizeof(struct sockaddr);
    recv_buffer     = new char[MAX_MESSAGE_SIZE + 1];
    streams_status  = vector<bool>(NUMBER_STREAMS, true);
    streams         = vector<Response*>(NUMBER_STREAMS, nullptr);

    if ((sockFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(port);
    server_addr.sin_addr   = *((IN_ADDR *)host->h_addr);

    bzero(&(server_addr.sin_zero), 8);

    thread(&UDP_Client::recv_Responses, this).detach();
}

UDP_Client::~UDP_Client()
{

}

/* Methods */
// Senders
void UDP_Client::send_Request(string resource_request)
{
    if (resource_request != START)
    {
        cout << "\n 🗂️ Request: " << resource_request << " successfully sent ✅\n";
        resource_names.push_back(resource_request);
    }
    else
    {
        cout << "\n 🗳️ End of requests, command [ " << resource_request << " ] found \n\n";
        return;
    }

    string request    = utils::complete_Bytes(resource_request.size(), 2) + resource_request;

    bytes_send = sendto(sockFD, &(request.front()), request.size(), 0, (SOCK_ADDR*)& server_addr, SOCK_ADDR_SIZE);
}

void UDP_Client::resend_Request(uint stream, uint sequence_number)
{
    cout << " ⚠️ Error in cheksum re-requesting segment " << sequence_number << " in stream " << stream << endl;

    string resend_request = utils::complete_Bytes(0, 2) + 
                            std::to_string(stream) +
                            std::to_string(sequence_number);

    sendto(sockFD, &(resend_request.front()), resend_request.size(), 0, (SOCK_ADDR*)& server_addr, SOCK_ADDR_SIZE);
}

// Receivers
void UDP_Client::recv_Responses()
{
    char ch_stream;
    uint current_stream;   

    /***** Testing *******/
    uint i = 0;
    /*********************/
    while (1)
    {
        bytes_recv = recvfrom(sockFD, recv_buffer, MAX_MESSAGE_SIZE, MSG_WAITALL, (SOCK_ADDR *)& server_addr, &addr_len);
        recv_buffer[bytes_recv] = '\0';

        // Stream
        ch_stream = recv_buffer[0];
        current_stream = atoi(&ch_stream);

        // # Sequence
        string sequence_number(recv_buffer, 1, 5);

        // Padding
        string padding(recv_buffer, 11, 3);

        // Data
        string data(recv_buffer, 14, MAX_DATA_SIZE);

        // Checksum
        string str_checksum(recv_buffer, MAX_DATA_SIZE + 14, 1);

        if (streams_status[current_stream] == AVAILABLE)
        {
            string number_segments(recv_buffer, 6, 5);
            
            streams[current_stream] = new Response( resource_names[current_stream], 
                                                    current_stream, 
                                                    atoi(&(number_segments.front())));

            streams_status[current_stream] = UNAVAILABLE;                                  
        }

        /***** Testing *******/
        if (i == 0)
            data[0] = '/'; data[1] = '-'; data[5] = '@';
        /*********************/

        if (utils::test_Checksum(data, atoi(&(str_checksum.front()))))
        {
            streams[current_stream]->insert_Segment(atoi(&(sequence_number.front())), 
                                                    atoi(&(padding.front())), 
                                                    &(data.front()));
        }
        else
        {
            resend_Request(streams[current_stream]->get_Stream(), atoi(sequence_number.c_str()));
        }

        if (streams[current_stream]->get_Counter() == streams[current_stream]->get_Numb_Segments())
        {
            streams[current_stream]->build_Response(REQUESTS_DIR);
            streams_status[current_stream] = AVAILABLE;
        }

        /***** Testing *******/
        i = (i + 1)%10;
        /*********************/
    }
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