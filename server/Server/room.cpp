#include "room.h"

room::room(int _roomlevel)
{
    roomlevel = _roomlevel;
    playernum = 0;
}

int room::get_playernum()
{
    return playernum;
}

void room::setPlayernum(int _num)
{
    playernum = _num;
}

void room::playerin(QString _id)
{
    playerid.append(_id);
    playernum++;
}

void room::playerout(QString _id)
{
    for (int i = 0; i < playerid.size(); i++)
    {
        if (playerid.at(i) == _id)
        {
            playerid.removeAt(i);
            break;
        }
    }
}

