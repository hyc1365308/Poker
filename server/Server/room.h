#ifndef ROOM_H
#define ROOM_H

#include <QStringList>
class room
{
public:
    room(int _roomlevel);
    int get_playernum();
    QStringList get_playerid();
    void setPlayernum(int _num);
    void playerin(QString _id);
    void playerout(QString _id);
private:
    int roomlevel;
    int playernum;
    QStringList playerid;
};

#endif // ROOM_H
