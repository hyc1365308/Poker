#include "server.h"
#include "ui_server.h"

server::server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    sql_op = new sqloperation();
    roomlist = new QList<QTcpSocket*>;
    initroomstate();
    initServer();
//    QString localHostName = QHostInfo::localHostName();
//    qDebug()<<localHostName;
//    QHostInfo localHostInfo = QHostInfo::fromName(localHostName);
//    foreach (QHostAddress address, localHostInfo.addresses()) {
//        if (address.protocol() == QAbstractSocket::IPv4Protocol)
//            qDebug()<<address.toString();
//    }
}

server::~server()
{
    delete ui;
}

void server::initroomstate()
{
    room1 = new QList<room *>;
    for (int i = 0; i < 10; i++)
    {
        room *temp = new room(1);
        room1->append(temp);
    }
    room2 = new QList<room *>;
    for (int i = 0; i < 10; i++)
    {
        room *temp = new room(2);
        room2->append(temp);
    }
    room3 = new QList<room *>;
    for (int i = 0; i < 10; i++)
    {
        room *temp = new room(3);
        room3->append(temp);
    }
    room4 = new QList<room *>;
    for (int i = 0; i < 10; i++)
    {
        room *temp = new room(4);
        room4->append(temp);
    }
    room5 = new QList<room *>;
    for (int i = 0; i < 10; i++)
    {
        room *temp = new room(5);
        room5->append(temp);
    }
}

void server::initServer()
{
    this->listenSocket =new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,8888);
    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    qDebug()<<"Wait for connection!";
//    this->_listenSocket = new QTcpServer;
//    this->_listenSocket->listen(QHostAddress::Any,8889);
//    QObject::connect(this->_listenSocket,SIGNAL(newConnection()),this,SLOT(_acceptConnection()));
}

void server::acceptConnection()
{
    this->readWriteSocket =this->listenSocket->nextPendingConnection();
    qDebug()<<"new connection";
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

//void server::_acceptConnection()
//{
//    QTcpSocket *temp = new QTcpSocket();
//    temp = this->_listenSocket->nextPendingConnection();
//    QObject::connect(temp,SIGNAL(readyRead()),this,SLOT(recvMessage()));
//    roomlist->append(temp);
//}

void server::recvMessage()
{
    QString info;
    info += this->readWriteSocket->readAll();
    doSomething(info);
}

void server::doSomething(QString _info)
{
    QStringList _infolist = _info.split('?');
    if (_infolist.at(0) == "login")
    {
        _login(_infolist.at(1),_infolist.at(2));
        QString info_ = _infolist.at(1) + "请求登录";
        ui->infolist->append(info_);
    }
    else if (_infolist.at(0) == "sign")
    {
        QString info_ = _infolist.at(1) +" " +_infolist.at(3) + " " + _infolist.at(4) + _infolist.at(5) + "请求注册";
        ui->infolist->append(info_);
        _sign(_infolist.at(1),_infolist.at(2),_infolist.at(3),_infolist.at(4),_infolist.at(5));
    }
    else if (_infolist.at(0) == "get_name_gold_sex")
    {
        QString info_ = _infolist.at(1) + "请求查询昵称和金币";
        ui->infolist->append(info_);
        get_name_gold_sex(_infolist.at(1));
    }
    else if (_infolist.at(0) == "get_roomstate")
    {
        QString info_ = "retoget_roomstate";
        ui->infolist->append(info_);
        for (int i = 0; i < 10; i++)
            info_ += "?" + QString::number(room1->at(i)->get_playernum(),10);
        for (int i = 0; i < 10; i++)
            info_ += "?" + QString::number(room2->at(i)->get_playernum(),10);
        for (int i = 0; i < 10; i++)
            info_ += "?" + QString::number(room3->at(i)->get_playernum(),10);
        for (int i = 0; i < 10; i++)
            info_ += "?" + QString::number(room4->at(i)->get_playernum(),10);
        for (int i = 0; i < 10; i++)
            info_ += "?" + QString::number(room5->at(i)->get_playernum(),10);
        sendmessage(info_);
    }
    else if (_infolist.at(0) == "get_name_sex")
    {
        QString info_ = "retoget_name_sex?" + sql_op->getNm_sex(_infolist.at(1));
        sendmessage(info_);
    }
    else if (_infolist.at(0) == "playerin")
    {
        QString temp1 = _infolist.at(2);
        QString temp2 = _infolist.at(3);
        setroom(_infolist.at(1),temp1.toInt(),temp2.toInt());
    }
}

void server::_login(QString _key,QString _pd)
{
    QString ans = sql_op->checkPd(_key,_pd);
    sendmessage(ans);
}

void server::setroom(QString _userid, int _roomlevel, int _roomnumber)
{
    switch(_roomlevel)
    {
    case 1:
        room1->at(_roomnumber)->playerin(_userid);
        break;
    case 2:
        room2->at(_roomnumber)->playerin(_userid);
        break;
    case 3:
        room3->at(_roomnumber)->playerin(_userid);
        break;
    case 4:
        room4->at(_roomnumber)->playerin(_userid);
        break;
    case 5:
        room5->at(_roomnumber)->playerin(_userid);
        break;
    }
}

void server::_sign(QString _nm, QString _pd, QString _em, QString _tel, QString _sex)
{
    QString ans = "retosign?";
    if (sql_op->findem(_em))
        ans += "邮箱" + _em;
    if (sql_op->findtel(_tel))
        ans += "手机" + _tel;
    if (!sql_op->findem(_em) && !sql_op->findtel(_tel))
    {
        if (sql_op->insert(_nm, _pd,_em,_tel,_sex))
            ans += "okay";
        else
            ans += "fail";
    }
    sendmessage(ans);
}

void server::sendmessage(QString _info)
{
    QByteArray *array =new QByteArray;
    qDebug()<<"send message:"<<_info;
    array->clear();
    array->append(_info);
    this->readWriteSocket->write(array->data());
}

void server::get_name_gold_sex(QString _id)
{
    QString ans = "retoget_name_gold_sex?";
    ans = ans + sql_op->getNm_gold_sex(_id);
    sendmessage(ans);
}


