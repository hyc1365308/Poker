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

class roomlist : public QDialog
{
    Q_OBJECT

public:
    explicit roomlist(QString _userid,QWidget *parent = 0);
    int getusergold();//向服务器请求用户的筹码数
    void recover();//恢复按钮未被点击时的状态
    //bool eventFilter(QObject *obj, QEvent *ev);//时间监听器
    void enterroom(int i);

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

    void _decrease();

    void _increase();

    void on_pageup_clicked();

    void on_pagedown_clicked();

private:
    Ui::roomlist *ui;
    QDialog *personinfoDialog;
    QDialog *settingDialog;
    QDialog *rankDialog;
    QDialog *tipsDialog;
    QString userID;
    int roomlevel;//标记房间等级
    int current_page;//标记房间页数
    QString roombutton[5][2];//记录button背景路径
    QString roomname[5];
    QPushButton *enterbutton[5];
    QLabel *_roomname[5];
    QLabel *_roomstate[5];
    int roomstate[5][10];
    int pagenumber;
};

#endif // ROOMLIST_H
