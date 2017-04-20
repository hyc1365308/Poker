#ifndef ROOM_H
#define ROOM_H

#include <QDialog>
#include <QDebug>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include "player.h"
#include <QPainter>
#include <Qtime>

namespace Ui {
class room;
}

const int startmoney[] = {10000,50000,100000,500000,1000000};

const int deafbet[] = {100,500,1000,5000,10000};

const QString pokerpath = ":/source/picture/allPoker/";

const QString pokerkind[4] = {"spade","heart","club","diamond"};

const QString pokersize[13] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};

//const QString roomname[5] = {"倔强青铜","秩序白银","荣耀黄金","尊贵铂金","永恒钻石"};

//标记每个人在界面上的位置，最后一个是发牌员的位置
const int position_center[2][10] = {{820,960,960,820,600,380,240,240,380,600},{235,315,425,455,455,455,425,315,235}};

const int position_xy[2][9] = {{790,970,970,790,570,350,170,170,350},{150,230,390,470,470,470,390,230,150}};

const int Card_xy[2][9] = {{860,1040,1040,860,640,420,240,240,420},{160,240,400,480,470,480,400,240,160}};

class room : public QDialog
{
    Q_OBJECT

public:
    explicit room(QString _userID,QString _formalID,int _roomlevel,int _roomnumber,QWidget *parent = 0);
    ~room();
    show1_3card();//展示第1到3张公共牌
    show4card();//展示第四张公共牌
    show5card();//展示第五张公共牌
    void getbeforeplayer();
    void placebeforeplayer();
    void gamestart();
    void get2owncard();
    void placefirstflag();
    void paintEvent(QPaintEvent *);
    void send2card(int _loc);
    QString transform(int num);
    void bet_turn1();
private slots:
    void changetime();

private:
    Ui::room *ui;
    int roomlevel;
    int roomnumber;
    const QString roomname[5];
    player *ourself;//自己
    player *playerlist[8];//记录房间的其它用户,不包括自己
    int playerspace[8];//记录当你进去之前房间里自第一个人开始与后一人的间隔
    int playernumber;//标记房间里的人的个数,包括自己
    QString commonCard[5];//5张公共牌
    bool seat[9];//标记一个房间的9个位置是否有人，当有人离开时置为false;
    int first_player;
    QLabel *prepare_start;//只有first_person有start权限
    QWidget *current_player;//标记发牌执行到的对象

    QWidget *position[9];//总体的0——8九个位置
    QLabel *head[9];//每个人的头像
    QLabel *name[9];//每个人的名字
    QLabel *money[9];//每个人的钱数
    QLabel *chouma[9];//钱数前面的筹码标志
    QWidget *Card[9];//每个人的两张卡牌
    QWidget *first_flag[9];//是否为第一个人的标记

};

#endif // ROOM_H
