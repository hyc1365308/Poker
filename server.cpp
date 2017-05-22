/*
 * Author   :   Feng Yulin
 * Time     :   2017-05-04(1st)
 * Function :   Server class
*/

#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <vector>
#include <string>

using std::cout;
using std::endl;

class CliSock
{
private:
    const SOCKET socket;
    sockaddr_in addr;           // address, in case of need
    std::string id;             // user id(user name)

    char recv_buffer[512];      // receive buffer
    char send_buffer[512];      // send buffer

public:
    CliSock(const SOCKET sock) : socket(sock)
    {}

    ~CliSock()
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
    static const int ROOM_NUM = 16;

    const int listen_port;
    const int game_port;
    SOCKET listen_sock;
    std::vector<CliSock> game_sock;
    sockaddr_in sin;

    std::vector<SOCKET> hall;
    std::vector<SOCKET> rooms[ROOM_NUM];

    char recv_buffer[512];      // receive buffer
    char send_buffer[512];      // send buffer

public:
    Server(const int listen_port = 8900, const int game_port = 8901) : listen_port(listen_port), game_port(game_port)
    {
        init();
        bindPort();
        listenClients();
    }

    ~Server()
    {
        close();
    }
    
    bool init()
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

        printf("Waiting for connecting...\n");
        sClient = accept(listen_sock, (SOCKADDR *)&remoteAddr, &nAddrlen);

        if(sClient == INVALID_SOCKET)
        {
            printf("accept error !");
            return false;
        }
        printf("Connect a client %s:%d.\r\n", inet_ntoa(remoteAddr.sin_addr), remoteAddr.sin_port);
        return true;
    }

    bool sendData()
    {

    }

    void close()
    {
        closesocket(listen_sock);
        WSACleanup();
    }

    void run()
    {
        while (true)
        {
            SOCKET csock;
            acceptConnect(csock);
            int ret = recv(csock, recv_buffer, 512, 0);        
            if(ret > 0)
            {
                recv_buffer[ret] = 0x00;
                printf(recv_buffer);
            }

            // we should verify the client


            // send data
            strcpy(send_buffer, "Hello, I'm a server!\n");
            send(csock, send_buffer, strlen(send_buffer), 0);
            memset(send_buffer, '\0', strlen(send_buffer));
            // strcpy(send_buffer, )
            // closesocket(csock);
            hall.push_back(csock);

            for (auto it = hall.begin(); it != hall.end(); )
            {
                // sock = *it
                strcpy(send_buffer, "keep alive\r\n");
                if (send(*it, send_buffer, strlen(send_buffer), 0) <= 0)
                {
                    // sock has been closed
                    it = hall.erase(it);
                }
                else
                {
                    cout << *it << " " << endl;
                    ++it;
                }
            }
            cout << "Now hall has " << hall.size() << " member" << endl;
            cout << endl;
        }
        // close();
    }
};

int main(int argc, char* argv[])
{
    Server server;
    server.run();
}