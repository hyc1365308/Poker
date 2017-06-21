#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <vector>
#include <tuple>

#include "json/json.h"

#define SUCCEED 1
#define FAILURE 0

typedef std::tuple<int, int> PlayerTuple;

enum Type
{
    INVALID,    // invalid packet
    TEST_ALIVE, // test alive
    LOGIN,
    ENTRY,
    LEAVE,
    PREPARE,
    DIS_PREPARE,
    BEGIN,
    REFUEL,
    CALL,
    FOLD,
    ALLIN,
    REQUSET,
    LOGIN_RESULT,
    ENTRY_RESULT,
    PREPARE_RESULT,
    DIS_PREPARE_RESULT,
    BEGIN_GAME,
    HALL,
    ROOM,

    LICENSE_PLAYER,
    LICENSE_PUBLIC,
    GAME_RESULT,
};

class Packet
{
public:
    /*
     * decode packet
    */
    static bool decode(std::string packet, Json::Value & root)
    {
        Json::Reader reader;
        // Json::Value root;
        if (reader.parse(packet, root))
        {
            if (root["type"].isNull() || root["type"] == INVALID)
            {
                return false;
            }
            return true;
        }
        return false;
    }

    
    /*
     * client packet
    */
    static std::string login(const std::string & username, const std::string & password)
    {
        /*
         * client login
        */
        Json::Value root;
        root["type"] = LOGIN;
        root["username"] = username;
        root["password"] = password;
        return root.toStyledString();
    }

    static std::string entry(const int room_num)
    {
        /*
         * client enter the room
        */
        Json::Value root;
        root["type"] = ENTRY;
        root["room"] = room_num;
        return root.toStyledString();
    }

    static std::string leave(const int room_num)
    {
        /*
         * client leave the room
        */
        Json::Value root;
        root["type"] = LEAVE;
        root["room"] = room_num;
        return root.toStyledString();
    }

    static std::string prepare()
    {
        /*
         * client prepare to begin the game
        */
        Json::Value root;
        root["type"] = PREPARE;
        return root.toStyledString();
    }

    static std::string disPrepare()
    {
        /*
         * client cancle the preparation
        */
        Json::Value root;
        root["type"] = DIS_PREPARE;
        return root.toStyledString();
    }

    static std::string begin()
    {
        /*
         * begin game
        */
        Json::Value root;
        root["type"] = BEGIN;
        return root.toStyledString();
    }


    /*
     * player packet
    */
    static std::string refuel(const int money)
    {
        /*
         * player check-raise
        */
        Json::Value root;
        root["type"] = REFUEL;
        root["money"] = money;
        return root.toStyledString();
    }

    static std::string call()
    {
        /*
         * player call 
        */
        Json::Value root;
        root["type"] = CALL;
        return root.toStyledString();
    }

    static std::string fold()
    {
        /*
         * player fold 
        */
        Json::Value root;
        root["type"] = FOLD;
        return root.toStyledString();
    }

    static std::string allin()
    {
        /*
         * player allin
        */
        Json::Value root;
        root["type"] = ALLIN;
        return root.toStyledString();
    }

    static std::string licensePlayer(int cardNum)
    {
        /*
         * license player
         * suit : cardNum / 13
         * suit order : HEARTS DIAMONDS CLUB SPADE
         * number : cardNum % 13 + 1
        */
        Json::Value root;
        root["type"] = LICENSE_PLAYER;
        root["card"] = cardNum;
        return root.toStyledString();
    }

    static std::string licensePublic(int index, int cardNum)
    {
        /*
         * license public
         * suit : cardNum / 13
         * suit order : HEARTS DIAMONDS CLUB SPADE
         * number : cardNum % 13 + 1
         * index : the order of the public card(saved in index)
        */
        Json::Value root;
        root["type"] = LICENSE_PUBLIC;
        root["index"] = index;
        root["card"] = cardNum;
        return root.toStyledString();
    }

    static std::string showResult(Json::Value gameResult)
    {
        Json::Value root;
        root["type"] = GAME_RESULT;
        root["value"] = gameResult;
        return root.toStyledString();
    }


    /*
     * server packet
    */
    static std::string testAlive()
    {
        Json::Value root;
        root["type"] = TEST_ALIVE;
        return root.toStyledString();
    }

    static std::string requset()
    {
        Json::Value root;
        root["type"] = REQUSET;
        return root.toStyledString();
    }

    static std::string rLogin(bool succeed, const int money = 0)
    {
        /*
         * player login result
        */
        Json::Value root;
        root["type"] = LOGIN_RESULT;
        root["result"] = succeed ? SUCCEED : FAILURE;
        root["money"] = money;
        return root.toStyledString();
    }

    static std::string rEntry(bool succeed, const int room_id)
    {
        Json::Value root;
        root["type"]   = ENTRY_RESULT;
        root["result"] = succeed ? SUCCEED : FAILURE;
        root["room"]   = room_id;
        return root.toStyledString();
    }

    static std::string rPrepare(bool succeed = true)
    {
        /*
         * player prepare game result
        */
        Json::Value root;
        root["type"] = PREPARE_RESULT;
        root["result"] = succeed ? SUCCEED : FAILURE;
        return root.toStyledString();
    }

    static std::string rDisPrepare(bool succeed = true)
    {
        /*
         * player cancle preparation result
        */
        Json::Value root;
        root["type"] = DIS_PREPARE_RESULT;
        root["result"] = succeed ? SUCCEED : FAILURE;
        return root.toStyledString();
    }

    static std::string rBegin()
    {
        /*
         * game begin
        */
        Json::Value root;
        root["type"] = BEGIN_GAME;
        return root.toStyledString();
    }

    // static std::string hall(std::vector<Room> & v)
    // {
    //     /*
    //      * hall information
    //     */
    //     Json::Value root;
    //     Json::Value room_array;
    //     Json::Value item;
    //     root["type"] = HALL;
    //     for (auto room : v)
    //     {
    //         item["id"] = room.get_id();
    //         std::string seat = "";
    //         for (int i = 0; i < MAX_PLAYER_NUM; ++i)
    //         {
    //             seat += (room[i] == NULL) ? "0" : "1";
    //         }
    //         item["seat"] = seat;
    //         room_array.append(item);
    //     }
    //     root["room"] = room_array;
    //     return root.toStyledString();
    // }

    static std::string room(const int room_id, std::vector<PlayerTuple> & players)
    {
        /*
         * room information
        */
        Json::Value root;
        Json::Value seat_array;
        Json::Value item;
        root["type"] = ROOM;
        root["room_id"] = room_id;
        for (int i = 0; i < players.size(); ++i)
        {
            PlayerTuple & player = players[i];
            item["id"] = std::get<0>(player);
            item["money"] = std::get<1>(player);
            seat_array.append(item);
        }
        root["seat"] = seat_array;
        return root.toStyledString();
    }
};

#endif
