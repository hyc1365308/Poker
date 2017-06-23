#ifndef HALL_H
#define HALL_H

#include "player_sock.h"
#include "room.h"

#include <iostream>
#include <mutex>

class Room;

class Hall
{
private:
    std::set<PlayerSock*> player_sets_;
    mutex mtx_;

public:
    Hall();
    ~Hall();
    
    bool insert(PlayerSock * new_player)
    {
        mtx_.lock();

        player_sets_.insert(new_player);

        mtx_.unlock();
    }

    friend void* testConnect(void*);
    friend void* waitEntry(void*);

    int size() const { return player_sets_.size(); }
};

#endif
