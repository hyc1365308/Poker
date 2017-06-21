#ifndef ROOM_H
#define ROOM_H

#include "player_sock.h"
#include "packet.h"
#include "./game/Game.h"

class Room
{
private:
    static int next_id;
    const int id;
    pthread_t tid;
    // std::vector<SOCKET> players;
    std::vector<PlayerSock*> players;

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

    bool append(PlayerSock * player)
    {
        /*
         * add a new player
        */
        if (players.size() == MAX_PLAYER_NUM)
            return false;

        std::cout << "Room " << id << " join a new player" << std::endl;
        players.push_back(player);
        return true;
    }

    void remove(const PlayerSock * player)
    {
        /*
         * remove a player from this room
        */
        for (auto it = players.begin(); it != players.end(); ++it)
        {
            if (*it == player)
            {
                it = players.erase(it);
                return;
            }
        }
    }

    bool send(const int i, const std::string packet)
    {
        return players[i]->sendData(packet);
    }

    bool send(PlayerSock* sock, const std::string packet)
    {
        return sock->sendData(packet);
    }

    /*
     * get a operate from player
    */
    Json::Value getOperate(Player* player)
    {
        std::cout << "get operation now" << std::endl;
        PlayerSock * sock = get_player(player);
        std::cout << "now turn to player " << sock->get_id() << std::endl;
        Json::Value root;
        sock->sendData(Packet::requset());
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

    int get_id() { return id; }
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

    // std::vector<PlayerTuple> & getPlayers()
    // {
    //     std::vector<PlayerTuple> player_tuples;
    //     for (int i = 0; i < players.size(); ++i)
    //     {
    //         player_tuples.push_back(make_tuple(players[i]));
    //     }
    // }
};

#endif
