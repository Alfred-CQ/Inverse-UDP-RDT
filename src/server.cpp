#include "../include/udp_server.h"

int main()
{
    UDP_Server* myServer    { new UDP_Server("127.0.0.1") };
 
    myServer->print_Information();

    cout << "   ⏳ UDPServer waiting for data transmission ⏳\n";

    while (1)
    {
        myServer->recv_Requests();
        myServer->send_Response("Hello");
    }

    cout << " 🏁 FINISH UDP SERVER PROGRAM 🏁\n";
}