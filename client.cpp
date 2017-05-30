#include <WINSOCK2.H>
#include <STDIO.H>

#include "packet.h"

#pragma  comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA data; 
    if(WSAStartup(sockVersion, &data) != 0)
    {
        return 0;
    }

    SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sclient == INVALID_SOCKET)
    {
        printf("invalid socket !");
        return 0;
    }

    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(8900);
    serAddr.sin_addr.S_un.S_addr = inet_addr("183.172.137.118"); 
    if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("connect error !");
        closesocket(sclient);
        return 0;
    }
    // char * sendData = "Hello, I'm coming!\n";
    std::string user_name  = "test_user";
    std::string password   = "test";
    std::string packet_str = Packet::login(user_name, password);
    send(sclient, packet_str.c_str(), packet_str.size(), 0);

    char recData[255];

    while(true)
    {
        // int ret = recv(sclient, recData, 255, 0);
        // Json::Value root;
        // if(ret > 0 && Packet::decode(recData, root))
        // {
        //     recData[ret] = 0x00;
        //     printf(recData);
        // }
        int room_num;
        std::cout << "> ";
        std::cin >> room_num;
        std::cout << std::endl;
        packet_str = Packet::entry(room_num);
        send(sclient, packet_str.c_str(), packet_str.size(), 0);
    }

    closesocket(sclient);
    WSACleanup();
    return 0;
}
