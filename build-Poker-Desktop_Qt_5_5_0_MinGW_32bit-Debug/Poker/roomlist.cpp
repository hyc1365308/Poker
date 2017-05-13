#include "roomlist.h"
#include "ui_roomlist.h"

roomlist::roomlist(QString _userid,QWidget *parent) :
    userId(_userid),QDialog(parent),
    roomstate {{0},{0},{0},{0},{0}},pagenumber(2),
    ui(new Ui::roomlist)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    ui->label_rank->setAlignment(Qt::AlignCenter);
    ui->label_setting->setAlignment(Qt::AlignCenter);
    ui->label_tips->setAlignment(Qt::AlignCenter);
    tipsDialog = new QDialog(this);
    personinfoDialog = new QDialog(this);
    settingDialog = new QDialog(this);
    rankDialog = new QDialog(this);
    init();
//    ui->username->setText(userName);
//    ui->usergold->setText(QString::number(money, 10));
    roomlevel = 0;
    current_page = 0;
    //从服务器获取roomstate???

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 2; j++)
            roombutton[i][j] = backgroundpath + buttonname[i][j];
    QVBoxLayout *roomlistLayout = new QVBoxLayout;
    for (int i = 0; i < 5; i++)
    {
        QHBoxLayout *singleroom = new QHBoxLayout;
        _roomname[i] = new QLabel;
        _roomname[i]->setAlignment(Qt::AlignCenter);
        _roomname[i]->setFont(QFont( "方正姚体",24));
        //_roomname->setStyleSheet("background-color:orange;color:blue");
        _roomname[i]->setStyleSheet("color: rgb(85, 0, 0);border-image: url(:/source/picture/background/singalroom.jpg);");
        _roomname[i]->setMinimumHeight(40);

        _roomstate[i] = new QLabel;
        _roomstate[i]->setAlignment(Qt::AlignCenter);
        _roomstate[i]->setFont(QFont("宋体",20));
        _roomstate[i]->setStyleSheet("color: rgb(85, 0, 0);border-image: url(:/source/picture/background/singalroom.jpg);");
        _roomstate[i]->setMinimumHeight(40);
        _roomstate[i]->setMaximumWidth(60);

        enterbutton[i] = new QPushButton("进入房间");
        enterbutton[i]->setMaximumWidth(120);
        enterbutton[i]->setMinimumHeight(40);
        enterbutton[i]->setFont(QFont("楷体",20));
        enterbutton[i]->setCursor(Qt::PointingHandCursor);
        enterbutton[i]->setStyleSheet("background-color: rgb(255, 85, 0);");
        //connect(enterbutton[i],SIGNAL(clicked()),this,SLOT(enterroom()));
        singleroom->addWidget(_roomname[i],0);
        singleroom->addWidget(_roomstate[i],1);
        singleroom->addWidget(enterbutton[i],2);
        singleroom->setSpacing(2);

        roomlistLayout->setSpacing(30);
        roomlistLayout->setMargin(20);
        roomlistLayout->addLayout(singleroom,i-1);

        //enterbutton[i]->installEventFilter(this);
    }
    connect(enterbutton[0],SIGNAL(clicked()),this,SLOT(enterroom1()));
    connect(enterbutton[1],SIGNAL(clicked()),this,SLOT(enterroom2()));
    connect(enterbutton[2],SIGNAL(clicked()),this,SLOT(enterroom3()));
    connect(enterbutton[3],SIGNAL(clicked()),this,SLOT(enterroom4()));
    connect(enterbutton[4],SIGNAL(clicked()),this,SLOT(enterroom5()));

    ui->room_list->setLayout(roomlistLayout);

    ui->_currentpage->setAlignment(Qt::AlignCenter);
    ui->_currentpage->setFont(QFont("隶书",20));
    ui->_currentpage->setStyleSheet("color: rgb(85, 0, 0);border-image: url(:/source/picture/background/singalroom.jpg);");


    //connect(pageup,SIGNAL(clicked()),this,SLOT(_decrease()));
    //connect(pagedown,SIGNAL(clicked()),this,SLOT(_increase()));
}

roomlist::~roomlist()
{
    delete ui;
}

