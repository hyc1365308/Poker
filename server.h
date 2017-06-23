/************************************************
 * 名称 : server.h
 * 作者 : 冯瑜林
 * 时间 : 2017-05-10(1st)、2017-6-22(last)
 * 内容 : 服务器头文件
************************************************/

#ifndef SERVER_H
#define SERVER_H

#include "room.h"
#include "hall.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <map>
#include <vector>
#include <set>
#include <tuple>


const int ROOM_NUM = 11;

/*
 * 存储所有玩家信息的文件路径
 * 文件格式：
 *      玩家个数
 *      玩家1：id   密码  金钱数
 *      玩家2：id   密码  金钱数
 *      ……
*/
// 玩家信息，只包括密码与钱数
typedef std::tuple<std::string, int> PlayerInfo;   // password money

class Server
{
private:
    /* 私有成员变量 */
    const std::string player_file_path = "./data/player_info.txt";
    const char* server_info_file_path = "./data/server_info.txt";

    std::map<std::string, PlayerInfo> players_;    // 所有玩家的信息，id作为键，money与密码为键值

    const int listen_port_;     // 监听端口
    SOCKET listen_sock_;        // 监听socket
    sockaddr_in sin_;           // 本机地址
    char recv_buffer_[512];     // receive buffer

    Room* rooms_[ROOM_NUM];     // 所有房间的指针
    Hall* hall_;                // 大厅指针

    /* 私有成员函数 */
    bool initSocket();          // 初始化socket连接
    bool loadPlayers();         // 加载用户信息
    void init();                // 初始化所有服务器信息

    bool verifyUser(std::string & user_name, std::string & password);

    bool bindPort();

    bool listenClients();

    bool acceptConnect(SOCKET & sClient);

    void waitConnect();


public:
    Server(const int listen_port = 8900) : listen_port_(listen_port)
    {
        init();
        bindPort();
        listenClients();
        std::cout << "server is running now" << std::endl;
    }

    ~Server()
    {
        close();
        std::cout << "server is closed now" << std::endl;
    }


    void close()
    {
        closesocket(listen_sock_);
        WSACleanup();
    }

    void run();
    void updatePlayerInfo(std::map<std::string, int>);
};

#endif
