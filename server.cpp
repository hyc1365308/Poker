/*
 * Author   :   Feng Yulin
 * Time     :   2017-05-04(1st), 17-05-10(2nd), 17-5-22(3rd)
 * Function :   Server class
*/

#include "room.h"

#include <stdio.h>

using std::cout;
using std::endl;

class PlayerSock
{
private:
    std::string id;             // user id(user name)
    sockaddr_in addr;           // address, in case of need
    const SOCKET socket;

    char recv_buffer[512];      // receive buffer
    char send_buffer[512];      // send buffer

public:
    PlayerSock(const SOCKET sock) : socket(sock)
    {}

    ~PlayerSock()
    {
        closesocket(socket);
    }

    bool testConnect()
    {
        char test_buffer[] = "test alive\r\n";
        if (send(socket, test_buffer, strlen(test_buffer), 0) <= 0)
            // sock has been closed
            return false;
        else
            return true;
    }

    bool sendData()
    {

    }

    bool recvData()
    {

    }
};

class Server
{
private:
    static const int ROOM_NUM = 4;

    const int listen_port;
    const int game_port;
    SOCKET listen_sock;
    sockaddr_in sin;

    std::vector<SOCKET> hall;
    Room rooms[ROOM_NUM];       // ROOM_NUM rooms

    char recv_buffer[512];      // receive buffer
    char send_buffer[512];      // send buffer

    friend void* waitConnect(void*);
    friend void* testConnect(void*);
    bool init();

public:
    Server(const int listen_port = 8900, const int game_port = 8901) : listen_port(listen_port), game_port(game_port)
    {
        init();
        bindPort();
        listenClients();
        run();
    }

    ~Server()
    {
        close();
    }

    bool bindPort()
    {
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

    bool listenClients()
    {
        if(listen(listen_sock, 5) == SOCKET_ERROR)
        {
            cout << "Listen error !!!" << endl;
            return false;
        }
        return true;
    }

    bool acceptConnect(SOCKET & sClient)
    {
        // SOCKET sClient;
        sockaddr_in remoteAddr;
        int nAddrlen = sizeof(remoteAddr);

        printf("Waiting for connecting...\r\n");
        sClient = accept(listen_sock, (SOCKADDR *)&remoteAddr, &nAddrlen);

        if(sClient == INVALID_SOCKET)
        {
            printf("Accept error !\r\n");
            return false;
        }
        printf("Connect a client %s:%d\r\n", inet_ntoa(remoteAddr.sin_addr), remoteAddr.sin_port);
        return true;
    }

    /*bool sendData()
    {

    }*/

    void close()
    {
        closesocket(listen_sock);
        WSACleanup();
    }

    void run();
};

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
            continue;
        }

        unsigned long ul = 1;
        int flags = ioctlsocket(csock, FIONBIO, (unsigned long *)&ul);
        if(flags == SOCKET_ERROR)
        {
            cout << "Set FIONBIO failed" << endl;
            closesocket(csock);
            continue;
        }

        int ret = recv(csock, recv_buffer, 512, 0);
        if(ret > 0)
        {
            recv_buffer[ret] = 0x00;
            cout << "Recv : " << recv_buffer << endl;
        }

        // verify the client


        // send data
        strcpy(send_buffer, "Hello, I'm a server!\n");
        send(csock, send_buffer, strlen(send_buffer), 0);
        memset(send_buffer, '\0', strlen(send_buffer));
        // strcpy(send_buffer, )
        // closesocket(csock);
        server->hall.push_back(csock);
        // server->rooms[0]->append(csock);
    }
}

void* testConnect(void* arg)
{
    // test all hall's socket connections every 5 seconds
    // if a socket is closed, remove it from the hall
    Server* server = (Server*) arg;
    char test_buffer[] = "test alive\r\n";
    while(true)
    {
        for (auto it = server->hall.begin(); it != server->hall.end(); )
        {
            // sock = *it
            if (send(*it, test_buffer, strlen(test_buffer), 0) <= 0)
            {
                // sock has been closed
                it = server->hall.erase(it);
            }
            else
            {
                cout << *it << " " << endl;
                ++it;
            }
        }
        cout << "Now hall has " << server->hall.size() << " member" << endl;
        cout << endl;

        // test all sockets connection every 5 seconds
        sleep(5);
    }
}

bool Server::init()
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

void Server::run()
{
    // create test connection thread
    pthread_t test_tid;
    pthread_create(&test_tid, NULL, testConnect, this);

    // create wait connection thread
    pthread_t wait_tid;
    pthread_create(&wait_tid, NULL, waitConnect, this);

    void* tret;
    pthread_join(wait_tid, &tret);
    pthread_join(test_tid, &tret);
}

int main(int argc, char* argv[])
{
    Server server;
}