void roomlist::connectHost()
{
    this->readWriteSocket = new QTcpSocket;
    this->readWriteSocket->connectToHost(QHostAddress("127.0.0.1"),8888);
    //qDebug()<<"connect to host……";
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

void roomlist::on_rank_clicked()
{

}

void roomlist::on_setting_clicked()
{

}

void roomlist::on_tips_clicked()
{
    //提示最低准入
    tipsDialog->setWindowTitle("游戏提示");
    tipsDialog->setStyleSheet("border-image:url();");
    tipsDialog->setAutoFillBackground(true);
    //tipsDialog->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    QPalette palette1;
    QPixmap pixmap1(":/source/picture/background/danya.jpg");
    palette1.setBrush(QPalette::Window, QBrush(pixmap1));
    tipsDialog->setPalette(palette1);

    tipsDialog->resize(480,300);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setSpacing(10);
    gridLayout->setMargin(10);
    QLabel *title = new QLabel("房间等级须知");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont( "Timers",24,QFont::Bold));
    title->setStyleSheet("color:blue");//文本颜色
    gridLayout->addWidget(title,0,0,1,3);

    QLabel *roomlv = new QLabel("房间等级");
    QLabel *roomlv_chip = new QLabel("大小盲注");
    QLabel *roomlv_takechip = new QLabel("携带筹码");
    roomlv->setAlignment(Qt::AlignCenter);
    roomlv_chip->setAlignment(Qt::AlignCenter);
    roomlv_takechip->setAlignment(Qt::AlignCenter);
    roomlv->setFont(QFont( "Timers",20,QFont::Bold));
    roomlv_chip->setFont(QFont( "Timers",20,QFont::Bold));
    roomlv_takechip->setFont(QFont( "Timers",20,QFont::Bold));
    gridLayout->addWidget(roomlv,1,0);
    gridLayout->addWidget(roomlv_chip,1,1);
    gridLayout->addWidget(roomlv_takechip,1,2);

    QString _room_level[] = {"倔强青铜","秩序白银","荣耀黄金","尊贵铂金","永恒钻石"};
    QString _room_chip[] = {"100/200","500/1000","1000/2000","5000/10000","10000/20000"};
    QString _room_takechip[] = {"1万","5万","10万","50万","100万"};
    for (int i = 0; i < 5; i++)
    {
        QLabel *room = new QLabel(_room_level[i]);
        QLabel *room_chip = new QLabel(_room_chip[i]);
        QLabel *room_takechip = new QLabel(_room_takechip[i]);
        room->setAlignment(Qt::AlignCenter);
        room_chip->setAlignment(Qt::AlignCenter);
        room_takechip->setAlignment(Qt::AlignCenter);

        room->setFont(QFont( "Timers",16));
        room_chip->setFont(QFont( "Timers",16));
        room_takechip->setFont(QFont( "Timers",16));
        gridLayout->addWidget(room,i+2,0);
        gridLayout->addWidget(room_chip,i+2,1);
        gridLayout->addWidget(room_takechip,i+2,2);
    }
    tipsDialog->setLayout(gridLayout);
    tipsDialog->show();
}

void roomlist::on_roomlist1_clicked()
{
    if (money < 10000)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("最低准入一万，你的钱太少了，充值之后再玩吧！"),QMessageBox::Yes);
        return;
    }
    if (roomlevel != 1)
    {
        current_page = 0;
        //qDebug()<<"current page"<<current_page;
        ui->_currentpage->setText("第"+QString::number(current_page+1,10)+"页");
        ui->roomlist1->setStyleSheet("border-image:url("+roombutton[0][1]+");");
        recover();
        roomlevel = 1;
        for (int i = 0; i < 5; i++){
            qDebug()<<roomstate[roomlevel-1][i+5*current_page];
            _roomstate[i]->setText(QString::number(roomstate[roomlevel-1][i+5*current_page],10) + "/9");
            _roomname[i]->setText(roomname[roomlevel-1] + QString::number(i+1+5*current_page,10));
        }
        //ui->room_list->show();
    }
}

void roomlist::on_roomlist2_clicked()
{
    if (money < 50000)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("最低准入5万，你的钱太少了，充值之后再玩吧！"),QMessageBox::Yes);
        return;
    }
    if (roomlevel != 2)
    {
        current_page = 0;
        ui->_currentpage->setText("第"+QString::number(current_page+1,10)+"页");
        ui->roomlist2->setStyleSheet("border-image:url("+roombutton[1][1]+");");
        recover();
        roomlevel = 2;
        for (int i = 0; i < 5; i++)
        {
            _roomstate[i]->setText(QString::number(roomstate[roomlevel-1][i+5*current_page],10) + "/9");
            _roomname[i]->setText(roomname[roomlevel-1] + QString::number(i+1+5*current_page,10));
        }
    }
}

void roomlist::on_roomlist3_clicked()
{
    if (money < 100000)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("最低准入十万，你的钱太少了，充值之后再玩吧！"),QMessageBox::Yes);
        return;
    }
    if (roomlevel != 3)
    {
        current_page = 0;
        ui->_currentpage->setText("第"+QString::number(current_page+1,10)+"页");
        ui->roomlist3->setStyleSheet("border-image:url("+roombutton[2][1]+");");
        recover();
        roomlevel = 3;
        for (int i = 0; i < 5; i++)
        {
            _roomstate[i]->setText(QString::number(roomstate[roomlevel-1][i+5*current_page],10) + "/9");
            _roomname[i]->setText(roomname[roomlevel-1] + QString::number(i+1+5*current_page,10));
        }
    }
}

void roomlist::on_roomlist4_clicked()
{
    if (money < 500000)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("最低五十万，你的钱太少了，充值之后再玩吧！"),QMessageBox::Yes);
        return;
    }
    if (roomlevel != 4)
    {
        current_page = 0;
        ui->_currentpage->setText("第"+QString::number(current_page+1,10)+"页");
        ui->roomlist4->setStyleSheet("border-image:url("+roombutton[3][1]+");");
        recover();
        roomlevel = 4;
        for (int i = 0; i < 5; i++)
        {
            _roomstate[i]->setText(QString::number(roomstate[roomlevel-1][i+5*current_page],10) + "/9");
            _roomname[i]->setText(roomname[roomlevel-1] + QString::number(i+1+5*current_page,10));
        }
    }
}

