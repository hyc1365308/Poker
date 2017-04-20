#include "room.h"
#include "ui_room.h"
#include <QImage>
#include <QTimer>

room::room(QString _userID,QString _formalID,int _roomlevel,int _roomnumber,QWidget *parent) :
    roomlevel(_roomlevel),roomnumber(_roomnumber),QDialog(parent),
    roomname{"倔强青铜","秩序白银","荣耀黄金","尊贵铂金","永恒钻石"},
    ui(new Ui::room)
{
    ui->setupUi(this);
    //qDebug()<<"Currrent time:"<<QTime::currentTime().toString("hh:mm:ss");
    QTimer *timer = new QTimer(this);   //声明一个定时器
    connect(timer, SIGNAL(timeout()), this, SLOT(changetime()));  //连接信号槽，定时器超时触发窗体更新
    timer->start(1000);   //启动定时器
    ui->timelabel->setAlignment(Qt::AlignCenter);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle(roomname[roomlevel-1]+"房间"+QString::number(roomnumber,10));
    //qDebug()<<userID + formalID;
    ourself = new player(_formalID,_userID);
    ourself->setmoney(startmoney[roomlevel-1]);

    for (int i = 0; i < 9; i++)
    {
        position[i] = new QWidget(this);
        position[i]->setGeometry(position_xy[0][i],position_xy[1][i],80,100);
        position[i]->setStyleSheet("border-image: url(:/source/picture/background/blue.jpg);");

        Card[i] = new QWidget(this);
        if (i == 4)
            Card[i]->setGeometry(Card_xy[0][i],Card_xy[1][i],100,100);
        else
            Card[i]->setGeometry(Card_xy[0][i],Card_xy[1][i],50,60);
        Card[i]->setStyleSheet("border-image:url();");

        first_flag[i] = new QWidget(this);
        first_flag[i]->setGeometry(position_xy[0][i]-20,position_xy[1][i],20,20);
        first_flag[i]->setStyleSheet("border-image:url();");
    }

    for (int i = 0; i < 9 ; i++)
    {
        head[i] = new QLabel(position[i]);
        head[i]->setGeometry(10,10,60,60);
        head[i]->setStyleSheet("border-image:url();");

        name[i] = new QLabel(position[i]);
        name[i]->setGeometry(10,70,60,15);
        name[i]->setFont(QFont("宋体",9,QFont::Bold));

        money[i] = new QLabel(position[i]);
        money[i]->setGeometry(25,85,45,15);
        money[i]->setFont(QFont("宋体",9,QFont::Bold));

        chouma[i] = new QLabel(position[i]);
        chouma[i]->setGeometry(10,85,10,10);
        chouma[i]->setStyleSheet("border-image: url(:/source/picture/icon/chouma.png);");

    }
    name[4]->setText(ourself->getuserID());
    money[4]->setText(QString::number(ourself->getmoney(),10));

    prepare_start = new QLabel(this);
    prepare_start->setGeometry(QRect(1010,530,100,100));

    for (int i = 0; i < 9; i++)
        seat[i] = false;
    seat[4] = true;//自己的位置已经坐人了

    //接下来从服务器获取房间里的其它人进行初始化
    getbeforeplayer();
    //放置玩家的位置
    if (first_player == 4)
    {
        prepare_start->setStyleSheet("border-image:url(:/source/picture/icon/start.png)");
        prepare_start->setEnabled(true);
        prepare_start->setCursor(Qt::PointingHandCursor);
    }
    else
    {
        prepare_start->setStyleSheet("border-image:url(:/source/picture/icon/prepare.png)");
        prepare_start->setEnabled(false);
    }
    placebeforeplayer();
    placefirstflag();
}

room::~room()
{
    delete ui;
}

void room::getbeforeplayer()
{
    //以下都要向服务器请求
    playernumber = 4;
    QString otherid[8] = {"user2","user3","user4"};
    QString othername[8] = {"hyq","fyl","csq"};//列表里的第一个为first_person
    int space[8] = {3,3};
    for (int i = 0; i < playernumber - 2; i++)
        playerspace[i] = space[i];
    int othermoney[8] = {1089190000,4522324,232320};
    for (int i = 0; i < playernumber - 1; i++)
    {
        playerlist[i] = new player(otherid[i],othername[i]);
        playerlist[i]->setmoney(othermoney[i]);
    }
}

