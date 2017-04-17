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
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    //规定密码只能为英文大小写字母和数字
    QRegExp regExp("[A-Za-z0-9]+");
    ui->Password->setValidator(new QRegExpValidator(regExp,this));

    //用户id只能是数字，英文大小写字母，英文下划线，汉字
    QRegExp regExp2("^[a-zA-Z0-9_\u4e00-\u9fa5\\w]+$");
    ui->userID->setValidator(new QRegExpValidator(regExp2,this));

    ui->info->installEventFilter(this);
    ui->sign_in->installEventFilter(this);
    ui->info->show();
    ui->sign_in->show();
}

login::~login()
{
    delete ui;
}

void login::on_loginButton_clicked()
{
    //对于用户名或者密码可以考虑怎么样从数据库获取
    QString ID = this->ui->userID->text().trimmed();
    QString PASSWORD = this->ui->Password->text().trimmed();
    if (ID == tr(""))
    {
        QMessageBox::warning(NULL,tr("警告"),tr("请先输入用户名和密码!!!"),QMessageBox::Yes);
    }

    else if (ID != tr("admin"))//之后改为在数据库中查找语句
    {
        QMessageBox::warning(NULL,tr("警告"),tr("该用户名不存在，请先注册!!!"),QMessageBox::Yes);
    }

    else if (PASSWORD != tr("admin"))
    {
        QMessageBox::warning(NULL,tr("警告"),tr("密码错误!!!"),QMessageBox::Yes);
    }
    else
    {
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
        QDialog *signDialog = new QDialog(this);
        sign_in(signDialog);
    }
    else if (obj == this->ui->info && ev->type() == QEvent::MouseButtonPress)
    {
        //进行info显示
        QDialog *infoDialog = new QDialog(this);
        show_info(infoDialog);
        //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    }
    return false;
}

void login::show_info(QDialog *infoDialog)
{
    infoDialog->setWindowTitle("关于我们");
    infoDialog->setStyleSheet("border-image:url();");
    infoDialog->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/source/picture/background/blue.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    infoDialog->setPalette(palette);

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
    infoDialog->show();
}

void login::sign_in(QDialog *signDialog)//有待完善
{
    signDialog->setWindowTitle("用户注册");
    signDialog->setStyleSheet("border-image:url();");
    signDialog->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/source/picture/background/danya.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    signDialog->setPalette(palette);

    signDialog->resize(300,300);
    //const QString left_label[] = {"用户昵称","输入密码","确认密码","联系邮箱"};
    QVBoxLayout *v_layout = new QVBoxLayout;
    v_layout->setSpacing(10);
    v_layout->setMargin(2);
    QLabel *label_ID = new QLabel("用户昵称");
    QLabel *label_PW = new QLabel("输入密码");
    QLabel *label_PW_ = new QLabel("确认密码");
    QLabel *label_EM = new QLabel("联系邮箱");
    v_layout->addWidget(label_ID,0);
    v_layout->addWidget(label_PW,1);
    v_layout->addWidget(label_PW_,2);
    v_layout->addWidget(label_EM,3);
    signDialog->setLayout(v_layout);
    signDialog->show();
}

