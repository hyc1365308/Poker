#ifndef ROOM_H
#define ROOM_H

#include "player_sock.h"
#include "packet.h"
// #include "./game/Player.h"
#include "./game/Game.h"

// extern class Game;

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

    PlayerSock* operator[](const int i)
    {
        /*
         * get a player socket
        */
        return get_player(i);
    }

    int get_id() { return id; }

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
};

#endif
