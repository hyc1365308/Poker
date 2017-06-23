#ifndef HALL_H
#define HALL_H

#include "player_sock.h"
#include "room.h"

#include <iostream>
#include <mutex>
#include <set>
#include <pthread.h>

class Hall
{
private:
    std::set<PlayerSock*> player_sets_;
    std::mutex mtx_;
    Room** rooms_;
    const int room_num_;

    friend void* testConnect(void*);
    friend void* waitEntry(void*);

public:
    Hall(Room** rooms, const int room_num) : rooms_(rooms), room_num_(room_num) { }

    ~Hall() {}
    
    bool insert(PlayerSock * new_player)
    {
        mtx_.lock();

        player_sets_.insert(new_player);

        mtx_.unlock();
    }

    int size() const { return player_sets_.size(); }

    void run();
};

#endif
