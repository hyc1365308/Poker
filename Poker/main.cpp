#include "login.h"
#include "roomlist.h"
#include "room.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    QApplication a(argc, argv);
    login _login; //登陆界面
    _login.setFixedSize(400,300);
    while(_login.exec() == QDialog::Accepted)
    {

        roomlist _roomlist(_login.getuserid(),_login.getformalid());
        _roomlist.setFixedSize(1200,700);
        while(_roomlist.exec() == QDialog::Accepted)
        {
            room r(_roomlist.getID(),_roomlist.getformalID(),_roomlist.getroomlevel(),_roomlist.getroomnumber());
            r.setFixedSize(1200,700);
            r.gamestart();
            if(r.exec() == QDialog::Accepted)
                return a.exec();
        }
    }
    return 0;
}
