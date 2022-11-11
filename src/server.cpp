#include "../include/resources.h"

int main()
{
    int sock;
    int bytes_read;
    socklen_t addr_len;

    char recv_data[1024];
    struct sockaddr_in server_addr, client_addr;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Bind");
        exit(1);
    }

    addr_len = sizeof(struct sockaddr);

    cout << "UDPServer Waiting for client on port 5000" << endl;

    while (1)
    {
        bytes_read = recvfrom(sock, recv_data, 1024, MSG_WAITALL, (struct sockaddr *)&client_addr, &addr_len);
        recv_data[bytes_read] = '\0';

        cout << "Received (Client) : " << inet_ntoa(client_addr.sin_addr) << " - " << ntohs(client_addr.sin_port) << "\n";
        cout << " [ Msg ] " << recv_data;

        sendto(sock, "Hola", 4, MSG_CONFIRM, (const struct sockaddr *)&client_addr, addr_len);

        cout << "Hello message sent.\n\n";

        cout.flush();
    }

    return 0;
}