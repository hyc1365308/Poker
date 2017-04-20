#include "player.h"

player::player(QString _formalID,QString _userID):formalID(_formalID),userID(_userID)
{

}

QString player::getuserID()
{
    return userID;
}

QString player::getformalID()
{
    return formalID;
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
