#include "mainwindow.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    login _login; //登陆界面
    _login.setFixedSize(400,300);
    if(_login.exec() == QDialog::Accepted)
    {
        w.show();
        return a.exec();
    }
    else
        return 0;

}
