#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QtSql>
#include <QSettings>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private slots:
    void on_loginButton_clicked();
    bool eventFilter(QObject *obj, QEvent *ev);
    void show_info();
    void sign_in();

    void on_rememberPassword_clicked();

    void on_autoLogin_clicked();

    void on_userID_textChanged(const QString &arg1);

public:
    QString getuserid();

public slots:
    void _sign_in();
    void _cancel();


private:
    Ui::login *ui;
    QDialog *infoDialog;
    QDialog *signDialog;
    QLineEdit *ID;//用户注册框输入ID
    QLineEdit *PW;//用户注册框输入密码
    QLineEdit *PW_;//用户注册框在此输入密码
    QLineEdit *EM;//用户注册框输入email
    QPushButton *confirm;//用户注册框确认按钮
    QPushButton *cancel;//用户注册框重新输入按钮
    QString userID;
    bool rememberPd;
    bool autoLogin;
};

#endif // LOGIN_H
