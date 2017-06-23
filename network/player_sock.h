/************************************************
 * 名称 : room.h
 * 作者 : 冯瑜林
 * 时间 : 2017-05-12(1st)、2017-6-20(last)
 * 内容 : 用户连接类
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

// 缓冲区大小
#define BUFFER_SIZE 512

class PlayerSock
{
    /*
     * 该类为用户连接类，主要作用是管理与用户的连接
     *     所有与用户的通信及其他相关功能都是通过该类完成的
    */
private:
    std::string id;             // 用户ID
    int money;                  // 用户当前金额(每轮游戏后更新)
    const SOCKET sock;          // 用户的socket连接值

    char recv_buffer[BUFFER_SIZE];      // 接收缓冲区

public:
    PlayerSock(const SOCKET sock, const std::string & id, const int money) : sock(sock), id(id), money(money)
    {
        std::cout << "Player " << id << ", sock(" << sock << ") login now" << std::endl;
    }

    ~PlayerSock()
    {
        std::cout << "Player " << id << " quit now" << std::endl;
        closesocket(sock);
    }

    bool testConnect()
    {
        /*
         * 功能 : 检测sock连接是否断开，如果断开返回false
         * 返回值 : 如果连接断开返回false，否则返回true
        */

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
        /*
         * 功能 : 使用sock发送数据
         * 参数 : 要发送的数据(string类型)
         * 返回值 : 成功返回true，否则false
        */

        std::cout << std::endl << "Send " << id << std::endl << data;

        // 发送数据
        int ret = send(sock, data.c_str(), data.size() + 1, 0);

        std::cout << "\t  ret = " << ret << std::endl;
        
        if (ret <= 0)
            return false;
        else
            return true;
    }

    std::string recvData()
    {
        /*
         * 功能 : 使用sock接收数据
         * 返回值 : 接收到的数据(string类型)
        */

        // 清空缓冲区
        memset(recv_buffer, 0, strlen(recv_buffer) * sizeof(char));

        int ret = recv(sock, recv_buffer, BUFFER_SIZE, 0);
        if (ret > 0)
        {
            // 打印接收到的数据
            std::cout << std::endl << "Recv " << id << std::endl << recv_buffer;
            std::cout << "\t  ret = " << ret << std::endl;

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
    void set_money(const int money) { this->money = money; }
};

#endif