void roomlist::on_roomlist5_clicked()
{
    if (money < 1000000)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("最低准入一百万，你的钱太少了，充值之后再玩吧！"),QMessageBox::Yes);
        return;
    }
    if (roomlevel != 5)
    {
        current_page = 0;
        ui->_currentpage->setText("第"+QString::number(current_page+1,10)+"页");
        ui->roomlist5->setStyleSheet("border-image:url("+roombutton[4][1]+");");
        recover();
        roomlevel = 5;
        for (int i = 0; i < 5; i++)
        {
            _roomstate[i]->setText(QString::number(roomstate[roomlevel-1][i+5*current_page],10) + "/9");
            _roomname[i]->setText(roomname[roomlevel-1] + QString::number(i+1+5*current_page,10));
        }
    }
}


void roomlist::init()
{
    connectHost();
    QString info = "get_name_gold_sex?" + userId;
    qDebug()<<"send info:"<<info;
    sendMessage(info);
}

void roomlist::recover()
{
    switch (roomlevel) {
    case 1:
        ui->roomlist1->setStyleSheet("border-image:url("+roombutton[0][0]+");");
        break;
    case 2:
        ui->roomlist2->setStyleSheet("border-image:url("+roombutton[1][0]+");");
        break;
    case 3:
        ui->roomlist3->setStyleSheet("border-image:url("+roombutton[2][0]+");");
        break;
    case 4:
        ui->roomlist4->setStyleSheet("border-image:url("+roombutton[3][0]+");");
        break;
    case 5:
        ui->roomlist5->setStyleSheet("border-image:url("+roombutton[4][0]+");");
        break;
    default:
        break;
    }

}


void roomlist::enterroom1()
{
    enterroom(1 + current_page * 5);
}

void roomlist::enterroom2()
{
    enterroom(2 + current_page * 5);
}

void roomlist::enterroom3()
{
    enterroom(3 + current_page * 5);
}

void roomlist::enterroom4()
{
    enterroom(4 + current_page * 5);
}

void roomlist::enterroom5()
{
    enterroom(5 + current_page * 5);
}

void roomlist::enterroom(int i)
{
    qDebug()<<"enter room "<<roomname[roomlevel-1]<<i;
    roomnumber = i;
    accept();
}

void roomlist::on_pageup_clicked()
{
    if (current_page <= 0)
        return;
    current_page--;
    ui->_currentpage->setText("第"+QString::number(current_page+1,10)+"页");
    for (int i = 0; i < 5; i++)
    {
        _roomstate[i]->setText(QString::number(roomstate[roomlevel-1][i+5*current_page],10) + "/9");
        _roomname[i]->setText(roomname[roomlevel-1] + QString::number(i+1+5*current_page,10));
    }
}

void roomlist::on_pagedown_clicked()
{
    if (current_page >= pagenumber-1)
        return;
    current_page++;
    ui->_currentpage->setText("第"+QString::number(current_page+1,10)+"页");
    for (int i = 0; i < 5; i++)
    {
        _roomstate[i]->setText(QString::number(roomstate[roomlevel-1][i+5*current_page],10) + "/9");
        _roomname[i]->setText(roomname[roomlevel-1] + QString::number(i+1+5*current_page,10));
    }
}

QString roomlist::getuserId()
{
    return userId;
}

QString roomlist::getuserName()
{
    return userName;
}

QString roomlist::getuserSex()
{
    return userSex;
}

int roomlist::getroomlevel()
{
    return roomlevel;
}

int roomlist::getroomnumber()
{
    return roomnumber;
}

void roomlist::sendMessage(QString _info)
{
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(_info);
    this->readWriteSocket->write(array->data());
}

void roomlist::recvMessage()
{
    QString info;
    info += this->readWriteSocket->readAll();
    QStringList _infolist = info.split('?');
    if (_infolist.at(0) == "retoget_name_gold_sex")
    {
        userName = _infolist.at(1);
        QString _money = _infolist.at(2);
        money = _money.toInt();
        ui->username->setText(userName);
        ui->usergold->setText(_money);
        userSex = _infolist.at(3);
        if (userSex == "boy")
            ui->person->setStyleSheet("border-image: url(:/source/picture/icon/boy.jpg);");
        else if (userSex == "girl")
            ui->person->setStyleSheet("border-image: url(:/source/picture/icon/girl1.jpg);");
        else if (userSex == "unknown")
            ui->person->setStyleSheet("border-image: url(:/source/picture/icon/dog.jpg);");
        QString info = "get_roomstate";
        sendMessage(info);
    }
    if (_infolist.at(0) == "retoget_roomstate")
    {
        for (int i = 1; i <= 50; i++){
            QString temp = _infolist.at(i);
            //qDebug()<<temp;
            roomstate[(i-1)/10][(i-1)%10] = temp.toInt();
        }
        on_roomlist1_clicked();
    }
}

