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


const int ROOM_NUM = 6;

/*
 * 存储所有玩家信息的文件路径
 * 文件格式：
 *      玩家个数
 *      玩家1：id   密码  金钱数
 *      玩家2：id   密码  金钱数
 *      ……
*/
const std::string player_file_path = "./data/player_info.txt";
const char* server_info_file_path = "./data/server_info.txt";

// 玩家信息，只包括密码与钱数
typedef std::tuple<std::string, int> PlayerInfo;   // password money

class Server
{
private:
    const std::string file_name;

    std::map<std::string, PlayerInfo> players;    // 所有玩家的信息，id作为键，money与密码为键值

    const int listen_port;      // 监听端口
    SOCKET listen_sock;         // 监听socket
    sockaddr_in sin;            // 本机地址

    // std::set<PlayerSock*> hall;
    Room* rooms[ROOM_NUM];      // 所有房间

    char recv_buffer[512];      // receive buffer
    char send_buffer[512];      // send buffer

    void init()
    {
        initSocket();
        loadPlayers();
        for (int i = 0; i < ROOM_NUM; ++i)
        {
            rooms[i] = new Room();
        }

        hall = new Hall(rooms, ROOM_NUM);

        for (int i = 0; i < ROOM_NUM; ++i)
        {
            rooms[i]->set_hall(hall);
        }
    }

    bool initSocket();     // 初始化socket连接
    bool loadPlayers();    // 加载用户信息

    bool verifyUser(std::string & user_name, std::string & password);

    bool bindPort();

    bool listenClients();

    bool acceptConnect(SOCKET & sClient);

    void waitConnect();

    Hall* hall;

    // /*
    //  * 类友元函数
    // */
    // friend void* waitConnect(void*);        // 监听客户端连接函数
    // friend void* testConnect(void*);        // 检测连接是否断开
    // friend void* hallThread(void*);         // 大厅线程

public:
    Server(const string & file_name = player_file_path, const int listen_port = 8900) : file_name(file_name), listen_port(listen_port)
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
        closesocket(listen_sock);
        WSACleanup();
    }

    void run();
    void updatePlayerInfo(std::map<std::string, int>);
};

#endif
