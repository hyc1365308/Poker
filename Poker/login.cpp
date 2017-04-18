#include "login.h"
#include "ui_login.h"
#include <QString>
#include <QDialog>
#include <QEvent>
#include <QtGui>
#include <QKeyEvent>
#include "QMessageBox.h"
#include <QDebug>
#include <QGridLayout>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    rememberPd = false;
    autoLogin = false;
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    //规定密码只能为英文大小写字母和数字
    QRegExp regExp("[A-Za-z0-9]+");
    ui->Password->setValidator(new QRegExpValidator(regExp,this));

    //用户id只能是数字，英文大小写字母，英文下划线，汉字
    QRegExp regExp2("^[a-zA-Z0-9_\u4e00-\u9fa5\\w]+$");
    ui->userID->setValidator(new QRegExpValidator(regExp2,this));


    infoDialog = new QDialog(this);
    infoDialog->setWindowTitle("关于我们");
    infoDialog->setStyleSheet("border-image:url();");
    infoDialog->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/source/picture/background/blue.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    infoDialog->setPalette(palette);
    //infoDialog->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    infoDialog->resize(300,200);
    QVBoxLayout *v_layout = new QVBoxLayout;
    v_layout->setSpacing(2);
    QLabel *stuff = new QLabel(infoDialog);
    stuff->setText("组员名单");
    stuff->setAlignment(Qt::AlignCenter);
    stuff->setFont(QFont( "Timers",12,QFont::Bold));
    stuff->setStyleSheet("color:red");//文本颜色
    const QString all_stuff[4][3] = {{"姓名","学号","手机"},{"冯瑜林","2014011365","13141317514"},{"黄越钦","2014011324","13121415804"},{"邹岩松","2014011296","17888834764"}};
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setSpacing(2);
    gridLayout->setMargin(2);
    for (int i = 0;i < 4; i++)
        for (int j = 0;j < 3; j++)
        {
            QLabel *label = new QLabel(all_stuff[i][j]);
            label->setFrameStyle(QFrame::Panel + QFrame::Sunken);
            if (i == 0)
            {
                label->setFont(QFont( "Timers",10,QFont::Bold));
            }
            label->setMinimumSize(20,0);
            label->setAlignment(Qt::AlignCenter);
            gridLayout->addWidget(label,i,j);
        }
    v_layout->addWidget(stuff,0);
    v_layout->addLayout(gridLayout,1);
    infoDialog->setLayout(v_layout);

    signDialog = new QDialog(this);
    signDialog->setWindowTitle("用户注册");
    signDialog->setStyleSheet("border-image:url();");
    signDialog->setAutoFillBackground(true);
    //signDialog->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    QPalette palette1;
    QPixmap pixmap1(":/source/picture/background/danya.jpg");
    palette1.setBrush(QPalette::Window, QBrush(pixmap1));
    signDialog->setPalette(palette1);

    signDialog->resize(300,180);
    //const QString left_label[] = {"用户昵称","输入密码","确认密码","联系邮箱"};

    QGridLayout *gridLayout1 = new QGridLayout;
    gridLayout1->setSpacing(10);
    gridLayout1->setMargin(5);

    QLabel *title = new QLabel("请输入注册信息");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont( "Timers",12,QFont::Bold));
    title->setStyleSheet("color:blue");//文本颜色
    gridLayout1->addWidget(title,0,0,1,2);

    QLabel *label_ID = new QLabel("用户昵称");
    ID = new QLineEdit;
    label_ID->setAlignment(Qt::AlignCenter);
    ID->setPlaceholderText("数字/英文大小写字母/英文下划线/汉字");
    ID->setValidator(new QRegExpValidator(regExp2,this));
    ID->setFocus();
    gridLayout1->addWidget(label_ID,1,0);
    gridLayout1->addWidget(ID,1,1);

    QLabel *label_PW = new QLabel("输入密码");
    PW = new QLineEdit;
    label_PW->setAlignment(Qt::AlignCenter);
    PW->setPlaceholderText("数字/英文大小写字母");
    PW->setEchoMode(QLineEdit::Password);
    PW->setValidator(new QRegExpValidator(regExp,this));
    gridLayout1->addWidget(label_PW,2,0);
    gridLayout1->addWidget(PW,2,1);

    QLabel *label_PW_ = new QLabel("确认密码");
    PW_ = new QLineEdit;
    label_PW_->setAlignment(Qt::AlignCenter);
    PW_->setEchoMode(QLineEdit::Password);
    PW_->setPlaceholderText("数字/英文大小写字母");
    PW_->setValidator(new QRegExpValidator(regExp,this));
    gridLayout1->addWidget(label_PW_,3,0);
    gridLayout1->addWidget(PW_,3,1);

    QLabel *label_EM = new QLabel("联系邮箱");
    EM = new QLineEdit;
    label_EM->setAlignment(Qt::AlignCenter);
    EM->setPlaceholderText("正确邮箱格式");
    gridLayout1->addWidget(label_EM,4,0);
    gridLayout1->addWidget(EM,4,1);

    confirm = new QPushButton("确认注册");
    connect(confirm,SIGNAL(clicked()),this,SLOT(_sign_in()));
    cancel = new QPushButton("重新输入");
    connect(cancel,SIGNAL(clicked()),this,SLOT(_cancel()));

    QHBoxLayout *_button = new QHBoxLayout;
    _button->addWidget(confirm);
    _button->addWidget(cancel);

    gridLayout1->addLayout(_button,5,0,1,2);

    signDialog->setLayout(gridLayout1);

    ui->userID->setFocus();
    ui->info->installEventFilter(this);
    ui->sign_in->installEventFilter(this);
}

