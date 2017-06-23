/************************************************
 * 名称 : hall.h
 * 作者 : 冯瑜林
 * 时间 : 2017-07-03(1st)
 * 内容 : 大厅头文件
************************************************/

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
    Room ** rooms_;
    const int room_num_;

    friend void* testConnect(void*);
    friend void* waitEntry(void*);

public:
    Hall(Room ** rooms, const int room_num) : rooms_(rooms), room_num_(room_num) { }

    ~Hall() {}
    
    bool insert(PlayerSock * new_player)
    {
        mtx_.lock();
        std::cout << "insert a new player into the hall\n";
        std::cout << "player " << new_player << std::endl;
        std::cout << "now hall has " << player_sets_.size() << " players\n";
        player_sets_.insert(new_player);
        mtx_.unlock();
    }

    int size() const { return player_sets_.size(); }

    void run();
};

#endif
