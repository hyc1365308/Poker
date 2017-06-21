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
    serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); 
    if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("connect error !");
        closesocket(sclient);
        return 0;
    }
    // char * sendData = "Hello, I'm coming!\n";
    // std::string user_name  = "test_user";
    // std::string password   = "test";
    std::string user_name = argv[1];
    std::string password  = argv[2];
    std::string packet_str = Packet::login(user_name, password);
    send(sclient, packet_str.c_str(), packet_str.size(), 0);

    char recData[255];
    int ret;
    Json::Value root;

    // login
    while (true)
    {
        ret = recv(sclient, recData, 255, 0);
        if(ret > 0 && Packet::decode(recData, root))
        {
            recData[ret] = 0x00;
            if (root["type"] == LOGIN_RESULT && root["result"].asInt() == SUCCEED)
            {
                std::cout << "login succeed, money is " << root["money"] << std::endl;
            }
            break;
        }
    }

    // entry a room
    while (true)
    {
        std::cout << "Please input room num: ";
        int room_id;
        std::cin >> room_id;
        std::cout << std::endl;
        packet_str = Packet::entry(room_id);
        send(sclient, packet_str.c_str(), packet_str.size(), 0);
        ret = recv(sclient, recData, 255, 0);
        if(ret > 0 && Packet::decode(recData, root))
        {
            recData[ret] = 0x00;
            if (root["type"] == ENTRY_RESULT && root["result"].asInt() == SUCCEED)
            {
                std::cout << "entry room " << root["room"] << std::endl;
            }
            break;
        }
    }

    // begin a game
    while(true)
    {
        ret = recv(sclient, recData, 255, 0);
        if(ret > 0 && Packet::decode(recData, root))
        {
            recData[ret] = 0x00;
            if (root["type"] == TEST_ALIVE)
            {
                std::cout << "server test alive" << std::endl;
                continue;
            }
            else if (root["type"] != REQUSET)
            {
                continue;
            }
            else
            {
                std::cout << "Please input an operator(call, refuel, fold, allin)" << std::endl;
                std::string operation;
                std::cin >> operation;
                if (operation == "call")
                {
                    packet_str = Packet::call();
                    send(sclient, packet_str.c_str(), packet_str.size(), 0);
                }
                else if (operation == "refuel")
                {
                    int money;
                    std::cin >> money;
                    packet_str = Packet::refuel(money);
                    send(sclient, packet_str.c_str(), packet_str.size(), 0);
                }
                else if (operation == "fold")
                {
                    packet_str = Packet::fold();
                    send(sclient, packet_str.c_str(), packet_str.size(), 0);
                }
                else if (operation == "allin")
                {
                    packet_str = Packet::allin();
                    send(sclient, packet_str.c_str(), packet_str.size(), 0);
                }
            }

            std::cout << "send data:" << std::endl << packet_str << std::endl;
        }
        else
        {
            continue;
        }
    }

    closesocket(sclient);
    WSACleanup();
    return 0;
}
