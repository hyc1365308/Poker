#include "room.h"
#include "./game/Game.h"

int Room::next_id = 0;

void* runRoom(void* arg)
{
    Room* room = (Room*) arg;
    // run room thread
    while(true)
    {
        room->run_now = false;    // 房间当前没有进行游戏

        if (room->get_num() < 3)
        {
            // sleep 3 second
            // std::cout << "room sleep\n";
            sleep(3);
            continue;
        }

        std::cout << "Room " << room->id_ << " has " << room->players.size() << " players" << std::endl;

        std::vector<Player*> pv;
        // begin a new game
        std::cout << "Now begin a new game" << std::endl;
        for (auto it : room->players)
        {
            Player* player = new Player(it->get_id(), 10000);
            pv.push_back(player);
        }

        Game new_game(pv, room);

        room->run_now = true;

        new_game.start();
        // new_game.run();
    }
}

void Room::init()
{
    // create a new thread
    if(pthread_create(&tid_, NULL, runRoom, this) != 0)
    {
        std::cout << "Create room " << id_ << " thread wrong" << std::endl;
    }
}

void Room::exit()
{

}

