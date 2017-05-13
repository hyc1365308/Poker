#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    //自动登录部分还未实现，感觉也没什么意义，就这样吧
    //就让它空占个选择框以保持对称性吧
    ui->autoLogin->setEnabled(false);

    //connectHost();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("remember.db");
    if (!db.open())
    {
        qDebug("数据库打开失败！！！");
    }
    else
    {
        qDebug("数据库打开成功！！！");
        QSqlQuery query;
        if(!query.exec("SELECT usertel_em,userpd FROM rememberdata")){
            qDebug()<< query.lastError().text();
        }
        while(query.next())
        {
            QString usertel_em = query.value(0).toString();
            QString userpd = query.value(1).toString();
            record.append(usertel_em);
            record_pd.append(userpd);
        }
        query.finish();
        db.close();
    }
    //qDebug()<<record.size();
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    //规定密码只能为英文大小写字母和数字
    QRegExp regExp("[A-Za-z0-9]+");
    ui->Password->setValidator(new QRegExpValidator(regExp,this));

    ui->userID->setPlaceholderText("邮箱/手机");

    //用户昵称只能是数字，英文大小写字母，英文下划线，汉字
    QRegExp _regExp2("^[a-zA-Z0-9_\u4e00-\u9fa5\\w]+$");
    //ui->userID->setValidator(new QRegExpValidator(regExp2,this));

    //用户手机号为11位数字
    QRegExp regExp3("\\d{11}$");
    QRegExp regExp2("\\d{11}|([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
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

    signDialog->resize(300,250);

    QGridLayout *gridLayout1 = new QGridLayout;
    gridLayout1->setSpacing(10);
    gridLayout1->setMargin(5);

    QLabel *title = new QLabel("请输入注册信息");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont( "Timers",12,QFont::Bold));
    title->setStyleSheet("color:blue");//文本颜色
    gridLayout1->addWidget(title,0,0,1,2);

    QLabel *label_ID = new QLabel("用户昵称");
    NM = new QLineEdit;
    label_ID->setAlignment(Qt::AlignCenter);
    NM->setPlaceholderText("数字/英文大小写字母/英文下划线/汉字");
    NM->setValidator(new QRegExpValidator(_regExp2,this));
    NM->setFocus();
    gridLayout1->addWidget(label_ID,1,0);
    gridLayout1->addWidget(NM,1,1);

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

    QLabel *label_EM = new QLabel("登录邮箱");
    EM = new QLineEdit;
    label_EM->setAlignment(Qt::AlignCenter);
    EM->setPlaceholderText("正确邮箱格式");
    gridLayout1->addWidget(label_EM,4,0);
    gridLayout1->addWidget(EM,4,1);

    QLabel *label_TEL = new QLabel("手机号码");
    TEL = new QLineEdit;
    label_TEL->setAlignment(Qt::AlignCenter);
    TEL->setPlaceholderText("手机号码(11位)");
    TEL->setValidator(new QRegExpValidator(regExp3,this));
    gridLayout1->addWidget(label_TEL,5,0);
    gridLayout1->addWidget(TEL,5,1);

    QLabel *label_SEX = new QLabel("选择性别");
    SEX = new QComboBox;
    label_SEX->setAlignment(Qt::AlignCenter);
    SEX->addItem(QWidget::tr("我是汉纸"));
    SEX->addItem(QWidget::tr("我是妹纸"));
    SEX->addItem(QWidget::tr("我不告诉你"));


    //PHO->setStyleSheet("border-image: url(:/source/picture/background/blue.jpg);");;
    gridLayout1->addWidget(label_SEX,6,0);
    gridLayout1->addWidget(SEX,6,1);

    confirm = new QPushButton("确认注册");
    connect(confirm,SIGNAL(clicked()),this,SLOT(_sign_in()));
    cancel = new QPushButton("重新输入");
    connect(cancel,SIGNAL(clicked()),this,SLOT(_cancel()));

    QHBoxLayout *_button = new QHBoxLayout;
    _button->addWidget(confirm);
    _button->addWidget(cancel);

    gridLayout1->addLayout(_button,7,0,1,2);

    signDialog->setLayout(gridLayout1);

    ui->userID->setFocus();
    ui->info->installEventFilter(this);
    ui->sign_in->installEventFilter(this);
}

login::~login()
{
    delete ui;
}

