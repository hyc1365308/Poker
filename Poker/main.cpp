#include "mainwindow.h"
#include "login.h"
#include "roomlist.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    login _login; //登陆界面
    _login.setFixedSize(400,300);
    while(_login.exec() == QDialog::Accepted)
    {

        roomlist _roomlist(_login.getuserid());
        _roomlist.setFixedSize(1200,700);
        while(_roomlist.exec() == QDialog::Accepted)
        {
            w.show();
            return a.exec();
        }
    }
    return 0;
}