login::~login()
{
    delete ui;
}

void login::on_loginButton_clicked()
{
    //对于用户名或者密码可以考虑怎么样从数据库获取
    QString _ID = this->ui->userID->text().trimmed();
    QString PASSWORD = this->ui->Password->text().trimmed();
    //qDebug()<<_ID<<" "<<PASSWORD;

    QString userid = "@@@";
    QString userpd = "@@@";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if (!db.open())
    {
        qDebug("数据库打开失败！！！");
    }
    else
    {
        qDebug("数据库打开成功！！！");
        QSqlQuery query;
        if(!query.exec("SELECT userid,userpd FROM usersdata")){
            qDebug()<< query.lastError().text();
        }
        while(query.next())
        {
            userid = query.value(0).toString();
            userpd = query.value(1).toString();
            if (userid == _ID)
            {
                //qDebug()<< userid<<" "<<userpd;
                break;
            }
            userid = "@@@";
        }
        query.finish();
    }

    //qDebug()<<userid<<" "<<userpd;

    if (_ID == "")
    {
        QMessageBox::warning(NULL,tr("警告"),tr("请先输入用户名和密码!!!"),QMessageBox::Yes);
    }

    else if (userid == "@@@")
    {
        QMessageBox::warning(NULL,tr("警告"),tr("该用户名不存在，请先注册!!!"),QMessageBox::Yes);
    }

    else if (PASSWORD != userpd)
    {
        QMessageBox::warning(NULL,tr("警告"),tr("密码错误!!!"),QMessageBox::Yes);
    }
    else
    {
        if (rememberPd){
            QSettings scfg("config.ini",QSettings::IniFormat);
            scfg.setValue("USERID",userid);
            scfg.setValue("USERPD",userpd);
            scfg.sync();
            rememberPd = false;
        }
        if (autoLogin){
            QSettings scfg("config.ini",QSettings::IniFormat);
            scfg.setValue("AUTOLOGIN",1);
            scfg.sync();
            autoLogin = false;
        }
        userID = userid;
        accept();
        return;
    }
    this->ui->userID->clear();
    this->ui->Password->clear();
    this->ui->userID->setFocus();
}

