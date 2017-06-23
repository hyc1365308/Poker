/************************************************
 * 名称 : packet.h
 * 作者 : 冯瑜林
 * 时间 : 2017-05-10(1st)、2017-6-18(last)
 * 内容 : 网络 json 数据包的生成与解析
************************************************/

#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <vector>
#include <tuple>

#include "json/json.h"

#define SUCCEED 1
#define FAILURE 0

#define separator "#$#"

// 用户个人数据
//      元组各个位置分别是 : 用户名字 钱数 位置(从0开始连续编号)
typedef std::tuple<std::string, int, bool> PlayerTuple;

/*
 * 数据包的类型
*/
enum Type
{
    INVALID,        // 无效数据包

    LOGIN,          // 玩家登录数据包
    ENTRY,          // 玩家进入房间
    LEAVE,          // 玩家离开房间
    PREPARE,        // 玩家准备开始
    DIS_PREPARE,    // 玩家取消准备

    ENTRY_RESPONSE,         // 进入房间响应
    LOGIN_RESPONSE,         // 登录结果
    PREPARE_RESPONSE,       // 准备响应
    DIS_PREPARE_RESPONSE,   // 取消准备响应

    REFUEL,         // 加注
    CALL,           // 跟注
    FOLD,           // 盖牌
    ALLIN,          // 全跟

    GAME_BEGIN,     // 通知玩家游戏正式开始
    REQUSET,        // 请求玩家进行一次操作
    OPERATE,        // 其他成员的操作，数据包要向所有client发送
    HALL_INFO,      // 发送大厅信息
    ROOM_INFO,      // 发送房间信息

    LICENSE_PLAYER, // 玩家个人牌
    LICENSE_PUBLIC, // 公共牌
    GAME_RESULT,    // 游戏结果
};

