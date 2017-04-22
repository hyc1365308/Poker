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

        roomlist _roomlist(_login.getuserid());//游戏大厅界面
        _roomlist.setFixedSize(1200,700);
        while(_roomlist.exec() == QDialog::Accepted)
        {
            room r(_roomlist.getuserId(),_roomlist.getroomlevel(),_roomlist.getroomnumber());//游戏房间界面
            r.setFixedSize(1200,700);
            r.gamestart();
            if(r.exec() == QDialog::Accepted)
                return a.exec();
        }
    }
    return 0;
}
