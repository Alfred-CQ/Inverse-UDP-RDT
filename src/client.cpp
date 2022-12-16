#include "../include/resources.h"
#include "ClientUDP.h"

int main()
{
    ClientUDP client(5000);

    while (1)
    {
        cout << "Type Something (q or Q to quit): ";
        fgets(client.send_data, 1024, stdin);

        if ((strcmp(client.send_data, "q\n") == 0) || strcmp(client.send_data, "Q\n") == 0)
            break;

        client.n = sendto(client.sock, client.send_data, strlen(client.send_data), 0, (struct sockaddr *)&client.server_addr, sizeof(struct sockaddr));

        cout << "Send to (" << client.n << ") bytes\n";

        client.bytes_read = recvfrom(client.sock, client.recv_data, 1024, MSG_WAITALL, (struct sockaddr *)&client.server_addr, &client.addr_len);
        client.recv_data[client.bytes_read] = '\0';

        cout << "[ Msg Server ] " << client.recv_data << "\n\n";
    }

    return 0;
}