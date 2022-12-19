#include "../include/main_header.h"
#include "../include/udp_client.h"

int main()
{
    UDP_Client* myClient        { new UDP_Client("127.0.0.1", PORT) };
    string      command         { "Y" };
    RESOURCES   resource_names;
    
    myClient->print_Information();

    while (command != "N")
    {
        cout << "************************************************************"
             << "\n[ -all for all resources ] [ -start to start receiving]\n"
             << "           Type the resources you need🔎\n";

        while (command != ALL && command != START)
        {
            string resource_name;

            cout << "  > "; cin >> resource_name;

            resource_names.push_back(resource_name);

            command = resource_name;
        }

        for (size_t i = 0; i < resource_names.size(); ++i)
            myClient->send_Request(resource_names[i]);

        cout << "Would like to retry? (Y/N)" << endl; cin >> command;

        resource_names.clear();
    }

    cout << " 🏁 FINISH UDP CLIENT PROGRAM 🏁\n";

    return 0;
}