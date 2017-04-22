#include "player.h"

player::player(QString _userName,QString _userId):userName(_userName),userId(_userId)
{

}

QString player::getuserId()
{
    return userId;
}

QString player::getuserName()
{
    return userName;
}

void player::setmoney(int _money)
{
    money = _money;
}

int player::getmoney()
{
    return money;
}

void player::setowncard(QString card1,QString card2)
{
    ownCard[0] = card1;
    ownCard[1] = card2;
}
