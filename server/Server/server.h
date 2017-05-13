#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QtNetwork>
#include <QTcpSocket>
#include <QString>
#include <QDebug>
#include <QHostInfo>
#include <QHostAddress>
#include <QStringList>
#include <QList>
#include "sqloperation.h"
#include "room.h"

namespace Ui {
class server;
}

class server : public QMainWindow
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = 0);
    ~server();
    void initServer();
    void doSomething(QString _info);
    void _login(QString _key,QString _pd);
    void _sign(QString _nm,QString _pd,QString _em,QString _tel,QString _sex);
    void get_name_gold_sex(QString _id);
    void get_name_sex(QString _id);
    void insert(QString _info);
    void sendmessage(QString _info);
    void get_photo(QString _id);
    void initroomstate();
    void setroom(QString _userid, int _roomlevel, int _roomnumber);

public slots:
    void acceptConnection();
    //void _acceptConnection();
    void recvMessage();
    //void _recvMessage();

private:
    Ui::server *ui;  
    QTcpServer  *listenSocket;
    QTcpSocket  *readWriteSocket;//8888
    QTcpServer *_listenSocket;
    QList<QTcpSocket *> *roomlist;//8889
    sqloperation *sql_op;
    QList<room *> *room1;//分别为5个等级的房间
    QList<room *> *room2;
    QList<room *> *room3;
    QList<room *> *room4;
    QList<room *> *room5;
};

#endif // SERVER_H
