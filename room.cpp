/************************************************
 * 名称 : room.h
 * 作者 : 冯瑜林
 * 时间 : 2017-05-13(1st)、2017-6-22(last)
 * 内容 : 房间类实现文件
************************************************/

#include "room.h"
#include "hall.h"
#include "server.h"

// 房间的编号从0开始,依次向后
int Room::next_id_ = 0;

/*
 * 私有成员函数
*/

extern void* runRoom(void* arg);
void Room::init()
{
    // 创建room线程并开始执行
    if(pthread_create(&tid_, NULL, runRoom, this) != 0)
    {
        std::cout << "Create room " << id_ << " thread wrong" << std::endl;
    }
}

int Room::getPlayerPos(const std::string player_id)
{
    /*
     * 功能 : 根据玩家id获取其所在位置
     * 参数 :
     *      player_id : 用户ID，string 类型
     * 返回值 :
     *      一个int值,代表用户位置
    */

    for (int i = 0; i < players_.size(); ++i)
    {
        if (player_id == players_[i]->get_id())
        {
            return i;
        }
    }

    std::cout << "get player id_ worng" << std::endl;
    return -1;
}

std::vector<PlayerTuple> Room::getPlayers(const int pos)
{
    /*
     * 功能 : 得到此刻的房间信息
     *        (这里根据不同的玩家返回不同的信息(座次不同))
     * 参数 :
     *      pos : 玩家座次(位置)
     * 返回值 :
     *      一个PlayerTuple的数组,表示每个玩家的信息
    */

    std::vector<PlayerTuple> player_tuples;
    for (int i = 0; i < players_.size(); ++i)
    {
        // 玩家如果在该位置,将当前该位置的is_in值设置为true
        bool is_in = (i == pos) ? true : false;

        // 获取玩家的ID、钱数与is_in值
        player_tuples.push_back(make_tuple(players_[i]->get_id(), players_[i]->get_money(), is_in));
    }

    return player_tuples;
}


/*
 * 提供给Server类及Game类的接口
*/

bool Room::append(PlayerSock * player)
{
    /*
     * 功能 : 向房间添加一个用户
     * 参数 :
     *      player : PlayerSock指针，代表玩家
     * 返回值 :
     *      一个PlayerTuple的数组,表示每个玩家的信息
    */

    if (players_.size() >= MAX_PLAYER_NUM || run_now_)
    {
        // 当前人员已满或者房间游戏已经开始
        return false;
    }

    mtx_.lock();
    std::cout << "Room " << id_ << " join a new player" << std::endl;
    players_.push_back(player);
    mtx_.unlock();

    for (int i = 0; i < players_.size(); ++i)
    {
        // 向房间内每个玩家发送当前的信息
        players_[i]->sendData(Packet::rEntry(true, id_));
        players_[i]->sendData(Packet::room(id_, getPlayers(i)));
    }

    return true;
}

void Room::remove(const int pos)
{
    /*
     * 功能 : 移除房间中的一个玩家
     * 参数 :
     *      pos : PlayerSock指针，代表玩家
    */

    remove(players_[pos]);
}

void Room::remove(const PlayerSock * player)
{
    /*
     * 功能 : 移除房间中的一个玩家
     * 参数 :
     *      player : PlayerSock指针，代表玩家
     * 返回值 :
    */

    mtx_.lock();
    for (auto it = players_.begin(); it != players_.end(); ++it)
    {
        if (*it == player)
        {
            it = players_.erase(it);
            break;
        }
    }
    mtx_.unlock();
}

void Room::removeAll()
{
    /*
     * 功能 : 移除所有用户，并将其添加到大厅中
    */

    for (auto player : players_)
    {
        hall_->insert(player);
    }

    players_.clear();
}

Json::Value Room::getOperate(Player* player, const int cur_max_money)
{
    /*
     * 功能 : 从指定玩家处获取一个操作
     *        注意 : 一个玩家长时间未操作(暂定为3min，可修改下面wait_time来更改)，则将其盖牌
     * 参数 :
     *      player : Player指针，代表一个玩家
     *      cur_max_money : 当前场上最大的下注金额
     * 返回值 :
    */

    const int test_interval = 10;   // ms
    const int wait_time = 60 * 3;   // s

    PlayerSock * sock = get_player(player);
    std::cout << "\nNow turn to player " << sock->get_id() << std::endl;

    // 向指定用户发送请求
    sock->sendData(Packet::requset(cur_max_money));

    // 不断等待用户发送数据
    Json::Value root;
    for (int i = 0; i < wait_time * 1000 / test_interval; ++i)
    {
        std::string packet_str = sock->recvData();

        if (packet_str == "" || !Packet::decode(packet_str, root))
        {
            // 用户未发送数据或者数据包无效
            Sleep(test_interval);
            continue;
        }
        else
        {
            // 得到有效数据包，返回结果
            std::cout << "get operation done" << std::endl;
            return root;
        }
    }

    // 超过等待时间后仍未进行操作，相当于盖牌
    root["type"] = FOLD;
    return root;
}

