#ifndef ROOMLIST_H
#define ROOMLIST_H

#include <QDialog>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class roomlist;
}

const QString roomname[] = {"倔强青铜","秩序白银","荣耀黄金","尊贵铂金","永恒钻石"};
class roomlist : public QDialog
{
    Q_OBJECT

public:
    explicit roomlist(QString _userid,QString _formalid,QWidget *parent = 0);
    int getusergold();//向服务器请求用户的筹码数
    void recover();//恢复按钮未被点击时的状态
    //bool eventFilter(QObject *obj, QEvent *ev);//时间监听器
    void enterroom(int i);
    QString getID();
    int getroomlevel();
    int getroomnumber();
    QString getformalID();
    ~roomlist();

private slots:
    void on_rank_clicked();

    void on_setting_clicked();

    void on_tips_clicked();

    void on_roomlist1_clicked();

    void on_roomlist2_clicked();

    void on_roomlist3_clicked();

    void on_roomlist4_clicked();

    void on_roomlist5_clicked();

    void enterroom1();

    void enterroom2();

    void enterroom3();

    void enterroom4();

    void enterroom5();

    void on_pageup_clicked();

    void on_pagedown_clicked();

private:
    Ui::roomlist *ui;
    QDialog *personinfoDialog;
    QDialog *settingDialog;
    QDialog *rankDialog;
    QDialog *tipsDialog;
    QString userID;
    QString formalID;
    int roomlevel;//标记房间等级
    int current_page;//标记房间页数
    int roomnumber;
    QString roombutton[5][2];//记录button背景路径
    QPushButton *enterbutton[5];
    QLabel *_roomname[5];
    QLabel *_roomstate[5];
    int roomstate[5][10];
    int pagenumber;
};

#endif // ROOMLIST_H
