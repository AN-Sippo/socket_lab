#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
int main()
{
    struct sockaddr_in address;
    int sock, new_socket;
    int addrlen = sizeof(address);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Listening at port: %d\n", ntohs(address.sin_port));

    struct sockaddr_in client_addr;
    if ((new_socket = accept(sock, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // 相手のIPアドレスとポート番号を取得
    printf("Client IP: %s, Client port: %d\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    const char *hello = "Hello from server";
    send(new_socket, hello, strlen(hello), 0);
    printf("client message:%s\n", buffer);
    close(new_socket);
    close(sock);
    return 0;
}