void Room::castOperate(const Player* player, const int operation, const int money_left,
                       const int money_all_op, bool is_blind)
{
    /*
     * 功能 : 广播某个用户的操作
     *        注意 : 如果是下盲注，则需广播给下注人，其他操作不需要(不能)广播给下注人
     * 参数 :
     *      player : Player指针，代表一个玩家
     *      operation : 操作类型(为Type枚举类型，共四种)
     *                      REFUEL  CALL  FOLD  ALLIN
     *      money_left: 用户目前剩下的钱数
     *      money_all_op : 用户本轮游戏中下注的总钱数
     *      is_blind  : 是否是下盲注，默认是否
     * 返回值 :
    */

    int player_pos = getPlayerPos(player->_name);

    std::string op_str = Packet::operate(player_pos, Type(operation), money_left, money_all_op);

    for (int i = 0; i < players_.size(); ++i)
    {
        if (!is_blind && i == player_pos)
        {
            // 下盲注 或者 是给本人发
            // 不需要通知自己，跳过
            continue;
        }

        // 发送当前操作信息
        players_[i]->sendData(op_str);
    }
}

void Room::licensePlayer(Player* player, Card & card)
{
    /*
     * 功能 : 向某个用户发其私人牌
     * 参数 :
     *      player : Player指针，代表一个玩家
     *      card   : 要发的牌，Card类型
     * 返回值 :
    */

    std::cout << "send license info now" << std::endl;
    PlayerSock * sock = get_player(player);
    std::cout << "now send message " << sock->get_id() << std::endl;
    sock->sendData(Packet::licensePlayer(card.toInt()));
}

void Room::licensePublic(int order, Card & card)
{
    /*
     * 功能 : 向所有用户发其公共牌
     * 参数 :
     *      order : 要发送的是第几张公共牌
     *      card  : 要发的牌，Card类型
     * 返回值 :
    */

    std::cout << "send public license info now" << std::endl;
    std::string root = Packet::licensePublic(order, card.toInt());
    for (auto sock : players_)
    {
        sock->sendData(root);
    }
}

void Room::showResult(std::vector<std::tuple<int, Card, Card>> in_result)
{
    /*
     * 功能 : 向所有用户展示结果，同时服务器要更新所有数据
     * 参数 :
     *      result : 一个三元元组的数组，存储了用户结算后的钱数与两张牌的信息
     *          注意 :  用户的编号即为其在数组中的位置
     *                  需要将两张Card转化成int后再进行传输
    */

    // 向Packet类传递的数组
    //      三元元组分别存储: 玩家结算后钱数、两张牌的int值
    std::vector<std::tuple<int, int, int>> out_result;

    // 要更新的用户id及其更新值
    std::map<std::string, int> update_dict;

    for (int i = 0; i < in_result.size(); ++i)
    {
        auto & player = in_result[i];

        // 提取出每张牌，转换成int后压入新数组中
        const int player_money = std::get<0>(player);
        const int card_num1 = std::get<1>(player).toInt();
        const int card_num2 = std::get<2>(player).toInt();

        out_result.push_back(std::make_tuple(player_money, card_num1, card_num2));

        // 将当前用户要更新的信息添加到字典中
        update_dict[getPlayerID(i)] = player_money;
    }

    // 向所有玩家发送结果
    std::string result_str = Packet::showResult(out_result);
    for (auto sock : players_)
    {
        sock->sendData(result_str);
    }

    // 在server端更新用户信息
    // server_->updatePlayerInfo(update_dict);
}


/*
 * 类友元函数
*/
void* runRoom(void* arg)
{
    /*
     * 功能 : 房间线程
     *        注意 : 房间循环进行游戏，每次游戏之后都将所有人赶出房间(~^o^~ 移到大厅)
     * 参数 :
     * 返回值 :
    */

    // 获取room指针
    Room* room = (Room*) arg;

    while(true)
    {
        room->run_now_ = false;    // 设置房间当前不再进行游戏

        // 这里简化操作，当有不少于 MIN_PLAYER_NUM 个玩家都进入房间后直接开始
        if (room->get_num() < MIN_PLAYER_NUM)
        {
            // 等待3s
            sleep(3);
            continue;
        }

        std::cout << "Room " << room->id_ << " has " << room->players_.size() << " players" << std::endl;

        std::vector<Player*> pv;
        
        // 开始一场新的游戏
        std::cout << "Now begin a new game" << std::endl;
        for (auto it : room->players_)
        {
            Player* player = new Player(it->get_id(), it->get_money());
            pv.push_back(player);
        }

        // 构建游戏类
        Game new_game(pv, room);

        // 设置当前房间正在使用中
        room->run_now_ = true;

        // 新一轮游戏开始
        new_game.start();

        room->removeAll();
    }
}
