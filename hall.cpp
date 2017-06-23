#include "hall.h"

void* testConnect(void* arg)
{
    /*
     * test all hall's socket connections every 5 seconds
     * if a socket is closed, remove it from the hall
    */

    Hall* hall = (Hall*) arg;

    while(true)
    {
        for (auto it = hall->player_sets_.begin(); it != hall->player_sets_.end(); )
        {
            // sock = *it
            if ((*it)->testConnect())
            {
                // std::cout << "test connect " << *it << " " << std::endl;
                ++it;
            }
            else
            {
                // sock has been closed
                hall->mtx_.lock();
                std::cout << "socket " << *it << " is detected closed" << std::endl;
                delete (*it);
                it = hall->player_sets_.erase(it);
                hall->mtx_.unlock();
            }
        }

        // test all sockets connection every 10 seconds
        sleep(10);
    }
}

void* waitEntry(void* arg)
{
    Hall* hall = (Hall*) arg;

    while (true)
    {
        for (auto it = hall->player_sets_.begin(); it != hall->player_sets_.end(); ++it)
        {
            // std::cout << "check sock " << (*it) << "'s entry" << std::endl;
            std::string packet = (*it)->recvData();
            if (packet != "")
            {
                std::cout << "sock " << *it << " get a packet" << std::endl;
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

        Sleep(100);  // sleep
    }
}

void Hall::run()
{
    // create test connection thread
    pthread_t test_tid;
    pthread_create(&test_tid, NULL, testConnect, this);

    // create wait entry thread
    pthread_t hall_tid;
    pthread_create(&hall_tid, NULL, waitEntry, this);

    std::cout << "hall thread is running" << std::endl;
}
