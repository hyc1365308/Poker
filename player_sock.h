#include <iostream>
#include <winsock2.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <pthread.h>

#include "packet.h"

const int ROOM_NUM = 4;
const int MAX_PLAYER_NUM = 8;

class PlayerSock
{
private:
    std::string id;             // user id(user name)
    // sockaddr_in addr;        // address, in case of need
    const SOCKET sock;

    char recv_buffer[512];      // receive buffer
    char send_buffer[512];      // send buffer

public:
    PlayerSock(const SOCKET sock, const std::string & id) : sock(sock), id(id)
    {
        std::cout << "A new player login now" << std::endl;
        std::cout << "player id = " << id << ", sock = " << sock << std::endl;
    }

    ~PlayerSock()
    {
        std::cout << "Player " << id << " quit now" << std::endl;
        closesocket(sock);
    }

    bool testConnect()
    {
        // char test_buffer[] = "test alive\r\n";
        std::string packet = Packet::testAlive();
        if (send(sock, packet.c_str(), packet.size(), 0) <= 0)
            // sock has been closed
            return false;
        else
            return true;
    }

    bool sendData(std::string data)
    {
        if (send(sock, data.c_str(), data.size() + 1, 0) <= 0)
            return true;
        else
            return false;
    }

    std::string recvData()
    {
        int ret = recv(sock, recv_buffer, 512, 0);
        if(ret > 0)
        {
            recv_buffer[ret] = 0x00;
            // std::cout << "Recv : " << recv_buffer << std::endl;
            return recv_buffer;
        }
        return "";
    }

    friend std::ostream & operator<< (std::ostream & out, const PlayerSock * player)
    {
        out << "user id: " << player->id << ", socket num = " << player->sock;
        return out;
    }

    std::string get_id() { return id; }
};

