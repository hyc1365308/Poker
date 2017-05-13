#include "sqloperation.h"

sqloperation::sqloperation()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if (!db.open())
    {
        qDebug("数据库打开失败！！！");
    }
    else
    {
        QSqlQuery query;
        if(!query.exec("SELECT userid,username,userpd,userem,usertel FROM usersdata"))
            qDebug()<< query.lastError().text();
        while(query.next())
        {
            QString userid = query.value(0).toString();
            QString username = query.value(1).toString();
            QString userpd = query.value(2).toString();
            QString userem = query.value(3).toString();
            QString usertel = query.value(4).toString();
            idList.append(userid);
            nmList.append(username);
            pdList.append(userpd);
            emList.append(userem);
            telList.append(usertel);
        }
        query.finish();
    }
    currentsize = idList.size();
}

sqloperation::~sqloperation()
{
    db.close();
}

QString sqloperation::checkPd(QString _according,QString _pd)
{
    bool flag = false;
    QString ans = "retologin";
    for (int i = 0; i < emList.size(); i++)
    {
        if ((emList.at(i) == _according)|| (telList.at(i) == _according ))
        {
            flag = true;
            if (pdList.at(i) == _pd )
                ans = ans + "?exist" + "?right?" + idList.at(i);
            else
                ans = ans + "?exist" + "?wrong";
            break;
        }
    }
    if (!flag)
        ans = ans + "?inexist";
    return ans;
}

QString sqloperation::getNm_gold_sex(QString _according)
{
    QSqlQuery query;
    if(query.exec("SELECT userid,username,usergold,usersex FROM usersdata"))
    {
        while(query.next())
        {
            QString userid = query.value(0).toString();
            QString username = query.value(1).toString();
            int usergold = query.value(2).toInt();
            QString usersex = query.value(3).toString();
            if (userid == _according)
            {
                QString ans = username + "?" + QString::number(usergold,10) + "?" + usersex;
                query.finish();
                return ans;
            }
        }
    }
    else
        return ("none");
}

QString sqloperation::getNm_sex(QString _according)
{
    QSqlQuery query;
    if(query.exec("SELECT userid,username,usersex FROM usersdata"))
    {
        while(query.next())
        {
            QString userid = query.value(0).toString();
            QString username = query.value(1).toString();
            QString usersex = query.value(2).toString();
            if (userid == _according)
            {
                QString ans = username + "?" + usersex;
                query.finish();
                return ans;
            }
        }
    }
    else
        return ("none");
}

bool sqloperation::insert(QString _nm,QString _pd,QString _em,QString _tel,QString _sex)
{
    QSqlQuery query;
    QString _id = "user" + QString::number(++currentsize,10);
    //这里可以根据sex选择男女头像，如果不说性别，就给一个动物的头像
    query.prepare("INSERT INTO usersdata(userid,username,userpd,userem,usertel,usersex,usergold,userwin,userlose) VALUES(?,?,?,?,?,?,?,?,?)");
//    QString _imagePath = "D:/poker/source/picture/icon/beauty.jpg";
//    QByteArray _data;
//    QFile* _file=new QFile(_imagePath); //file为二进制数据文件名
//    _file->open(QIODevice::ReadOnly);
//    _data = _file->readAll();
//    _file->close();
//    QVariant _var(_data);
    if (_sex == "我是汉纸")
        _sex = "boy";
    else if (_sex == "我是妹纸")
        _sex = "girl";
    else if (_sex == "我不告诉你")
        _sex = "unknown";
    query.addBindValue(_id);
    query.addBindValue(_nm);
    query.addBindValue(_pd);
    query.addBindValue(_em);
    query.addBindValue(_tel);
    query.addBindValue(_sex);
    query.addBindValue(50000);
    query.addBindValue(0);
    query.addBindValue(0);
    idList.append(_id);
    nmList.append(_nm);
    pdList.append(_pd);
    emList.append(_em);
    telList.append(_tel);
    if (!query.exec())
        return false;
    else
        return true;
}

QString sqloperation::getWin_lose(QString _according)
{

}

bool sqloperation::findem(QString em)
{
    for (int i = 0; i < emList.size(); i++)
    {
        if (em == emList.at(i))
            return true;
    }
    return false;
}

bool sqloperation::findtel(QString tel)
{
    for (int i = 0; i < telList.size(); i++)
    {
        if (tel == telList.at(i))
            return true;
    }
    return false;
}
