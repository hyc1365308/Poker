/*
 * Author   :   Feng Yulin
 * Time     :   2017-05-04(1st), 17-05-10(2nd), 17-5-22(3rd)
 * Function :   Server class
*/

#include "server.h"

#include <mutex>

using namespace std;

mutex mtx;

bool Server::verifyUser(std::string & user_name, std::string & password)
{
    /*
     * 验证用户是否在数据库中
    */

    cout << "verify user" << endl;
    cout << "\tusername: " <<user_name << endl;
    cout << "\tpassword: " << password << endl;
    cout << endl;

    if (players.find(user_name) == players.end())
    {
        cout << "user not in the name list" << endl;
        cout << "user name: " << user_name << endl;
        return false;
    }
    else
    {
        if (get<0>(players[user_name]) != password)
        {
            cout << "password is incorrect" << endl;
            cout << "id: " << user_name << ", pass: " << password << endl;
            return false;
        }
    }

    return true;
}

bool Server::bindPort()
{
    /*
     * 绑定端口，端口为listen_port
    */
    sin.sin_family = AF_INET;
    sin.sin_port = htons(listen_port);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if(bind(listen_sock, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        cout << "Bind port " << listen_port << " error!!!" << endl;
        return false;
    }
    return true;
}

bool Server::listenClients()
{
    if(listen(listen_sock, 5) == SOCKET_ERROR)
    {
        cout << "Listen error !!!" << endl;
        return false;
    }

    return true;
}

bool Server::acceptConnect(SOCKET & sClient)
{
    /*
     * 接收一个连接
    */

    sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);

    printf("\nWaiting for connecting...\r\n");
    sClient = accept(listen_sock, (SOCKADDR *)&remoteAddr, &nAddrlen);

    if(sClient == INVALID_SOCKET)
    {
        printf("Accept error !\r\n");
        return false;
    }

    printf("Connect a client %s:%d\r\n", inet_ntoa(remoteAddr.sin_addr), remoteAddr.sin_port);
    
    return true;
}

bool Server::initSocket()
{
    // init buffer
    memset(recv_buffer, '\0', 512);
    memset(send_buffer, '\0', 512);

    // init WSA
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        cout << "Init WSA incorrect!!!" << endl;
        return false;
    }

    // create socket
    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_sock == INVALID_SOCKET)
    {
        cout << "Create socket error!!!" << endl;
        return false;
    }

    return true;
}

bool Server::loadPlayers()
{
    std::ifstream fin(file_name);
    int num;        // 玩家个数
    fin >> num;
    std::string id, password;
    int money;
    cout << endl << "Read players data" << endl;
    printf("Num     id     password     money\n");
    for (int i = 0; i < num; ++i)
    {
        fin >> id >> password >> money;
        players[id] = make_tuple(password, money);
        printf("%3d %8s %12s %7d\n", i, id.c_str(), password.c_str(), money);
    }
    fin.close();

    return true;
}

void Server::updatePlayerInfo(std::map<std::string, int> update_dict)
{
    std::ofstream fout(file_name);
    fout << players.size() << endl;

    for (auto it : update_dict)
    {
        int money = it.second;

        // 获得原来过时的信息
        PlayerInfo old_info = players[it.first];

        // 构造新的信息
        string password = std::get<0>(old_info);
        PlayerInfo new_info = make_tuple(password, money);

        // 更新信息
        players[it.first] = new_info;

        fout << it.first << "  " << password << "  " << money;
    }

    fout.close();
}

bool setTestAlive(const int sockfd)
{
    struct TCP_KEEPALIVE {  
        u_long onoff;  
        u_long keepalivetime;  
        u_long keepaliveinterval;  
    };

    //KeepAlive实现  
    TCP_KEEPALIVE inKeepAlive = {0}; //输入参数  
    unsigned long ulInLen = sizeof(TCP_KEEPALIVE);  
    TCP_KEEPALIVE outKeepAlive = {0}; //输出参数  
    unsigned long ulOutLen = sizeof(TCP_KEEPALIVE);  
    unsigned long ulBytesReturn = 0;

    //设置socket的keep alive为5秒，并且发送次数为3次  
    inKeepAlive.onoff = 1;
    inKeepAlive.keepaliveinterval = 5000; //两次KeepAlive探测间的时间间隔
    inKeepAlive.keepalivetime     = 5000; //开始首次KeepAlive探测前的TCP空闭时间
    if (
        WSAIoctl((unsigned int)sockfd,  
        _WSAIOW(IOC_VENDOR,4),
        (LPVOID)&inKeepAlive, ulInLen,
        (LPVOID)&outKeepAlive, ulOutLen,  
        &ulBytesReturn, NULL, NULL) == SOCKET_ERROR
    )
    {
        perror("fail to set test_alive");
        return false;
    }

    return true;
    // const int keepalive = 1;      // 打开探测
    // const int keepidle  = 60;     // 开始探测前的空闲等待时间
    // const int keepintvl = 10;     // 发送探测分节的时间间隔
    // const int keepcnt   = 3;      // 发送探测分节的次数

    // if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&keepalive, sizeof (keepalive)) < 0)
    // {
    //     perror("fail to set SO_KEEPALIVE");
    //     exit(-1);
    // }
    // if (setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, (char *) &keepidle, sizeof (keepidle)) < 0)
    // {
    //     perror("fail to set SO_KEEPIDLE");
    //     exit(-1);
    // }
    // if (setsockopt(sockfd, SOL_TCP, TCP_KEEPINTVL, (char *)&keepintvl, sizeof (keepintvl)) < 0)
    // {
    //     perror("fail to set SO_KEEPINTVL");
    //     exit(-1);
    // }
    // if (setsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, (char *)&keepcnt, sizeof (keepcnt)) < 0)
    // {
    //     perror("fail to set SO_KEEPALIVE");
    //     exit(-1);
    // }
}

