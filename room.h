#ifndef ROOM_H
#define ROOM_H

#include "player_sock.h"
#include "packet.h"
#include "./game/Game.h"

#include <mutex>

const int MAX_PLAYER_NUM = 8;

class Room
{
private:
    static int next_id;
    const int id_;
    pthread_t tid_;
    // std::vector<SOCKET> players;
    std::vector<PlayerSock*> players;
    std::mutex mtx_;

    friend void* runRoom(void*);
    void init();
    void exit();

    bool run_now;   // 房间当前是否正在进行游戏

    // bool send(const int i, const std::string packet)
    // {
    //     return players[i]->sendData(packet);
    // }

    // bool send(PlayerSock* sock, const std::string packet)
    // {
    //     return sock->sendData(packet);
    // }

    int getPlayerPos(const std::string player_id)
    {
        for (int i = 0; i < players.size(); ++i)
        {
            if (player_id == players[i]->get_id())
            {
                return i;
            }
        }

        std::cout << "get player id_ worng" << std::endl;
        return -1;
    }

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

    bool append(PlayerSock * player)
    {
        /*
         * add a new player
        */
        if (players.size() >= MAX_PLAYER_NUM || run_now)
        {
            // 当前人员已满或者已经开始了游戏
            return false;
        }

        std::cout << "Room " << id_ << " join a new player" << std::endl;
        
        mtx_.lock();
        std::cout << "Room " << id_ << " push_back " << player->get_id() << std::endl;
        players.push_back(player);
        std::cout << "push_back done" << std::endl;
        mtx_.unlock();

        return true;
    }

    void remove(const PlayerSock * player)
    {
        /*
         * remove a player from this room
        */

        mtx_.lock();
        for (auto it = players.begin(); it != players.end(); ++it)
        {
            if (*it == player)
            {
                it = players.erase(it);
                break;
            }
        }
        mtx_.unlock();
    }

    /*
     * get a operate from player
    */
    Json::Value getOperate(Player* player, const int cur_max_money)
    {
        std::cout << "get operation now" << std::endl;
        PlayerSock * sock = get_player(player);
        std::cout << "now turn to player " << sock->get_id() << std::endl;
        Json::Value root;
        sock->sendData(Packet::requset(cur_max_money));
        while (true)
        {
            std::string packet_str = sock->recvData();
            // std::cout << "recv " << packet_str << std::endl;
            if (!Packet::decode(packet_str, root))
            {
                continue;
            }
            else
            {
                break;
            }
        }
        std::cout << "get operation done" << std::endl;

        return root;
    }

    /*
     * broadcast the player's operation
    */
    void castOperate(const Player* player, const int operation, const int money_left = 0, const int money_op = 0)
    {
        int player_pos = getPlayerPos(player->_name);

        std::string op_str = Packet::operate(player_pos, operation, money_left, money_op);

        for (int i = 0; i < players.size() && i != player_pos; ++i)
        {
            players[i]->sendData(op_str);
        }
    }

    /*
     * send license info to player
    */
    void licensePlayer(Player* player, Card & c)
    {
        std::cout << "send license info now" << std::endl;
        PlayerSock * sock = get_player(player);
        std::cout << "now send message " << sock->get_id() << std::endl;
        sock->sendData(Packet::licensePlayer(c.toInt()));
    }

    /*
     * send public license info to all player
    */
    void licensePublic(int index, Card & c)
    {
        std::cout << "send public license info now" << std::endl;
        std::string root = Packet::licensePublic(index, c.toInt());
        for (auto sock : players){
            sock->sendData(root);
        }
    }

    /*
     * send game result info to all player
    */
    void showResult(Json::Value gameResult)
    {
        std::cout << "send game result now" << std::endl;
        std::string root = Packet::showResult(gameResult);
        for (auto sock : players){
            sock->sendData(root);
        }
    }

    PlayerSock* operator[](const int i)
    {
        /*
         * get a player socket
        */
        return get_player(i);
    }

    int get_id() { return id_; }
    int get_num() { return players.size(); }

    PlayerSock* get_player(const int i) {
        if (i >= players.size() || i < 0)
        {
            return NULL;
        }
        return players[i];
    }

    PlayerSock* get_player(const Player* player)
    {
        std::string player_name = player->_name;
        // int player_num = -1;
        for (int i = 0; i < players.size(); ++i)
        {
            if (players[i]->get_id() == player_name)
            {
                // player_num = i;
                return players[i];
            }
        }

        return NULL;
    }

    std::vector<PlayerTuple> getPlayers(const PlayerSock* player_sock)
    {
        std::string player_id = player_sock->get_id();
        std::vector<PlayerTuple> player_tuples;
        for (int i = 0; i < players.size(); ++i)
        {
            bool is_in = (player_id == players[i]->get_id()) ? true : false;
            player_tuples.push_back(make_tuple(players[i]->get_id(), players[i]->get_money(), is_in));
        }

        return player_tuples;
    }
};

#endif
