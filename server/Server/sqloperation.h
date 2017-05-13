#ifndef SQLOPERATION_H
#define SQLOPERATION_H

#include <QString>
#include <QStringList>
#include <QtSql>

const QString sql_insert = "INSERT INTO usersdata(userid,username,userpd,userem,usertel,userphoto,usergold,userwin,userlose) VALUES(?,?,?,?,?,?,?,?,?)";
class sqloperation
{
public:
    sqloperation();
    ~sqloperation();
    QString checkPd(QString _according,QString _pd);
    bool insert(QString _nm,QString _pd,QString _em,QString _tel,QString _sex);
    QString getNm_gold_sex(QString _according);
    QString getNm_sex(QString _according);
    QString getWin_lose(QString _according);
    bool findem(QString em);
    bool findtel(QString tel);

private:
    QSqlDatabase db;
    QStringList idList;
    QStringList nmList;
    QStringList pdList;
    QStringList emList;
    QStringList telList;
    int currentsize;
};

#endif // SQLOPERATION_H
