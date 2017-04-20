#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
class player
{
public:
    player(QString _formalID,QString _userID);
    QString getuserID();
    QString getformalID();
    int getmoney();
    void setmoney(int _money);
    void bet();//押注 - 押上筹码
    void call(int num);//跟进 - 跟随众人押上同等的注额
    void fold();//收牌 / 不跟 - 放弃继续牌局的机会
    void check();//让牌 - 在无需跟进的情况下选择把决定“让”给下一位
    void raise(int num);//加注 - 把现有的注金抬高
    void re_raise(int num);// 再加注 - 再别人加注以后回过来再加注
    void all_in();//全押 - 一次过把手上的筹码全押光
    void setowncard(QString card1,QString card2);

private:
    QString formalID;
    QString userID;
    unsigned int money;//对于player的money还是暂时不考虑其超出int最大值的情况吧
    QString ownCard[2];//标记两张公共牌
};

#endif // PLAYER_H
