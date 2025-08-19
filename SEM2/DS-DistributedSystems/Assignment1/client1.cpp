#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
using namespace std;

#define PORT_NO 7000
#define SERVER_PORT 8000

void checkError(int value, const char *message)
{
    if (value < 0)
    {
        cout << errno << message << endl;
        exit(EXIT_FAILURE);
    }
}
void getFile(string filename, int client_socket, sockaddr_in server_address)
{
    char message[10];
    int i = 0;
    socklen_t server_address_len = sizeof(server_address);
    sendto(client_socket, filename.c_str(), 255, 0,
           (struct sockaddr *)&server_address, server_address_len);
    do
    {

        int recv_status = recvfrom(client_socket, message, 255, 0,
                                   (struct sockaddr *)&server_address, &server_address_len);

            cout << char(stoi(message));
        }
     while (1);
    cout << endl;
}
int main()
{
    int client_socket, bind_status;

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_NO);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    checkError(client_socket, "socket creation failed");



    bind_status = bind(client_socket, (struct sockaddr *)&address,
                       sizeof(address));
    checkError(bind_status, "socket bind failed");

    getFile("HelloWorld.txt", client_socket, server_address);

    return 0;
}
