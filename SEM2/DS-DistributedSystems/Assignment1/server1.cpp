#include <netinet/in.h>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <fstream>
#include <unistd.h>
#include <errno.h>
using namespace std;
#define PORT_NO 8000
#define CLIENT_PORT 7000

void checkError(int value, const char *message)
{
    if (value < 0)
    {
        cout << errno << message << endl;
        exit(EXIT_FAILURE);
    }
}
void sendDataBlock(int server_socket, sockaddr_in client_address, int data)
{
    string s = to_string(data);
    char const *message = s.c_str();
    cout << "sending data '"<<char(data)<<"' to client" << endl;
    socklen_t client_address_len = sizeof(client_address);
    int send_status = sendto(server_socket, message, 255, 0,
                             (struct sockaddr *)&client_address, client_address_len);
}

int getDataBlock(string filename, int point)
{
    char buffer[4];
    int data;

    fstream infile;
    infile.open(filename, ios::in | ios::out | ios::binary);
    infile.seekg(point, ios::beg);
    infile.read(buffer, 1);

    if (infile.eof())
    {
        data = -1;
    }
    else
    {
        data = int(buffer[0]);

    }
    infile.close();

    return data;
}
void receiveRequest(int server_socket, sockaddr_in client_address)
{
    char message[256];
    cout << "server listening....." << endl;
    while (1)
    {
        socklen_t client_address_len = sizeof(client_address);

        int recv_status = recvfrom(server_socket, message, 255, 0,
                                   (struct sockaddr *)&client_address, &client_address_len);

        cout << "request received for "<<message << endl;
        int data_block=0;
        int index = 0;
        while (data_block != -1)
        {   
            data_block = getDataBlock(message, index++);
            sendDataBlock(server_socket, client_address, data_block);
        }
    }
}
int main()
{
    int server_socket, bind_status;

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_NO);

    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY;
    client_address.sin_port = htons(CLIENT_PORT);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    checkError(server_socket, "socket creation failed");
    cout << "socket created" << endl;
    bind_status = bind(server_socket, (struct sockaddr *)&address,
                       sizeof(address));
    checkError(bind_status, "socket bind failed");

    receiveRequest(server_socket, client_address);

    return 0;
}
