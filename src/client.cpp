#include "../include/resources.h"

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    struct hostent *host;
    char send_data[1024];
    char recv_data[1024];
    int n, bytes_read;

    host = (struct hostent *)gethostbyname((char *)"127.0.0.1");

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero), 8);

    socklen_t addr_len = sizeof(struct sockaddr);

    while (1)
    {
        cout << "Type Something (q or Q to quit): ";
        fgets(send_data, 1024, stdin);

        if ((strcmp(send_data, "q\n") == 0) || strcmp(send_data, "Q\n") == 0)
            break;

        n = sendto(sock, send_data, strlen(send_data), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

        cout << "Send to (" << n << ") bytes\n";

        bytes_read = recvfrom(sock, recv_data, 1024, MSG_WAITALL, (struct sockaddr *)&server_addr, &addr_len);
        recv_data[bytes_read] = '\0';

        cout << "[ Msg Server ] " << recv_data << "\n\n";
    }

    return 0;
}