void* waitConnect(void* arg)
{
    Server* server = (Server*) arg;
    char recv_buffer[512];
    char send_buffer[512];
    while (true)
    {
        SOCKET csock;
        if (!server->acceptConnect(csock))
        {
            // accept connection wrong
            Sleep(10);
            continue;
        }

        // 设置csock为非阻塞模式
        unsigned long ul = 1;
        int flags = ioctlsocket(csock, FIONBIO, (unsigned long *)&ul);
        if(flags == SOCKET_ERROR)
        {
            cout << "Set FIONBIO failed" << endl;
            closesocket(csock);
            continue;
        }

        memset(recv_buffer, 0, strlen(recv_buffer) * sizeof(char));
        int ret = recv(csock, recv_buffer, 512, 0);
        if(ret > 0)
        {
            recv_buffer[ret] = 0x00;
            cout << "Recv : " << recv_buffer << endl;
        }

        // verify the client
        Json::Value root;
        if(!Packet::decode(recv_buffer, root))
        {
            cout << "Get a bad packet, return" << endl;
            cout << "Packet content:" << endl;
            cout << recv_buffer << endl;
            cout << "...\n";
            continue;
        }

        std::string user_name = "";
        std::string password  = "";
        if (!root["username"].isNull())
        {
            user_name = root["username"].asString();
        }
        
        if (!root["password"].isNull())
        {
            password  = root["password"].asString();
        }

        // verify players name & password
        if (!server->verifyUser(user_name, password))
        {
            PlayerSock _player(csock, user_name, 0);
            _player.sendData(Packet::rLogin(false));
            continue;
        }

        if (!setTestAlive(csock))
        {
            cout << "set csock test alive wrong" << endl;
        }

        int money = get<1>(server->players[user_name]);
        // cout << "client sock = " << csock << endl;
        PlayerSock * new_player = new PlayerSock(csock, user_name, money);

        bool flag = new_player->sendData(Packet::rLogin(true, money));
        // new_player->sendData("just for test\n");
        if (!flag)
        {
            cout << "cannot send login result" << endl;
        }
        
        mtx.lock();
        server->hall.insert(new_player);
        mtx.unlock();

        std::cout << "Now hall has " << server->hall.size() << " players" << std::endl;
        // server->rooms[0]->append(csock);
    }
}

void* hallThread(void* arg)
{
    Server* server = (Server*) arg;

    while (true)
    {
        for (auto it = server->hall.begin(); it != server->hall.end(); ++it)
        {
            // cout << "check sock " << (*it) << "'s entry" << endl;
            std::string packet = (*it)->recvData();
            if (packet != "")
            {
                cout << "sock " << *it << " get a packet" << endl;
                Json::Value root;
                if (Packet::decode(packet, root) && root["type"] == ENTRY)
                {
                    int room_id = root["room"].asInt();

                    if (room_id < 0 || room_id >= ROOM_NUM)
                    {
                        cout << "cannot find room " << room_id << endl;
                        continue;
                    }

                    // 向房间添加新的成员
                    bool flag = server->rooms[room_id]->append(*it);
                    
                    PlayerSock* old_sock = (*it);

                    if (flag)
                    {
                        mtx.lock();
                        cout << "erase " << (*it) << endl;
                        it = server->hall.erase(it);
                        cout << "erase done\n";
                        mtx.unlock();

                        std::cout << "Now hall has " << server->hall.size() << " member" << std::endl;
                    }
                }
            }
        }

        Sleep(100);  // sleep
    }
}

void* testConnect(void* arg)
{
    // test all hall's socket connections every 5 seconds
    // if a socket is closed, remove it from the hall
    Server* server = (Server*) arg;
    while(true)
    {

        for (auto it = server->hall.begin(); it != server->hall.end(); )
        {
            // sock = *it
            if ((*it)->testConnect())
            {
                cout << "test connect " << *it << " " << endl;
                ++it;
            }
            else
            {
                // sock has been closed
                mtx.lock();
                cout << "socket " << *it << " is detected closed" << endl;
                delete (*it);
                it = server->hall.erase(it);
                mtx.unlock();
            }
        }

        // test all sockets connection every 10 seconds
        sleep(10);
    }
}

void Server::run()
{
    // create test connection thread
    pthread_t test_tid;
    pthread_create(&test_tid, NULL, testConnect, this);

    // create wait connection thread
    pthread_t wait_tid;
    pthread_create(&wait_tid, NULL, waitConnect, this);

    pthread_t hall_tid;
    pthread_create(&hall_tid, NULL, hallThread, this);

    void* tret;
    pthread_join(wait_tid, &tret);
    pthread_join(hall_tid, &tret);
    pthread_join(test_tid, &tret);
}

int main(int argc, char* argv[])
{
    Server server("./data/player_info.txt");
    server.run();
}