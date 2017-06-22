#include <iostream>
#include <winsock2.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <pthread.h>

#include "packet.h"

#define BUFFER_SIZE 512

class PlayerSock
{
private:
    std::string id;             // user id(user name)
    int money;                  // user money
    // sockaddr_in addr;        // address, in case of need
    const SOCKET sock;

    char recv_buffer[BUFFER_SIZE];      // receive buffer
    char send_buffer[BUFFER_SIZE];      // send buffer

public:
    PlayerSock(const SOCKET sock, const std::string & id, const int money) : sock(sock), id(id), money(money)
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
        // std::string packet = Packet::testAlive();
        // if (send(sock, packet.c_str(), packet.size(), 0) <= 0)
        //     // sock has been closed
        //     return false;
        // else
        //     return true;

        // struct tcp_info info; 
        // int len = sizeof(info);
        // getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len); 

        // if (info.tcpi_state == TCP_ESTABLISHED)
        // { 
        //     //myprintf("socket connected\n"); 
        //     return true;
        // } 
        // else 
        // { 
        //     //myprintf("socket disconnected\n"); 
        //     return false;
        // } 

        // int select_flag = select(NULL, &sock, NULL, NULL, NULL);
        int recv_flag = recv(sock, recv_buffer, BUFFER_SIZE, 0);
        std::cout << "check sock " << sock << "'s flag is " << recv_flag << std::endl;
        if (recv_flag == -1)
        {
            return false;
        }
        return true;
    }

    bool sendData(std::string data)
    {
        std::cout << std::endl << "send player " << id << std::endl << data << std::endl << std::endl;
        // std::cout << "now send data" << std::endl;

        int ret = send(sock, data.c_str(), data.size() + 1, 0);
        std::cout << "\tsend data ret = " << ret << std::endl;
        if (ret <= 0)
            return false;
        else
            return true;
    }

    std::string recvData()
    {
        int ret = recv(sock, recv_buffer, BUFFER_SIZE, 0);
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

    std::string get_id() const { return id; }
    int get_money() const { return money; }
};

