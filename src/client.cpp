#include "../include/main_header.h"
#include "../include/udp_client.h"

int main()
{
    UDP_Client* myClient { new UDP_Client("127.0.0.1", PORT) };

    myClient->print_Information();

    while (1)
    {
        string message;
        
        cout << "Type Something (Q to quit): ";
        getline(cin, message);

        if ((message.size() == 0) || (message == "Q"))
            break;

        myClient->send_Message(message);

        myClient->recv_Response();
    }

    cout << " 🏁 FINISH UDP CLIENT PROGRAM 🏁\n";

    return 0;
}