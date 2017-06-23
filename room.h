/************************************************
 * 名称 : room.h
 * 作者 : 冯瑜林
 * 时间 : 2017-05-13(1st)、2017-6-22(last)
 * 内容 : 房间类头文件
************************************************/

#ifndef ROOM_H
#define ROOM_H

#include "player_sock.h"
#include "packet.h"
#include "./game/Game.h"

#include "hall.h"

#include <mutex>

// 每个房间最大用户数
#define MAX_PLAYER_NUM 8
#define MIN_PLAYER_NUM 3

class Hall;

class Room
{
private:
    /* 类成员变量 */
    static int next_id;     // 下一个房间的编号，用于批量生成多个房间
    const int id_;          // 本房间编号
    pthread_t tid_;         // 本房间线程id
    bool run_now;           // 房间当前是否正在进行游戏
    // Server * server_;
    Hall* hall_;

    std::vector<PlayerSock*> players_;  // 本房间所有玩家
    std::mutex mtx_;                    // 本房间线程锁


    /* 类成员函数 */
    void init();        // 初始化

    // 根据玩家id获取其所在位置
    int getPlayerPos(const std::string player_id);

    // 返回此刻房间信息(根据不同的玩家返回不同的座次信息)
    std::vector<PlayerTuple> getPlayers(const int pos);
    

    /* 类友元函数 */
    friend void* runRoom(void*);        // 本房间线程函数

public:
    Room(const int room_id = next_id++) : id_(room_id)
    {
        std::cout << "Room " << id_ << " is created" << std::endl;
        init();
    }

    ~Room()
    {
        std::cout << "Room " << id_ << " is destroyed" << std::endl;
    }

    /*
     * 提供给Server类及Game类的接口
    */

    // 添加一个玩家
    bool append(PlayerSock * player);

    // 移除一个玩家
    void remove(const int pos);
    void remove(const PlayerSock * player);
    void removeAll();

    // 从指定玩家处得到一次操作
    Json::Value getOperate(Player* player, const int cur_max_money);

    // 广播某玩家的某次操作
    void castOperate(const Player* player, const int operation, const int money_left = 0, const int money_op = 0, bool is_blind = false);
    
    // 发送玩家个人的牌信息
    void licensePlayer(Player* player, Card & c);

    // 发送公共牌信息
    void licensePublic(int index, Card & c);

    // 向所有玩家展现最终结果
    // void showResult(Json::Value gameResult);
    void showResult(std::vector<std::tuple<int, Card, Card>>);


    /*
     * 一些简短的只用于获取room内部信息的内联函数
    */
    
    void set_hall(Hall* hall) { hall_ = hall; }

    // 获取编号
    int get_id() const { return id_; }
    // 获取当前用户数
    int get_num() const { return players_.size(); }
    // 获取玩家的id
    std::string getPlayerID(const int pos) { return players_[pos]->get_id(); }

    // 根据用户编号返回其对应的PlayerSock指针
    PlayerSock* operator[](const int i)
    {
        return get_player(i);
    }
    // 根据用户位置得到其PlayerSock指针
    PlayerSock* get_player(const int i) const
    {
        if (i >= players_.size() || i < 0)
        {
            return NULL;
        }
        return players_[i];
    }

    // 根据用户Player类型指针得到其PlayerSock指针
    PlayerSock* get_player(const Player* player) const 
    {
        std::string player_name = player->_name;
        for (int i = 0; i < players_.size(); ++i)
        {
            if (players_[i]->get_id() == player_name)
            {
                return players_[i];
            }
        }

        return NULL;
    }
};

#endif