class Packet
{
public:
    /*
     * 解析数据包
    */
    static bool decode(std::string packet, Json::Value & root)
    {
        /*
         * 功能 : 解析一个数据包
         * 参数 :
         *      packet : string类型，代表数据包
         *      root   : 一个Json::Value对象的引用，代表传入的json值
         *               (这里为了解析效率，直接将结果保存在传入的 root 引用中)
        */

        Json::Reader reader;
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
    
    static std::vector<PlayerTuple> decodeRoomInfo(const Json::Value & root)
    {
        /*
         * 功能 : 解析房间信息
         * 参数 :
         *      root : 一个Json::Value对象，代表传入的json值
         * 返回值 :
         *      一个三元元组构成的数组，分别是每个玩家的id、money与是否在该位置
        */

        std::vector<PlayerTuple> player_tuples;

        Json::Value seat_array = root["seat"];
        Json::Value item;
        for (int i = 0; i < seat_array.size(); ++i)
        {
            std::string id = seat_array[i]["id"].asString();
            int money = seat_array[i]["money"].asInt();
            bool is_in = seat_array[i]["is_in"].asInt();

            player_tuples.push_back(make_tuple(id, money, is_in));
        }

        return player_tuples;
    }

    static std::vector<std::tuple<int, int, int>> decodeResult(const Json::Value & root)
    {
        /*
         * 功能 : 解析游戏结果信息
         * 参数 :
         *      root : 一个Json::Value对象，代表传入的json值
         * 返回值 :
         *      一个三元元组构成的数组，分别是每个玩家的id、money与是否在该位置
        */

        std::vector<std::tuple<int, int, int>> vec;

        for (int i = 0; i < root["value"].size(); ++i)
        {
            auto & item = root["value"][i];
            auto tuple_item = std::make_tuple(item["money"].asInt(), item["card1"].asInt(), item["card2"].asInt());
            vec.push_back(tuple_item);
        }

        return vec;
    }


    /*
     * 游戏开始前客户端向服务器端发送的数据包
    */
    static std::string login(const std::string & username, const std::string & password)
    {
        /*
         * 功能 : 客户端登录数据包生成
         * 参数 :
         *      username : 用户ID，string 类型
         *      password : 用户密码，string 类型
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = LOGIN;
        root["username"] = username;
        root["password"] = password;
        return root.toStyledString() + separator;
    }

    static std::string entry(const int room_num)
    {
        /*
         * 功能 : 生成玩家进入房间的数据包
         * 参数 :
         *      room_num : 要进入的房间编号
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = ENTRY;
        root["room"] = room_num;
        return root.toStyledString() + separator;
    }

    static std::string leave(const int room_num)
    {
        /*
         * 功能 : 生成玩家离开房间的数据包
         * 参数 :
         *      room_num : 要进入的房间编号
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = LEAVE;
        root["room"] = room_num;
        return root.toStyledString() + separator;
    }

    static std::string prepare()
    {
        /*
         * 功能 : 生成玩家准备开始的数据包
         * 参数 :
         *      
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = PREPARE;
        return root.toStyledString() + separator;
    }

    static std::string disPrepare()
    {
        /*
         * 功能 : 生成玩家取消准备的数据包
         * 参数 :
         *      
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = DIS_PREPARE;
        return root.toStyledString() + separator;
    }


    /*
     * 游戏开始前服务器端向客户端发送的数据包
    */
    static std::string rLogin(bool succeed, const int money = 0)
    {
        /*
         * 功能 : 用户登录响应
         * 参数 :
         *      succeed : 登录是否成功
         *      money   : 当前用户帐户钱数
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = LOGIN_RESPONSE;
        root["result"] = succeed ? SUCCEED : FAILURE;
        root["money"] = money;
        return root.toStyledString() + separator;
    }

    static std::string rEntry(bool succeed, const int room_id)
    {
        /*
         * 功能 : 用户进入房间响应
         * 参数 :
         *      succeed : 进入房间是否成功
         *      money   : 房间编号
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"]   = ENTRY_RESPONSE;
        root["result"] = succeed ? SUCCEED : FAILURE;
        root["room"]   = room_id;
        return root.toStyledString() + separator;
    }

    static std::string rPrepare(bool succeed = true)
    {
        /*
         * 功能 : 用户准备开始的响应
         * 参数 :
         *      succeed : 操作是否成功
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = PREPARE_RESPONSE;
        root["result"] = succeed ? SUCCEED : FAILURE;
        return root.toStyledString() + separator;
    }

    static std::string rDisPrepare(bool succeed = true)
    {
        /*
         * 功能 : 用户取消准备的响应
         * 参数 :
         *      succeed : 操作是否成功
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = DIS_PREPARE_RESPONSE;
        root["result"] = succeed ? SUCCEED : FAILURE;
        return root.toStyledString() + separator;
    }

    static std::string begin()
    {
        /*
         * 功能 : 向用户发送游戏开始的信息
         * 参数 :
         *      
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = GAME_BEGIN;
        return root.toStyledString() + separator;
    }


    /*
     * 游戏过程中玩家向服务器发送的数据包
    */
    static std::string call()
    {
        /*
         * 功能 : 跟注
         * 参数 :
         *      
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = CALL;
        return root.toStyledString() + separator;
    }

    static std::string refuel(const int money)
    {
        /*
         * 功能 : 加注
         * 参数 :
         *      money : 加注金额
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = REFUEL;
        root["money"] = money;
        return root.toStyledString() + separator;
    }

    static std::string fold()
    {
        /*
         * 功能 : 盖牌
         * 参数 :
         *      
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = FOLD;
        return root.toStyledString() + separator;
    }

    static std::string allin()
    {
        /*
         * 功能 : All in
         * 参数 :
         *      
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = ALLIN;
        return root.toStyledString() + separator;
    }


    /*
     * 游戏过程中服务器向玩家发送的数据包
    */
    static std::string licensePlayer(int cardNum)
    {
        /*
         * 功能 : 向玩家发送个人牌
         * 参数 :
         *      cardNum : 牌的编号
         *          花色(suit) = cardNum / 13
         *              顺序 : HEARTS DIAMONDS CLUB SPADE
         *          牌大小     = cardNum % 13 + 1
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = LICENSE_PLAYER;
        root["card"] = cardNum;
        return root.toStyledString() + separator;
    }

    static std::string licensePublic(int index, int cardNum)
    {
        /*
         * 功能 : 向玩家发送公共牌
         * 参数 :
         *      cardNum : 牌的编号
         *          花色(suit) = cardNum / 13
         *              顺序 : HEARTS DIAMONDS CLUB SPADE
         *          牌大小     = cardNum % 13 + 1
         * 返回值 :
         *      一个 string，为生成的数据包
        */
        Json::Value root;
        root["type"] = LICENSE_PUBLIC;
        root["index"] = index;
        root["card"] = cardNum;
        return root.toStyledString() + separator;
    }

    static std::string showResult(std::vector<std::tuple<int, int, int>> result)
    {
        /*
         * 功能 : 显示游戏结果
         * 参数 :
         *      gameResult : Json::Value对象，表示游戏结果
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        Json::Value player_arr;
        Json::Value item;
        root["type"] = GAME_RESULT;
        for (auto player : result)
        {
            item["money"] = std::get<0>(player);
            item["card1"] = std::get<1>(player);
            item["card2"] = std::get<2>(player);
            player_arr.append(item);
        }

        root["value"] = player_arr;

        return root.toStyledString() + separator;
    }

    static std::string requset(const int cur_max_money)
    {
        /*
         * 功能 : 向玩家请求一次操作
         * 参数 :
         *      gameResult : Json::Value对象，表示游戏结果
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = REQUSET;
        root["cur_max_money"] = cur_max_money;  // 当前场上最大钱数
        return root.toStyledString() + separator;
    }

    static std::string operate(const int player_pos, const Type & player_op, const int money_left = 0, const int money_op = 0)
    {
        /*
         * 功能 : 向其他玩家广播某玩家的某次操作
         * 参数 :
         *      player_pos : 玩家位置
         *      player_op  : 玩家操作，为 Type 枚举类型
         *      money_left : 该操作玩家当前剩余的钱数
         *      money_op   : 该玩家本次操作的钱数
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        root["type"] = OPERATE;
        root["player_pos"] = player_pos;
        root["player_op"]  = player_op;
        root["money_left"] = money_left;
        root["money_op"]   = money_op;

        return root.toStyledString() + separator;
    }

    static std::string hall(const int room_num, std::vector<std::tuple<int, int>> & vec)
    {
        /*
         * 功能 : 广播大厅信息
         * 参数 :
         *      vec : 一个二元元组的数组对象，存储所有的房间编号及其人数
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        Json::Value room_array;
        Json::Value item;
        root["type"] = HALL_INFO;
        root["room_num"] = room_num;
        for (auto room : vec)
        {
            item["id"]         = std::get<0>(room);
            item["player_num"] = std::get<1>(room);
            room_array.append(item);
        }
        root["room"] = room_array;
        return root.toStyledString() + separator;
    }

    static std::string room(const int room_id, std::vector<PlayerTuple> && players)
    {
        /*
         * 功能 : 用户所在房间信息
         * 参数 :
         *      players : 一个三元元组(PlayerTuple)的数组对象，存储所有的房间编号及其人数
         * 返回值 :
         *      一个 string，为生成的数据包
        */

        Json::Value root;
        Json::Value seat_array;
        Json::Value item;
        root["type"] = ROOM_INFO;
        root["room_id"] = room_id;
        for (int i = 0; i < players.size(); ++i)
        {
            PlayerTuple & player = players[i];
            item["id"] = std::get<0>(player);
            item["money"] = std::get<1>(player);
            item["is_in"] = std::get<2>(player);
            seat_array.append(item);
        }
        root["seat"] = seat_array;
        return root.toStyledString() + separator;
    }

    static std::vector<std::string> split(const std::string & str, const std::string & seperator = separator)
    {
        /*
         * 功能 : 根据传入的分隔符切分 string
         * 参数 :
         *      str       : 需要切分的 string
         *      separator : 分隔符
         * 返回值 :
         *      一个 string数组，为分割后的每个子字符串
        */

        // 切分结果
        std::vector<std::string> result;

        typedef std::string::size_type string_size;
        string_size i = 0;
        
        while(i != str.size()){
            // 找到字符串中首个不等于分隔符的字母；
            int flag = 0;
            while(i != str.size() && flag == 0){
                flag = 1;
                for(string_size x = 0; x < seperator.size(); ++x)
                if(str[i] == seperator[x]){
                    ++i;
                    flag = 0;
                    break;
                }
            }
            
            // 找到又一个分隔符，将两个分隔符之间的字符串取出；
            flag = 0;
            string_size j = i;
            while(j != str.size() && flag == 0){
                for(string_size x = 0; x < seperator.size(); ++x)
                if(str[j] == seperator[x]){
                    flag = 1;
                    break;
                }

                if(flag == 0) 
                ++j;
            }

            if(i != j){
                result.push_back(str.substr(i, j-i));
                i = j;
            }
        }

        return result;
    }

};

#endif
