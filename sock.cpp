#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

enum SOCKET_STATE
{
    SEND,
    RECV,
    WAIT_SEND
};

class Socket
{
private:
    int id;
    SOCKET_STATE state;
    SOCKET sock;
    sockaddr_in sin;
    HANDLE thread;

public:
    Socket(SOCKET sock) : sock(sock)
    {
        thread = (HANDLE)_beginthreadex(NULL, 0, Fun, NULL, 0, NULL);
    }

    ~Socket();
    
};