#include <iostream>

#define BUFFER_SIZE 512

class Client
{
private:
    SOCKET sock;
    const int server_port;
    const std::string server_ip;

    char recv_buffer[BUFFER_SIZE];
    void init();

    bool sendData(std::string && data);
    Json::Value recvData();

public:
    Client(const SOCKET port, const std::string & ip)
        : server_port(port), server_ip(ip)
    {
        init();
    }

    ~Client()
    {
        closesocket(sock);
        WSACleanup();
    }
    
    bool connectServer();

    void runClient();

    bool login(const std::string & user_name, const std::string & password);
    bool entry(const int room_id);
};
