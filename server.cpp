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
}

void Server::waitConnect()
{
    while (true)
    {
        SOCKET csock;
        if (!acceptConnect(csock))
        {
            // accept connection wrong
            Sleep(10);      // 等待一段时间
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
        if (!verifyUser(user_name, password))
        {
            PlayerSock _player(csock, user_name, 0);
            _player.sendData(Packet::rLogin(false));
            continue;
        }

        if (!setTestAlive(csock))
        {
            cout << "set csock test alive wrong" << endl;
        }

        int money = get<1>(players[user_name]);
        // cout << "client sock = " << csock << endl;
        PlayerSock * new_player = new PlayerSock(csock, user_name, money);

        bool flag = new_player->sendData(Packet::rLogin(true, money));
        // new_player->sendData("just for test\n");
        if (!flag)
        {
            cout << "cannot send login result" << endl;
        }
        
        hall->insert(new_player);

        std::cout << "Now hall has " << hall->size() << " players" << std::endl;
        // server->rooms[0]->append(csock);
    }
}

void Server::run()
{
    // 启动大厅hall中的两个线程
    hall->run();

    // 执行等待连接的函数
    waitConnect();
}

int main(int argc, char* argv[])
{
    Server server("./data/player_info.txt");
    server.run();
}