#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QtSql>
#include <QString>
#include <QEvent>
#include <QtGui>
#include <QKeyEvent>
#include "QMessageBox.h"
#include <QDebug>
#include <QGridLayout>
#include <QStringList>
#include <QDir>
#include <QFileDialog>
#include <QComboBox>
#include <QPixmap>
#include <QVariantList>
#include <QVariant>
#include <QScrollArea>
#include <QtNetwork>
#include <QTcpSocket>

namespace Ui {
class login;
}

const QString all_stuff[4][3] = {{"姓名","学号","手机"},{"冯瑜林","2014011365","13141317514"},{"黄越钦","2014011324","13121415804"},{"邹岩松","2014011296","17888834764"}};
class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

    void connectHost();
    void sendMessage(QString _info);
    void _sign(QString _nm, QString _pd, QString _em, QString _tel, QString _sex);

private slots:
    void on_loginButton_clicked();
    bool eventFilter(QObject *obj, QEvent *ev);
    void show_info();
    void sign_in();

    void on_userID_textChanged(const QString &arg1);

    //void changePhoto(int index);

    void recvMessage();


public:
    QString getuserid();

public slots:
    void _sign_in();
    void _cancel();


private:
    Ui::login *ui;
    QDialog *infoDialog;
    QDialog *signDialog;
    QLineEdit *NM;//用户注册框输入昵称
    QLineEdit *PW;//用户注册框输入密码
    QLineEdit *PW_;//用户注册框在此输入密码
    QLineEdit *EM;//用户注册框输入email
    QLineEdit *TEL;//用户注册框输入手机号码
    QComboBox *SEX;//用户选择图片路径
    QPushButton *confirm;//用户注册框确认按钮
    QPushButton *cancel;//用户注册框重新输入按钮
    QString userId;
    //QString userName;
    QStringList record;//记录的人tel或em
    QStringList record_pd;//记录的人密码
    //QStringList autologin;//记录自动登录

    QTcpSocket *readWriteSocket;
};

#endif // LOGIN_H