void login::connectHost()
{
    //this->readWriteSocket->abort();
    this->readWriteSocket = new QTcpSocket;
    this->readWriteSocket->connectToHost(QHostAddress("127.0.0.1"),8888);
    //qDebug()<<"connect to host……";
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

void login::recvMessage()
{
    QString info;
    info += this->readWriteSocket->readAll();

    qDebug() << "recvMessage:" <<info;
    QStringList _infolist = info.split('?');
    if (_infolist.at(0) == "retologin")//处理登录的查找回应
    {
        if (_infolist.at(1) == "inexist")
        {
            this->ui->userID->clear();
            this->ui->Password->clear();
            this->ui->userID->setFocus();
            QMessageBox::warning(NULL,tr("警告"),tr("该邮箱或手机号码不存在，请先注册!!!"),QMessageBox::Yes);
        }
        else if (_infolist.at(2) == "wrong")
        {
            this->ui->Password->clear();
            this->ui->Password->setFocus();
            QMessageBox::warning(NULL,tr("警告"),tr("密码错误!!!"),QMessageBox::Yes);
        }
        else if (_infolist.at(2) == "right")
        {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("remember.db");
            if (!db.open())
            {
                 qDebug("数据库打开失败！！！");
            }
            qDebug("数据库打开成功！！！");
            QSqlQuery query;
            if (!query.exec("CREATE TABLE IF NOT EXISTS rememberdata("
                                               "usertel_em VARCHAR,"
                                               "userpd VARCHAR,"
                                               "userauto INT)")){
                qDebug()<< query.lastError().text()<<"\n";
            }
            int removeloc = -1;
            QString _ID = ui->userID->text();
            QString userpd = ui->Password->text();
            bool temp = ui->rememberPassword->checkState();
            if (temp){
                for (int i = 0; i < record.size(); i++)
                {
                    QString _id = static_cast<QString>(record.at(i));
                    if (_id == _ID)
                    {
                        temp = false;
                        removeloc = i;
                        break;
                    }
                }
                if (temp)
                {
                    query.prepare("INSERT INTO rememberdata(usertel_em,userpd,userauto) VALUES(?,?,?)");
                    query.addBindValue(_ID);
                    query.addBindValue(userpd);
                    query.addBindValue(0);
                    record.append(_ID);
                    record_pd.append(userpd);
                    if (!query.exec())
                        qDebug()<<"记住密码失败！！！";
                    query.finish();
                }
            }
            else
            {
                for (int i = 0; i < record.size(); i++)
                {
                    QString _id = static_cast<QString>(record.at(i));
                    if (_id == _ID)
                    {
                        removeloc = i;
                        break;
                    }
                }
                query.prepare("DELETE FROM rememberdata WHERE usertel_em = ?");

                query.addBindValue(_ID);
                if(!query.exec())
                {
                    qDebug()<<query.lastError();
                }
                if (removeloc >= 0){
                    //qDebug()<<record.at(removeloc);
                    record.removeAt(removeloc);
                    record_pd.removeAt(removeloc);
                }
                query.finish();
            }
            db.close();
            userId = _infolist.at(3);
            accept();
        }
    }
    else if (_infolist.at(0) == "retosign")//处理注册的响应
    {
        //qDebug()<<_infolist.at(1);
        if (_infolist.at(1) == "okay")
            QMessageBox::information(NULL,tr("提示"),tr("恭喜您，注册成功!!!"),QMessageBox::Yes);
        else if(_infolist.at(1) == "fail")
            QMessageBox::information(NULL,tr("提示"),tr("由于未知原因注册失败，请稍后再试!!!"),QMessageBox::Yes);
        else if(_infolist.size() == 3)
        {
            QString hint = _infolist.at(1) +"和" + _infolist.at(2) + "已经被注册!!!";
            const char * p = hint.toStdString().data();
            QMessageBox::information(NULL,tr("提示"),tr(p),QMessageBox::Yes);
        }
        else
        {
            QString hint = _infolist.at(1) + "已经被注册!!!";
            const char * p = hint.toStdString().data();
            QMessageBox::information(NULL,tr("提示"),tr(p),QMessageBox::Yes);
        }
    }

    this->readWriteSocket->abort();
}

void login::on_loginButton_clicked()
{
    //对于手机/邮箱，或者密码可以考虑怎么样从数据库获取
    QString _ID = this->ui->userID->text().trimmed();
    //输入的_ID可以为手机号或者邮箱
    QString PASSWORD = this->ui->Password->text().trimmed();
    QRegExp regExp("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
    QRegExp _regExp("\\d{11}$");
    if (_ID == "")
    {
        QMessageBox::warning(NULL,tr("警告"),tr("请先输入邮箱或手机号码!!!"),QMessageBox::Yes);
    }
    else if(!regExp.exactMatch(_ID) && !_regExp.exactMatch(_ID))
    {
        QMessageBox::warning(NULL,tr("警告"),tr("邮箱格式或者手机号码格式不正确！"),QMessageBox::Yes);
    }
    else if(PASSWORD == "")
    {
        QMessageBox::warning(NULL,tr("警告"),tr("请输入密码!!!"),QMessageBox::Yes);
    }
    else
    {
        connectHost();
        QString s = "login?" + ui->userID->text() +"?" + ui->Password->text();
        sendMessage(s);
    }
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
    NM->clear();
    PW->clear();
    PW_->clear();
    EM->clear();
    TEL->clear();
    signDialog->show();
}

void login::_sign_in()
{
    QRegExp regExp("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
    QRegExp _regExp("\\d{11}$");
    if(NM->text()=="")
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
    else if(!_regExp.exactMatch(TEL->text()))
    {
        QMessageBox::warning(NULL,tr("警告"),tr("手机号码不正确！"),QMessageBox::Yes);
    }
    else
    {
        connectHost();
        QString s = "sign?" + NM->text() +"?" + PW->text() + "?" + EM->text() + "?" +TEL->text() + "?" + SEX->currentText();
        sendMessage(s);

//            以下注释部分为数据库的初始化和测试部分
//            数据库表单名为usersdata
//            包括userid,username,userpd,userem,usergold,userwin,userlose
//            分别为传输id，用户昵称,用户密码,用户邮箱,用户手机号,用户性别,用户金币,用户胜场，用户负场
//        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//        db.setDatabaseName("database.db");
//        if (!db.open())
//        {
//             qDebug("数据库打开失败！！！");
//        }
//        qDebug("数据库打开成功！！！");
//        QSqlQuery query;
//        if (!query.exec("CREATE TABLE usersdata("
//                           "userid VARCHAR,"
//                           "username VARCHAR,"
//                           "userpd VARCHAR,"
//                           "userem VARCHAR,"
//                           "usertel VARCHAR,"
//                           "usersex VARCHAR,"
//                           "usergold INT,"
//                           "userwin INT,"
//                           "userlose INT)")){
//                qDebug()<< query.lastError().text()<<"\n";
//            }
//            query.prepare("INSERT INTO usersdata(userid,username,userpd,userem,usertel,usersex,usergold,userwin,userlose) VALUES(?,?,?,?,?,?,?,?,?)");
//            QVariantList userids;
//            userids<< "user1" << "user2" << "user3";
//            query.addBindValue(userids);

//            QVariantList usernames;
//            usernames<< "zys" << "fyl" << "hyq";
//            query.addBindValue(usernames);

//            QVariantList userpds;
//            userpds<< "2014011296" << "2014011365" << "2014011324";
//            query.addBindValue(userpds);

//            QVariantList userems;
//            userems<< "1450214244@qq.com" << "thucoldwind@gmail.com" << "hyc1365308@sina.com";
//            query.addBindValue(userems);

//            QVariantList usertels;
//            usertels<< "17888834764" << "13141317514" << "13121415804";
//            query.addBindValue(usertels);

//            QVariantList usersexs;
//            usersexs<< "boy" << "boy" << "boy";
//            query.addBindValue(usersexs);

//            QVariantList usergolds;
//            usergolds<< 10000000 << 10000000 << 10000000;
//            query.addBindValue(usergolds);

//            QVariantList userwins;
//            userwins<< 0 << 0 << 0;
//            query.addBindValue(userwins);

//            QVariantList userloses;
//            userloses<< 0 << 0 << 0;
//            query.addBindValue(userloses);
//            if (!query.execBatch())
//            {
//                qDebug()<< query.lastError().text()<<"\n";
//            }
    }
}

void login::_cancel()
{
    NM->clear();
    PW->clear();
    PW_->clear();
    EM->clear();
    TEL->clear();
}

void login::on_userID_textChanged(const QString &arg1)
{
    bool flag = false;
    for (int i = 0; i < record.size(); i++)
    {
        QString _id = static_cast<QString>(record.at(i));
        if (_id == ui->userID->text().trimmed())
        {
            QString _pd = static_cast<QString>(record_pd.at(i));
            ui->rememberPassword->setChecked(true);
            ui->Password->setText(_pd);
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        ui->Password->clear();
        ui->rememberPassword->setChecked(false);
    }
}

QString login::getuserid()
{
    return userId;
}

void login::sendMessage(QString _info)
{
    qDebug()<<"sendMessage:"<<_info;
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(_info);
    this->readWriteSocket->write(array->data());
}


//void login::changePhoto(int index)
//{
//    qDebug()<<"index:"<<index;
//    if (index == 2)
//    {
//        QString g_strCurrentDir;
//        QString strImage = QFileDialog::getOpenFileName(this,"Please Select image file",g_strCurrentDir,"Image Format (*.png *.jpg *.bmp *.gif)");
//        if (strImage.isNull())
//            return;
//        g_strCurrentDir = QDir(strImage).absolutePath();
//        QString preText = PHO->currentText();
//        QString nowText = preText.left(7) + g_strCurrentDir;
//        PHO->setItemText(2,nowText);
//    }
//}
