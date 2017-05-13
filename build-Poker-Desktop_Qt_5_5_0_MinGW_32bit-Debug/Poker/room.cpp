#include "room.h"
#include "ui_room.h"

room::room(QString _userId,QString _userName,QString _userSex,int _roomlevel,int _roomnumber,QWidget *parent) :
    roomlevel(_roomlevel),roomnumber(_roomnumber),QDialog(parent),
    roomname{"倔强青铜","秩序白银","荣耀黄金","尊贵铂金","永恒钻石"},
    ui(new Ui::room)
{
    ui->setupUi(this);
    currentState = 0;
    //qDebug()<<"Currrent time:"<<QTime::currentTime().toString("hh:mm:ss");
    QTimer *_timer = new QTimer(this);   //声明一个定时器
    connect(_timer, SIGNAL(timeout()), this, SLOT(changetime()));  //连接信号槽，定时器超时触发窗体更新
    _timer->start(1000);   //启动定时器
    ui->timelabel->setAlignment(Qt::AlignCenter);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle(roomname[roomlevel-1]+"房间"+QString::number(roomnumber,10));
    //qDebug()<<userID + formalID;

    for (int i = 0; i < 5; i++)
    {
        dothings[i] = new QPushButton(things[i],this);
        dothings[i]->setStyleSheet("color: rgb(85, 0, 0);border-image: url(:/source/picture/background/singalroom.jpg);");
        dothings[i]->setGeometry(i*120+330,600,80,40);
        dothings[i]->setFont(QFont("宋体",16,QFont::Bold));
        dothings[i]->hide();
    }

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
        first_flag[i]->setGeometry(position_xy[0][i]-20,position_xy[1][i]+10,20,20);
        first_flag[i]->setStyleSheet("border-image:url();");

        _chouma[i] = new QLabel(this);
        _chouma[i]->setGeometry(_chouma_xy[0][i],_chouma_xy[1][i],20,20);
        //_chouma[i]->setStyleSheet("border-image: url();");
        _chouma[i]->setStyleSheet("border-image: url(:/source/picture/icon/chouma.png);");
        _chouma[i]->hide();

        betnum[i] = new QLabel(this);
        betnum[i]->setGeometry(_chouma_xy[0][i]+25,_chouma_xy[1][i],55,20);
        betnum[i]->setStyleSheet("border-image:url();color:red");
        betnum[i]->setFont(QFont("宋体",9,QFont::Bold));
        //betnum[i]->setText("100000");
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
        chouma[i]->setStyleSheet("border-image:url();");
        //chouma[i]->setStyleSheet("border-image: url(:/source/picture/icon/chouma.png);");
    }

    ourself = new player(_userId);
    ourself->setmoney(startmoney[roomlevel - 1]);
    ourself->set_Nm_Sex(_userName,_userSex);

    init();
    //之后应该从服务器获取
    current_player = -1;
    waitsecond = true;
    buttonclick = false;
    cnt = 0;


    name[4]->setText(ourself->getuserName());
    int _mymoney = ourself->getmoney();
    QString s = QString::number(_mymoney,10);
    if (_mymoney >= 1000000)
        s = transform(_mymoney);

    money[4]->setText(s);
    chouma[4]->setStyleSheet("border-image: url(:/source/picture/icon/chouma.png);");
    if (ourself->getuserSex() == "boy")
        head[4]->setStyleSheet("border-image:url(:/source/picture/icon/boy.jpg);");
    else if (ourself->getuserSex() == "girl")
        head[4]->setStyleSheet("border-image:url(:/source/picture/icon/girl1.jpg);");
    else
        head[4]->setStyleSheet("border-image:url(:/source/picture/icon/dog.jpg);");
    prepare_start = new QLabel(this);
    prepare_start->setGeometry(QRect(1010,530,100,100));

    testbutton = new QPushButton("click me!",this);
    testbutton->setGeometry(600,580,100,50);
    //testbutton->hide();
    testbutton->setStyleSheet("border-image: url(:/source/picture/background/singalroom.jpg);");
    connect(testbutton,SIGNAL(clicked()),this,SLOT(dosomething()));

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


void room::init()
{
    userMoney = startmoney[roomlevel-1];
    connectToHost();
    QString info = "playerin?" + ourself->getuserId() + "?" + roomlevel + "?" + roomnumber;
    sendMessage(info);
    get_nm_sex(ourself->getuserId());
}

