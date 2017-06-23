/************************************************
 * 名称 : server.h
 * 作者 : 冯瑜林
 * 时间 : 2017-05-10(1st)、2017-6-22(last)
 * 内容 : 服务器实现文件
************************************************/

#include "server.h"

#include <mutex>

using namespace std;

void Server::init()
{
    /*
     * 功能 : 初始化所有服务器信息
    */

    initSocket();
    loadPlayers();

    // 创建所有房间
    for (int i = 0; i < ROOM_NUM; ++i)
    {
        rooms_[i] = new Room(this);
    }

    // 创建大厅
    hall_ = new Hall(rooms_, ROOM_NUM);

    // 设置每个房间中大厅指针的值
    for (int i = 0; i < ROOM_NUM; ++i)
    {
        rooms_[i]->set_hall(hall_);
    }
}

bool Server::verifyUser(std::string & user_name, std::string & password)
{
    /*
     * 功能 : 验证用户是否在数据库中
     * 参数 :
     *      user_name : 用户ID，string 类型
     *      password  : 用户密码，string 类型
     * 返回值 :
     *      一个int值,代表用户位置
    */

    cout << "verify user" << endl;
    cout << "\tusername: " <<user_name << endl;
    cout << "\tpassword: " << password << endl;
    cout << endl;

    if (players_.find(user_name) == players_.end())
    {
        cout << "user not in the name list" << endl;
        return false;
    }
    else
    {
        if (get<0>(players_[user_name]) != password)
        {
            cout << "password is incorrect" << endl;
            return false;
        }
    }

    cout << "verify succeed" << endl;

    return true;
}

bool Server::bindPort()
{
    /*
     * 功能 : 绑定端口，端口为listen_port
    */

    sin_.sin_family = AF_INET;
    sin_.sin_port = htons(listen_port_);
    sin_.sin_addr.S_un.S_addr = INADDR_ANY;
    if(bind(listen_sock_, (LPSOCKADDR)&sin_, sizeof(sin_)) == SOCKET_ERROR)
    {
        cout << "Bind port " << listen_port_ << " error!!!" << endl;
        return false;
    }
    return true;
}

bool Server::listenClients()
{
    /*
     * 功能 : 监听客户端连接
     * 返回值 :
     *      监听成功，返回true，否则返回false
    */

    if(listen(listen_sock_, 5) == SOCKET_ERROR)
    {
        cout << "Listen error !!!" << endl;
        return false;
    }

    return true;
}

bool Server::acceptConnect(SOCKET & sClient)
{
    /*
     * 功能 : 接收一个连接
     * 参数 :
     *      sClient : SOCKET引用，在函数中被修改
     * 返回值 :
     *      如果接收成功，返回true，否则false
    */

    sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);

    printf("\nWaiting for connecting...\r\n");
    sClient = accept(listen_sock_, (SOCKADDR *)&remoteAddr, &nAddrlen);

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
    /*
     * 功能 : 初始化server 监听socket
    */

    // 初始化接收缓存
    memset(recv_buffer_, '\0', 512);

    // 初始化 WSA
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        cout << "Init WSA incorrect!!!" << endl;
        return false;
    }

    // 创建socket
    listen_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_sock_ == INVALID_SOCKET)
    {
        cout << "Create socket error!!!" << endl;
        return false;
    }

    return true;
}

bool Server::loadPlayers()
{
    /*
     * 功能 : 加载用户信息
    */

    std::ifstream fin(player_file_path);

    // 输入玩家个数
    int num;
    fin >> num;
    int money;
    std::string id, password;
    cout << endl << "Read players data" << endl;
    printf("Num     id     password     money\n");
    for (int i = 0; i < num; ++i)
    {
        fin >> id >> password >> money;
        players_[id] = make_tuple(password, money);
        printf("%3d %8s %12s %7d\n", i, id.c_str(), password.c_str(), money);
    }
    fin.close();

    return true;
}

void Server::updatePlayerInfo(std::map<std::string, int> update_dict)
{
    /*
     * 功能 : 更新所有用户信息(更新到文件中)
     * 参数 :
     *      update_dict : <用户ID : 对应的money值>
    */

    for (auto it : update_dict)
    {
        int money = it.second;

        // 获得原来过时的信息
        PlayerInfo old_info = players_[it.first];

        // 构造新的信息
        string password = std::get<0>(old_info);
        PlayerInfo new_info = make_tuple(password, money);

        // 更新信息
        players_[it.first] = new_info;
    }

    std::ofstream fout(player_file_path);
    // 输出用户个数
    fout << players_.size() << endl;

    // 输出每个用户的信息
    for (auto it : players_)
    {
        PlayerInfo player = it.second;

        string password = get<0>(player);
        int money = get<1>(player);

        fout << it.first << "  " << password << "  " << money << endl;
    }

    fout.close();
}

bool setTestAlive(const int sockfd)
{
    /*
     * 功能 : 设置 socket 的 test alive 选项
     * 参数 :
     *      sockfd : 要设置的socket值
    */

    struct TCP_KEEPALIVE {  
        u_long onoff;  
        u_long keepalivetime;  
        u_long keepaliveinterval;  
    };

    // KeepAlive实现
    TCP_KEEPALIVE inKeepAlive = {0};                //输入参数  
    unsigned long ulInLen = sizeof(TCP_KEEPALIVE);  
    TCP_KEEPALIVE outKeepAlive = {0};               //输出参数  
    unsigned long ulOutLen = sizeof(TCP_KEEPALIVE);  
    unsigned long ulBytesReturn = 0;

    // 设置socket的keep alive为5秒，发送次数为3次  
    inKeepAlive.onoff = 1;
    // 两次KeepAlive探测间的时间间隔
    inKeepAlive.keepaliveinterval = 5000;
    //开始首次KeepAlive探测前的TCP空闭时间
    inKeepAlive.keepalivetime     = 5000;
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
}

void Server::waitConnect()
{
    /*
     * 功能 : 等待新用户连接(循环等待)
    */

    while (true)
    {
        SOCKET csock;
        if (!acceptConnect(csock))
        {
            // 建立连接出错
            Sleep(10);      // 等待10ms
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

        memset(recv_buffer_, 0, strlen(recv_buffer_) * sizeof(char));
        int ret = recv(csock, recv_buffer_, 512, 0);
        if(ret > 0)
        {
            recv_buffer_[ret] = 0x00;
            cout << "Recv : " << recv_buffer_ << endl;
        }

        // 验证客户端是否合法
        Json::Value root;
        if(!Packet::decode(recv_buffer_, root))
        {
            cout << "Get a bad packet, return" << endl;
            cout << "Packet content:" << endl;
            cout << recv_buffer_ << endl;
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

        // 验证用户ID与密码
        if (!verifyUser(user_name, password))
        {
            PlayerSock _player(csock, user_name, 0);
            _player.sendData(Packet::rLogin(false));
            continue;
        }

        // 设置TestAlive选项
        if (!setTestAlive(csock))
        {
            cout << "set csock test alive wrong" << endl;
        }

        int money = get<1>(players_[user_name]);
        PlayerSock * new_player = new PlayerSock(csock, user_name, money);

        bool flag = new_player->sendData(Packet::rLogin(true, money));
        if (!flag)
        {
            cout << "cannot send login result" << endl;
        }
        
        // 向大厅中添加新用户
        hall_->insert(new_player);
    }
}

void Server::run()
{
    // 启动大厅hall中的两个线程
    hall_->run();

    // 执行等待连接的函数
    waitConnect();
}

int main(int argc, char* argv[])
{
    Server server;
    server.run();
}