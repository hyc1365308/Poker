#include "player.h"

player::player(QString _userId):userId(_userId)
{

}

void player::set_Nm_Sex(QString _userName, QString _sex)
{
    userName = _userName;
    userSex = _sex;
}

QString player::getuserId()
{
    return userId;
}

QString player::getuserName()
{
    return userName;
}

QString player::getuserSex()
{
    return userSex;
}

void player::setmoney(int _money)
{
    money = _money;
}

void player::set_AllMoney(unsigned int _allMoney)
{
    allMoney = _allMoney;
}

int player::getmoney()
{
    return money;
}

unsigned int player::getallMoney()
{
    return allMoney;
}

void player::setowncard(QString card1,QString card2)
{
    ownCard[0] = card1;
    ownCard[1] = card2;
}