void room::placebeforeplayer()
{
    //qDebug()<<"placebeforeplayer-----";
    if (playernumber <= 1)
    {
        first_player = 4;
        return;
    }
    int loc = 0;
    for (loc = 0; loc < playernumber - 2; loc++)
    {
        if (playerspace[loc] > 1)
            break;
    }
    //qDebug()<<"loc:"<<loc;
    for (int i = 0; i < playernumber - 1; i++)
    {
        QString moneylabel = QString::number(playerlist[i]->getmoney(),10);
        if (playerlist[i]->getmoney() >= 1000000)
            moneylabel = transform(playerlist[i]->getmoney());
        int _j = 3;
        if (i < loc)
        {
            for (int k = 0; k < loc; k++)
                _j -= playerspace[k];
        }
        if (i > loc)
        {
            for (int k = loc; k < i - loc; k++)
                _j += playerspace[k];
        }
        //qDebug()<<playerlist[i]->getuserID()<<playerlist[i]->getmoney();
        if (_j < 0)
            _j = 9 + _j;
        if (_j > 8)
            _j = _j - 9;
        if (i == 0)
            first_player = _j;//列表里的第一个人就是第一个开始的玩家
        //qDebug()<<"_j"<<_j;
        name[_j]->setText(playerlist[i]->getuserID());
        money[_j]->setText(moneylabel);
        seat[_j] = true;
    }
}

void room::get2owncard()
{
    //border-image: url(:/source/picture/background/button1.jpg);
    //从服务器获取自己的两张牌
    QString c1 = "spadeA";
    QString c2 = "club2";
    ourself->setowncard(c1,c2);
    qDebug()<<"get 2 card!!!";
    //update();
    QLabel *label1 = new QLabel(Card[4]);
    QLabel *label2 = new QLabel(Card[4]);
    label1->setStyleSheet("border-image: url(" + pokerpath + c1 + ".jpg)");
    label2->setStyleSheet("border-image: url(" + pokerpath + c2 + ".jpg)");
    //为了图片看得清楚些，还是不得不强行把这个扑克放大啊
    label1->setGeometry(0,0,70,100);
    label2->setGeometry(30,0,70,100);
    for (int i = 0; i <= 8; i++)
    {
        int loc = i + first_player;
        if (loc > 8)
            loc = loc - 9;
        if (seat[loc] == true && loc != 4)
        {
            //qDebug()<< "loc = "<<loc;
            send2card(loc);
         }
    }
}

void room::gamestart()
{
    qDebug()<<"game start!!!";
    get2owncard();
    bet_turn1();
}

void room::placefirstflag()
{
    first_flag[first_player]->setStyleSheet("border-image: url(:/source/picture/icon/firstflag.png);");
}

void room::paintEvent(QPaintEvent *e)
{
//到底应该怎么实现这个发牌动画啊啊啊、、、、
//    QPainter p(this);
//    QPixmap pixmap(pokerpath + "back.jpg");
//    p.drawPixmap(0,10,30,50,pixmap);
    //p.drawEllipse(0, 0, 100, 100);
}

void room::send2card(int _loc)
{
    //qDebug()<<"loc"<<_loc;
    QLabel *label = new QLabel(Card[_loc]);
    label->setStyleSheet("border-image: url(" + pokerpath + "back.jpg)");
    label->setGeometry(0,0,40,60);
}

QString room::transform(int num)
{
    QString answer;
    int _num = num / 10000;
    if (_num < 1000)
    {
        int _num4 = (num - _num * 10000)/1000;
        answer = QString::number(_num,10)+"."+QString::number(_num4,10)+"万";
    }
    else if (_num < 10000)
        answer = QString::number(_num,10)+"万";
    else
    {
        int _num2 = _num / 10000;
        if (_num2 < 10)
        {
            int _num3 = (_num - _num2 * 10000)/10;
            answer = QString::number(_num2,10)+"."+QString::number(_num3,10)+"亿";
        }
        else
        {
            int _num3 = (_num - _num2 * 10000)/100;
            answer = QString::number(_num2,10)+"."+QString::number(_num3,10)+"亿";
        }
    }
    return answer;
}


void room::changetime()
{
    ui->timelabel->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void room::bet_turn1()
{
    //分三个阶段--wait bet wait

}
