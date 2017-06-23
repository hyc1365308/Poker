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
                cout << "test connect " << *it << " " << endl;
                ++it;
            }
            else
            {
                // sock has been closed
                hall->mtx_.lock();
                cout << "socket " << *it << " is detected closed" << endl;
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
            // cout << "check sock " << (*it) << "'s entry" << endl;
            std::string packet = (*it)->recvData();
            if (packet != "")
            {
                cout << "sock " << *it << " get a packet" << endl;
                Json::Value root;
                if (Packet::decode(packet, root) && root["type"] == ENTRY)
                {
                    int room_id = root["room"].asInt();

                    if (room_id < 0 || room_id >= 6)
                    {
                        cout << "cannot find room " << room_id << endl;
                        continue;
                    }

                    // 向房间添加新的成员
                    bool flag = hall->rooms[room_id]->append(*it);
                    
                    PlayerSock* old_sock = (*it);

                    if (flag)
                    {
                        hall->mtx_.lock();
                        cout << "erase " << (*it) << endl;
                        it = hall->player_sets_.erase(it);
                        cout << "erase done\n";
                        hall->mtx_.unlock();

                        std::cout << "Now hall has " << hall.size() << " member" << std::endl;
                    }
                }
            }
        }

        Sleep(100);  // sleep
    }
}
