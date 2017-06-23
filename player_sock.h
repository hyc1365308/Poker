/************************************************
 * 名称 : room.h
 * 作者 : 冯瑜林
 * 时间 : 2017-05-12(1st)、2017-6-20(last)
 * 内容 : 房间类头文件
************************************************/

#ifndef PLAYER_SOCK_H
#define PLAYER_SOCK_H

#include <iostream>
#include <winsock2.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <string.h>

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
        // 初始化 recv_buffer
        memset(recv_buffer, 0, strlen(recv_buffer) * sizeof(char));

        int recv_flag = recv(sock, recv_buffer, BUFFER_SIZE, 0);
        
        int error = WSAGetLastError();
        if (recv_flag == -1 && error == WSAECONNRESET)
        {
            // 连接断开
            return false;
        }
        return true;
    }

    bool sendData(std::string data)
    {
        std::cout << std::endl << "Send " << id << std::endl << data;

        int ret = send(sock, data.c_str(), data.size() + 1, 0);
        std::cout << "\t  ret = " << ret << std::endl;
        if (ret <= 0)
            return false;
        else
            return true;
    }

    std::string recvData()
    {
        memset(recv_buffer, 0, strlen(recv_buffer) * sizeof(char));

        int ret = recv(sock, recv_buffer, BUFFER_SIZE, 0);
        if (ret > 0)
        {
            std::cout << std::endl << "Recv " << id << std::endl << recv_buffer;
            std::cout << "\t  ret = " << ret << std::endl;

            // for (int i = 0; i < ret; ++i)
            // {
            //     // 将所有空字符转换成换行符
            //     if (recv_buffer[i] == '\0')
            //         recv_buffer[i] = '\n';
            // }

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

#endif
