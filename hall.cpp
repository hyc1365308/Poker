/************************************************
 * 名称 : hall.h
 * 作者 : 冯瑜林
 * 时间 : 2017-06-21(1st)
 * 内容 : 大厅实现文件
************************************************/

#include "hall.h"

void* testConnect(void* arg)
{
    /*
     * 功能 : 检测所有用户连接是否断开，若断开，则从大厅中移除掉该用户
     * 参数 :
     *      arg : Hall类型对象的指针
    */

    Hall* hall = (Hall*) arg;

    while(true)
    {
        for (auto it = hall->player_sets_.begin(); it != hall->player_sets_.end(); )
        {
            if ((*it)->testConnect())
            {
                ++it;
            }
            else
            {
                // 用户socket已经断开
                hall->mtx_.lock();
                std::cout << "socket " << *it << " is detected closed" << std::endl;
                delete (*it);
                it = hall->player_sets_.erase(it);
                hall->mtx_.unlock();
            }
        }

        // 每10s检测一次是否断线
        sleep(10);
    }
}

void* waitEntry(void* arg)
{
    /*
     * 功能 : 等待大厅中的用户发出进入房间的请求
     * 参数 :
     *      arg : Hall类型对象的指针
    */
    Hall* hall = (Hall*) arg;

    while (true)
    {
        for (auto it = hall->player_sets_.begin(); it != hall->player_sets_.end(); ++it)
        {
            // 接收数据包
            std::string packet = (*it)->recvData();
            if (packet != "")
            {
                Json::Value root;
                if (Packet::decode(packet, root) && root["type"] == ENTRY)
                {
                    int room_id = root["room"].asInt();

                    if (room_id < 0 || room_id >= hall->room_num_)
                    {
                        std::cout << "cannot find room " << room_id << std::endl;
                        continue;
                    }

                    // 向房间添加新的成员
                    bool flag = hall->rooms_[room_id]->append(*it);
                    
                    PlayerSock* old_sock = (*it);

                    if (flag)
                    {
                        hall->mtx_.lock();
                        std::cout << "erase " << (*it) << std::endl;
                        it = hall->player_sets_.erase(it);
                        hall->mtx_.unlock();
                        std::cout << "Now hall has " << hall->size() << " member" << std::endl;
                    }
                }
            }
        }

        // 每轮循环后暂停一段
        Sleep(100);
    }
}

void Hall::run()
{
    // 创建测试用户连接的线程
    pthread_t test_tid;
    pthread_create(&test_tid, NULL, testConnect, this);

    // 创建大厅线程
    pthread_t hall_tid;
    pthread_create(&hall_tid, NULL, waitEntry, this);

    std::cout << "hall thread is running" << std::endl;
}
