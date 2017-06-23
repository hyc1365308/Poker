#include <WINSOCK2.H>
#include <STDIO.H>

#include "packet.h"
#include "client.h"

#pragma  comment(lib,"ws2_32.lib")

void Client::init()
{
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA data; 
    if(WSAStartup(sockVersion, &data) != 0)
    {
        std::cout << "WSAStartup wrong!!!" << std::endl;
        exit(-1);
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == INVALID_SOCKET)
    {
        std::cout << "invalid socket !" << std::endl;
        exit(-1);
    }
}

bool Client::connectServer()
{
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str());

    if (connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        printf("connect error!!!\n");
        closesocket(sock);
        return false;
    }

    return true;
}

bool Client::sendData(std::string && data)
{
    if (send(sock, data.c_str(), data.size(), 0) > 0)
        return true;

    return false;
}

Json::Value Client::recvData()
{
    Json::Value root;

    while (true)
    {
        memset(recv_buffer, 0, strlen(recv_buffer) * sizeof(char));
        int ret = recv(sock, recv_buffer, BUFFER_SIZE, 0);
        if(ret > 0 && Packet::decode(recv_buffer, root))
        {
            break;
        }
    }

    return root;
}

bool Client::login(const std::string & user_name, const std::string & password)
{
    if(!sendData(Packet::login(user_name, password)))
    {
        std::cout << "send packet wrong!" << std::endl;
        return false;
    }

    Json::Value root = recvData();

    if (root["type"] == LOGIN_RESULT && root["result"].asInt() == SUCCEED)
    {
        std::cout << "login succeed, money is " << root["money"] << std::endl;
    }
}

bool Client::entry(const int room_id)
{
    if (!sendData(Packet::entry(room_id)))
    {
        std::cout << "send packet wrong!" << std::endl;
        return false;
    }

    Json::Value root = recvData();

    if (root["type"] == ENTRY_RESULT)
    {
        if (root["result"].asInt() == SUCCEED)
        {
            std::cout << "entry room " << root["room"] << std::endl;

            root = recvData();
            if (root["type"] == ROOM)
            {
                std::vector<PlayerTuple> room_info = Packet::getRoomInfo(root);
                printf("%12s:%11s%8s%6s\n", "player num", "ID", "Money", "In");
                for (int i = 0; i < room_info.size(); ++i)
                {
                    std::string player_id = std::get<0>(room_info[i]);
                    int money = std::get<1>(room_info[i]);
                    
                    std::string in_pos;
                    if (std::get<2>(room_info[i]))
                    {
                        in_pos = "mine";
                    }
                    else
                    {
                        in_pos = "----";
                    }

                    printf("%12s:%11s%8d%6s\n", i, player_id, money, in_pos);
                }
            }

            return true;
        }
        else
        {
            std::cout << "cannot entry room " << room_id << std::endl;
            return false;
        }
    }
}

int main(int argc, char* argv[])
{
    Client client(8900, "127.0.0.1");

    std::string user_name = argv[1];
    std::string password  = argv[2];

    client.connectServer();

    // login
    client.login(user_name, password);

    // entry a room
    std::cout << "Please input room num: ";
    int room_id;
    std::cin >> room_id;

    client.entry(room_id);

    // begin a game
    /*while(true)
    {
        memset(recv_buffer, 0, strlen(recv_buffer) * sizeof(char));
        ret = recv(sock, recv_buffer, BUFFER_SIZE, 0);
        if(ret > 0 && Packet::decode(recv_buffer, root))
        {
            recv_buffer[ret] = 0x00;
            if (root["type"] != REQUSET)
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
                    send(sock, packet_str.c_str(), packet_str.size(), 0);
                }
                else if (operation == "refuel")
                {
                    int money;
                    std::cin >> money;
                    packet_str = Packet::refuel(money);
                    send(sock, packet_str.c_str(), packet_str.size(), 0);
                }
                else if (operation == "fold")
                {
                    packet_str = Packet::fold();
                    send(sock, packet_str.c_str(), packet_str.size(), 0);
                }
                else if (operation == "allin")
                {
                    packet_str = Packet::allin();
                    send(sock, packet_str.c_str(), packet_str.size(), 0);
                }
            }

            std::cout << "send data:" << std::endl << packet_str << std::endl;
        }
        else
        {
            continue;
        }
    }*/

    return 0;
}