bool login::eventFilter(QObject *obj, QEvent *ev)
{
    //QDebug()<<obj->objectName();
    if (obj == ui->sign_in && ev->type() == QEvent::MouseButtonPress)
    {
        //进行sign in操作
        QMouseEvent *mouseevent = static_cast<QMouseEvent*>(ev);
        if (mouseevent->buttons()&Qt::LeftButton)
            sign_in();
    }
    else if (obj == this->ui->info && ev->type() == QEvent::MouseButtonPress)
    {
        //进行info显示
        QMouseEvent *mouseevent = static_cast<QMouseEvent*>(ev);
        if (mouseevent->buttons()&Qt::LeftButton)
            show_info();
        //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    }
    return false;
}

void login::show_info()
{
    infoDialog->show();
}

void login::sign_in()
{
    ID->clear();
    PW->clear();
    PW_->clear();
    EM->clear();
    signDialog->show();
}

void login::_sign_in()
{
    QRegExp regExp("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
    if(ID->text()=="")
        QMessageBox::warning(NULL,tr("警告"),tr("请输入昵称！"),QMessageBox::Yes);
    else if(PW->text()=="")
        QMessageBox::warning(NULL,tr("警告"),tr("请输入密码！"),QMessageBox::Yes);
    else if(PW_->text()=="")
        QMessageBox::warning(NULL,tr("警告"),tr("请再次输入密码！"),QMessageBox::Yes);
    else if(EM->text()=="")
        QMessageBox::warning(NULL,tr("警告"),tr("请输入邮箱！"),QMessageBox::Yes);
    else if(PW->text() != PW_->text())
        QMessageBox::warning(NULL,tr("警告"),tr("两次输入密码不一致！"),QMessageBox::Yes);
    else if(!regExp.exactMatch(EM->text()))
    {
        QMessageBox::warning(NULL,tr("警告"),tr("邮箱格式不正确！"),QMessageBox::Yes);
    }
    else
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("database.db");
        if (!db.open())
        {
            qDebug("数据库打开失败！！！");
        }
        else
        {
            qDebug("数据库打开成功！！！");
            QSqlQuery query;

            //以下注释部分为数据库的初始化和测试部分
            //数据库表单名为usersdata
            //包括formalid,userid,userpd,userem,usergold,userwin,userlose
            //分别为传输id，用户昵称,用户密码,用户邮箱,用户金币,用户胜场，用户负场
            /*if (!query.exec("CREATE TABLE usersdata("
                           "formalid VARCHAR,"
                           "userid VARCHAR,"
                           "userpd VARCHAR,"
                           "userem VARCHAR,"
                           "usergold INT,"
                           "userwin INT,"
                           "userlose INT)")){
                qDebug()<< query.lastError().text()<<"\n";
            }
            query.prepare("INSERT INTO usersdata(formalid,userid,userpd,userem,usergold,userwin,userlose) VALUES(?,?,?,?,?,?,?)");
            QVariantList formalids;
            formalids<< "user1" << "user2" << "user3";
            query.addBindValue(formalids);

            QVariantList userids;
            userids<< "zys" << "fyl" << "hyq";
            query.addBindValue(userids);

            QVariantList userpds;
            userpds<< "2014011296" << "2014011365" << "2014011324";
            query.addBindValue(userpds);

            QVariantList userems;
            userems<< "1450214244@qq.com" << "thucoldwind@gmail.com" << "hyc1365308@sina.com";
            query.addBindValue(userems);

            QVariantList usergolds;
            usergolds<< 100000 << 100000 << 100000;
            query.addBindValue(usergolds);

            QVariantList userwins;
            userwins<< 0 << 0 << 0;
            query.addBindValue(userwins);

            QVariantList userloses;
            userloses<< 0 << 0 << 0;
            query.addBindValue(userloses);
            if (!query.execBatch())
            {
                qDebug()<< query.lastError().text()<<"\n";
            }*/

            //进行插入数据库操作
            QString _id = ID->text().trimmed();
            QString _pw = PW->text().trimmed();
            QString _em = EM->text().trimmed();
            if(query.exec("SELECT formalid,userid FROM usersdata")){
                qDebug()<< query.lastError().text()<<"\n";
            }
            QString _formalid;
            QString _userid;
            while(query.next())
            {
                _formalid = query.value(0).toString();
                _userid = query.value(1).toString();
                //qDebug()<< _userid<<" "<<_formalid;
                if (_userid == _id)
                {
                    //qDebug()<< "该昵称已被注册！！"<<_userid;
                    QMessageBox::information(NULL,tr("提醒"),tr("该昵称已经存在！！！"),QMessageBox::Yes);
                    return;
                }
            }
            query.prepare("INSERT INTO usersdata(formalid,userid,userpd,userem,usergold,userwin,userlose) VALUES(?,?,?,?,?,?,?)");
            QString last = _formalid.mid(4);
            int num = last.toInt()+1;
            QString _formalid_ = "user"+QString::number(num,10);
            //qDebug()<<_formalid_;
            query.addBindValue(_formalid_);
            query.addBindValue(_id);
            query.addBindValue(_pw);
            query.addBindValue(_em);
            query.addBindValue(50000);
            query.addBindValue(0);
            query.addBindValue(0);
            if (!query.exec())
                QMessageBox::information(NULL,tr("提醒"),tr("注册失败！！！"),QMessageBox::Yes);
            else
                QMessageBox::information(NULL,tr("提醒"),tr("注册成功！！！"),QMessageBox::Yes);
            query.finish();
        }
    }
}

void login::_cancel()
{
    ID->clear();
    PW->clear();
    PW_->clear();
    EM->clear();
}

void login::on_rememberPassword_clicked()
{
    if (ui->rememberPassword->isChecked())
        rememberPd = true;
}

void login::on_autoLogin_clicked()
{
    if (ui->autoLogin->isChecked())
        autoLogin = true;
}

void login::on_userID_textChanged(const QString &arg1)
{
    QSettings scfg("config.ini",QSettings::IniFormat);
    QString id = scfg.value("USERID").toString();
    if (id == ui->userID->text().trimmed())
    {
        QString pd = scfg.value("USERPD").toString();
        int autologin = scfg.value("AUTOLOGIN").toInt();
        if (autologin == 1)
        {
            accept();
            return;
        }
        else
        {
            ui->rememberPassword->setChecked(true);
            ui->Password->setText(pd);
        }
    }
    else
    {
        ui->Password->clear();
        ui->rememberPassword->setChecked(false);
    }
}

QString login::getuserid()
{
    return userID;
}
