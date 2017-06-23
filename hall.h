/************************************************
 * 名称 : hall.h
 * 作者 : 冯瑜林
 * 时间 : 2017-06-21(1st)
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
    std::set<PlayerSock*> player_sets_;     // 用户连接集合
    std::mutex mtx_;                        // 大厅内部数据锁
    Room ** rooms_;                         // 所有房间的指针
    const int room_num_;                    // 房间个数

    /* 友元函数 */
    friend void* testConnect(void*);    // 检测所有用户，看是否有连接已经断开
    friend void* waitEntry(void*);      // 等待用户发出进入房间的信息

public:
    // 初始化大厅，传入Room指针，房间个数
    Hall(Room ** rooms, const int room_num) : rooms_(rooms), room_num_(room_num) { }

    ~Hall() {}
    
    // 添加一个新用户
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

    // 开启两个大厅线程
    void run();
};

#endif
