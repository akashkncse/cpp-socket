#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

int main()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == -1)
    {
        perror("socket error");
        return 0;
    }
    else
    {
        std::cout << "socket() OK!" << std::endl;
    }
    sockaddr_in sockinfo;
    sockinfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockinfo.sin_family = AF_INET;
    sockinfo.sin_port = htons(8000);

    if (connect(client_socket, (sockaddr *)&sockinfo, sizeof(sockinfo)) == -1)
    {
        perror("connect() error");
        return 0;
    }
    else
    {
        std::cout << "connect() OK!" << std::endl;
    }
    char msg[40] = "hello!";
    int pre = strlen(msg);
    char buf[40];
    buf[0] = pre;
    strcpy(buf + 1, msg);
    send(client_socket, &buf, pre + 1, 0);
    return 0;
}