void room::connectToHost()
{
    this->readWriteSocket = new QTcpSocket;
    this->readWriteSocket->connectToHost(QHostAddress("127.0.0.1"),8888);
    //qDebug()<<"connect to host……";
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

void room::get_nm_sex(QString _id)
{
    QString info = "get_name_sex?" + _id;
    sendMessage(info);
}

void room::sendMessage(QString _info)
{
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(_info);
    this->readWriteSocket->write(array->data());
}

void room::recvMessage()
{
    QString info;
    info += this->readWriteSocket->readAll();
    QStringList _infolist = info.split('?');
    if (_infolist.at(0) == "retoget_name_sex")
    {
        ourself->set_Nm_Sex(_infolist.at(1),_infolist.at(2));
    }
}

void room::getbeforeplayer()
{
    //以下都要向服务器请求
    playernumber = 5;
    QString otherid[8] = {"user2","user3","user4","user5"};
    QString othername[8] = {"hyq","fyl","csq","sy"};//列表里的第一个为first_person
    int space[8] = {1,2,3};
    for (int i = 0; i < playernumber - 2; i++)
        playerspace[i] = space[i];
    int othermoney[8] = {1089190000,4522324,232320,23333223};
    for (int i = 0; i < playernumber - 1; i++)
    {
        playerlist[i] = new player(otherid[i]);
        playerlist[i]->set_Nm_Sex(othername[i],"boy");
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
    int loc;
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
        else if (i > loc)
        {
            for (int k = loc; k < i; k++)
                _j += playerspace[k];
        }
        //qDebug()<<playerlist[i]->getuserID()<<playerlist[i]->getmoney();
        if (_j < 0)
            _j = 9 + _j;
        if (_j > 8)
            _j = _j - 9;
        if (i == 0)
            first_player = _j;//列表里的第一个人就是第一个开始的玩家
        qDebug()<<"_j"<<_j;
        name[_j]->setText(playerlist[i]->getuserName());
        money[_j]->setText(moneylabel);
        chouma[_j]->setStyleSheet("border-image: url(:/source/picture/icon/chouma.png);");
        if (playerlist[i]->getuserSex() == "boy")
            head[_j]->setStyleSheet("border-image:url(:/source/picture/icon/boy.jpg);");
        else if (playerlist[i]->getuserSex() == "girl")
            head[_j]->setStyleSheet("border-image:url(:/source/picture/icon/girl1.jpg);");
        else
            head[_j]->setStyleSheet("border-image:url(:/source/picture/icon/dog.jpg);");
        seat[_j] = true;
    }
}

void room::get2owncard()
{
    //border-image: url(:/source/picture/background/button1.jpg);
    //从服务器获取自己的两张牌还有5张公共牌
    QString c1 = "spadeA";
    QString c2 = "club2";
    ourself->setowncard(c1,c2);

    commonCard[0] = "diamond3";
    commonCard[1] = "heartJ";
    commonCard[2] = "club5";
    commonCard[3] = "diamond9";
    commonCard[4] = "heartK";

    qDebug()<<"get 2 card!!!";
    //update();
    for (int i = 0; i <= 8; i++)
    {
        int loc = i + first_player;
        if (loc > 8)
            loc = loc - 9;
        if (seat[loc] == false)
            continue;
        current_player = loc;
        update();
        if (loc != 4)
        {
            //qDebug()<< "loc = "<<loc;
            send2card(loc);
         }
    }
    current_player = -1;
    QLabel *label1 = new QLabel(Card[4]);
    QLabel *label2 = new QLabel(Card[4]);
    label1->setStyleSheet("border-image: url(" + pokerpath + c1 + ".jpg)");
    label2->setStyleSheet("border-image: url(" + pokerpath + c2 + ".jpg)");
    //为了图片看得清楚些，还是不得不强行把这个扑克放大啊
    label1->setGeometry(0,0,70,100);
    label2->setGeometry(30,0,70,100);
}

void room::gamestart()
{
    qDebug()<<"game start!!!";
    prepare_start->hide();
    currentState = 1;
    get2owncard();
    currentState = 2;
    //perflop();
    show1_3card();
    flop();
    show4card();
    turn();
    show5card();
    river();
    end();
    qDebug()<<"game over!!!";
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
    if (currentState == 0)//准备阶段
    {
        //do nothing
        return;
    }
    else if (currentState == 1)
    {//发牌阶段，执行发牌动画

    }
    if (currentState == 2 && buttonclick)//下注阶段，执行时间条的绘制
    {
        QPainter p(this);
        QPen pen = p.pen();
        pen.setColor(Qt::red);
        pen.setWidth(5);
        p.setPen(pen);
        int xy[2][2]={{position_xy[0][current_player]-5,position_xy[0][current_player]+85},{position_xy[1][current_player]-5,position_xy[1][current_player]+105}};

        //初始cnt = 0
        if (cnt < 23){//执行上面一条线的减少
            int x = xy[0][0] + cnt * 4;
            QLine line1(x,xy[1][0],xy[0][1],xy[1][0]);
            p.drawLine(line1);
            QLine line2(xy[0][1],xy[1][0],xy[0][1],xy[1][1]);
            p.drawLine(line2);
            QLine line3(xy[0][1],xy[1][1],xy[0][0],xy[1][1]);
            p.drawLine(line3);
            QLine line4(xy[0][0],xy[1][1],xy[0][0],xy[1][0]);
            p.drawLine(line4);
        }
        else if (cnt < 50){//执行右边线的减少
            int y = xy[1][0] + cnt * 4 - 90;
            QLine line2(xy[0][1],y,xy[0][1],xy[1][1]);
            p.drawLine(line2);
            QLine line3(xy[0][1],xy[1][1],xy[0][0],xy[1][1]);
            p.drawLine(line3);
            QLine line4(xy[0][0],xy[1][1],xy[0][0],xy[1][0]);
            p.drawLine(line4);
        }
        else if (cnt < 73){//执行下边线的减少
            int x = xy[0][1] - cnt * 4 + 200;
            QLine line3(x,xy[1][1],xy[0][0],xy[1][1]);
            p.drawLine(line3);
            QLine line4(xy[0][0],xy[1][1],xy[0][0],xy[1][0]);
            p.drawLine(line4);
        }
        else if(cnt < 100)//执行左边线的减少
        {
            int y = xy[1][1] - cnt * 4 + 290;
            QLine line4(xy[0][0],y,xy[0][0],xy[1][0]);
            p.drawLine(line4);
        }
        else
        {
            //什么都不画？？？
            pen.setWidth(0);
            p.setPen(pen);
            p.drawLine(0,0,0,0);
        }
    }
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

void room::perflop()
{
    int i;
    for (i = current_player+1; ; i++)
    {
        if (i > 8)
            i = i - 9;

        if (seat[i] == false)
            continue;
        else
            break;
    }
    current_player = i;
}

void room::dosomething()
{
    perflop();
    if (current_player == first_player)
        bet(current_player,perflopbet[roomlevel-1]);
    else if (waitsecond){
        bet(current_player,perflopbet[roomlevel-1]*2);
        waitsecond = false;
    }
        //之后的需要向服务器请求了
    else{
        buttonclick = true;
        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(handletime()));
        timer->start(100);
        qDebug()<<"current_player"<<current_player;
        if (current_player == 4)
        {
        //显示button操作
                //sendplayertoserver();

        }
            //getplayerfromserver();
    }
}

void room::handletime()
{
    if (++cnt <= 100)
        update();
    else
    {
        timer->stop();
        buttonclick = false;
        cnt = 0;
    }
}

void room::bet(int _loc, int _number)
{
    _chouma[_loc]->show();
    betnum[_loc]->setText(QString::number(_number,10));
    //playerlist[_loc]->bet();
}

void room::flop()
{

}

void room::turn()
{

}

void room::river()
{

}

void room::end()
{

}

void room::show1_3card()
{
    ui->card_1->setStyleSheet("border-image: url(" + pokerpath + commonCard[0] + ".jpg)");
    ui->card_2->setStyleSheet("border-image: url(" + pokerpath + commonCard[1] + ".jpg)");
    ui->card_3->setStyleSheet("border-image: url(" + pokerpath + commonCard[2] + ".jpg)");
}

void room::show4card()
{
    ui->card_4->setStyleSheet("border-image: url(" + pokerpath + commonCard[3] + ".jpg)");
}

void room::show5card()
{
    ui->card_5->setStyleSheet("border-image: url(" + pokerpath + commonCard[4] + ".jpg)");
}
