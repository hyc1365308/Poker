#include "room.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <map>
#include <vector>
#include <tuple>


const int ROOM_NUM = 4;

/*
 * 存储所有玩家信息的文件路径
 * 文件格式：
 *      玩家个数
 *      玩家1：id   密码  金钱数
 *      玩家2：id   密码  金钱数
 *      ……
*/
const char* player_file_path = "./data/player_info.txt";
const char* server_info_file_path = "./data/server_info.txt";

// 玩家信息，只包括密码与钱数
typedef std::tuple<std::string, int> PlayerInfo;   // password money

class Server
{
private:
    std::map<std::string, PlayerInfo> players;    // 所有玩家的信息，id作为键，money与密码为键值

    const int listen_port;      // 监听端口
    const int game_port;        // 游戏端口
    SOCKET listen_sock;         // 监听socket
    sockaddr_in sin;            // 本机地址

    std::vector<PlayerSock*> hall;      // 大厅内的玩家
    Room rooms[ROOM_NUM];       // 所有房间

    char recv_buffer[512];      // receive buffer
    char send_buffer[512];      // send buffer

    friend void* waitConnect(void*);        // 监听客户端连接函数
    friend void* testConnect(void*);        // 检测连接是否断开
    friend void* hallThread(void*);         // 大厅线程

    bool init()
    {
        return (initSocket() && loadPlayers(player_file_path));
    }

    bool initSocket();                          // 初始化socket连接
    bool loadPlayers(const char* file_name);    // 加载用户信息

    bool verifyUser(std::string & user_name, std::string & password);

    bool bindPort();

    bool listenClients();

    bool acceptConnect(SOCKET & sClient);

public:
    Server(const int listen_port = 8900, const int game_port = 8901) : listen_port(listen_port), game_port(game_port)
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
};

