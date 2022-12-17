#include "../include/udp_server.h"

int main()
{
    UDP_Server* myServer    { new UDP_Server("127.0.0.1", PORT) };
 
    myServer->print_Information();

    cout << "   ⏳ UDPServer waiting for data transmission ⏳\n";

    while (1)
    {
        sleep(5);
    }

    cout << " 🏁 FINISH UDP SERVER PROGRAM 🏁\n";
}