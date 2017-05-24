#include "room.h"

int Room::next_id = 1;

void* runRoom(void* arg)
{
    Room* room = (Room*) arg;
    // run room thread
    while(true)
    {
        std::cout << "Room " << room->id << " has " << room->players.size() << " players" << std::endl;

        // sleep 3 second
        sleep(3);
    }
}

void Room::init()
{
    // create a new thread
    if(pthread_create(&tid, NULL, runRoom, this) != 0)
    {
        std::cout << "Create room " << id << " thread wrong" << std::endl;
    }
}

void Room::exit()
{

}

