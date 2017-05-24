#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <winsock2.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <pthread.h>

class Room
{
private:
    static int next_id;
    const int id;
    pthread_t tid;
    std::vector<SOCKET> players;

    friend void* runRoom(void*);
    void init();
    void exit();

public:
    Room(const int id = next_id++) : id(id)
    {
        std::cout << "Room " << id << " is created" << std::endl;
        init();
    }
    ~Room()
    {
        std::cout << "Room " << id << " is destroyed" << std::endl;
    }

    void append(const SOCKET sock)
    {
        players.push_back(sock);
    }

    void remove(const SOCKET sock)
    {
        for (auto it = players.begin(); it != players.end(); ++it)
        {
            if (*it == sock)
            {
                players.erase(it);
                return;
            }
        }
    }
};

#endif
