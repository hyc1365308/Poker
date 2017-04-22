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
        if(!query.exec("SELECT usertel_em,userpd,userauto FROM rememberdata")){
            qDebug()<< query.lastError().text();
        }
        while(query.next())
        {
            QString usertel_em = query.value(0).toString();
            QString userpd = query.value(1).toString();
            int userauto = query.value(2).toInt();
            record.append(usertel_em);
            record_pd.append(userpd);
            qDebug ()<<usertel_em<<" userauto "<<userauto;
            if (userauto == 1)
                autologin.append(usertel_em);
        }
        query.finish();
        db.close();
    }

//    for (int i = 0; i< record.size(); i++)
//        qDebug()<<record.at(i)<<" + "<<record_pd.at(i);

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
    //const QString left_label[] = {"用户昵称","输入密码","确认密码","登录邮箱","手机号码"};

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

    QLabel *label_PHO = new QLabel("选择头像");
    PHO = new QComboBox;
    label_PHO->setAlignment(Qt::AlignCenter);
    PHO->addItem(QWidget::tr("默认汉纸头像"));
    PHO->addItem(QWidget::tr("默认妹纸头像"));
    PHO->addItem(QWidget::tr("自己选择头像/"));
    connect(PHO, SIGNAL(activated(int)), this, SLOT(changePhoto(int)));


    //PHO->setStyleSheet("border-image: url(:/source/picture/background/blue.jpg);");;
    gridLayout1->addWidget(label_PHO,6,0);
    gridLayout1->addWidget(PHO,6,1);

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

void login::on_loginButton_clicked()
{
    //对于手机/邮箱，或者密码可以考虑怎么样从数据库获取
    QString _ID = this->ui->userID->text().trimmed();
    //输入的_ID可以为手机号或者邮箱
    QString PASSWORD = this->ui->Password->text().trimmed();
    //qDebug()<<_ID<<" "<<PASSWORD;

    QRegExp regExp("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
    QRegExp _regExp("\\d{11}$");

    int kind = 0;//1表示手机号登录，2表示邮箱登录
    if (_regExp.exactMatch(_ID))
        kind = 1;
    else if (regExp.exactMatch(_ID))
        kind = 2;


    QString userid = "@@@";
    QString userpd = "@@@";
    QString userem = "@@@";
    QString usertel = "@@@";

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
        if(!query.exec("SELECT userid,userpd,userem,usertel FROM usersdata")){
            qDebug()<< query.lastError().text();
        }
        while(query.next())
        {
            userid = query.value(0).toString();
            userpd = query.value(1).toString();
            //userName = query.value(3).toString();
            userem = query.value(2).toString();
            usertel = query.value(3).toString();
            if (((userem == _ID)&&(kind == 2)) || ((usertel == _ID )&&(kind == 1)))
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
        QMessageBox::warning(NULL,tr("警告"),tr("请先输入邮箱或手机号码!!!"),QMessageBox::Yes);
    }

    else if (userid == "@@@")
    {
        QMessageBox::warning(NULL,tr("警告"),tr("该邮箱或手机号码不存在，请先注册!!!"),QMessageBox::Yes);
    }

    else if (PASSWORD != userpd)
    {
        QMessageBox::warning(NULL,tr("警告"),tr("密码错误!!!"),QMessageBox::Yes);
    }
    else
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
        bool temp = ui->rememberPassword->checkState();
        bool temp2 = ui->autoLogin->checkState();

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

                    //qDebug()<<removeloc;
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
                if (temp2)
                {
                    //qDebug()<<"_ID"<<_ID;
                    for (int i = 0; i < autologin.size(); i++)
                    {
                        QString _id = static_cast<QString>(autologin.at(i));
                        if (_id == _ID)
                        {
                            temp2 = false;
                            break;
                        }
                    }
                    if (temp2){
//                            query.prepare("UPDATE rememberdata SET userauto = 1 WHERE usertel_em = _ID");

//                            if(!query.exec())
//                            {
//                                qDebug()<<query.lastError();
//                            }
//                            else
//                            {
//                                qDebug()<<"_ID"<<_ID;
//                            }
//                            query.finish();
                        }
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
            //qDebug()<<removeloc;
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
                    //autologin.removeAt(removeloc);
                }
                query.finish();
        }
        db.close();
        userId = userid;
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
//            QString imagePath = "D:/poker/source/picture/icon/beauty.jpg";

//            QFile* file=new QFile(imagePath); //file为二进制数据文件名

//            QByteArray data;
//            file->open(QIODevice::ReadOnly);

//            data = file->readAll();

//            file->close();
//            QVariant var(data);

            //qDebug()<<sizeof(data)<<" "<<sizeof(var);

            //以下注释部分为数据库的初始化和测试部分
            //数据库表单名为usersdata
            //包括userid,username,userpd,userem,usergold,userwin,userlose
            //分别为传输id，用户昵称,用户密码,用户邮箱,用户手机号,用户头像,用户金币,用户胜场，用户负场
//            if (!query.exec("CREATE TABLE usersdata("
//                           "userid VARCHAR,"
//                           "username VARCHAR,"
//                           "userpd VARCHAR,"
//                           "userem VARCHAR,"
//                           "usertel VARCHAR,"
//                           "userphoto LONGBLOB,"
//                           "usergold INT,"
//                           "userwin INT,"
//                           "userlose INT)")){
//                qDebug()<< query.lastError().text()<<"\n";
//            }
//            query.prepare("INSERT INTO usersdata(userid,username,userpd,userem,usertel,userphoto,usergold,userwin,userlose) VALUES(?,?,?,?,?,?,?,?,?)");
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

//            QVariantList userphotos;
//            userphotos<< var << var << var;
//            query.addBindValue(userphotos);

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

            //进行插入数据库操作
            QString _nm = NM->text().trimmed();
            QString _pw = PW->text().trimmed();
            QString _em = EM->text().trimmed();
            QString _tel = TEL->text().trimmed();
            if(query.exec("SELECT userid,userem,usertel FROM usersdata")){
                qDebug()<< query.lastError().text()<<"\n";
            }
            QString _userid;
            QString _userem;
            QString _usertel;
            while(query.next())
            {
                _userid = query.value(0).toString();
                _userem = query.value(1).toString();
                _usertel = query.value(2).toString();
                //qDebug()<< _userid<<" "<<_formalid;
                if (_userem == _em)
                {
                    //qDebug()<< "该邮箱已被注册！！"<<_userid;
                    QMessageBox::information(NULL,tr("提醒"),tr("该邮箱已被注册！！！"),QMessageBox::Yes);
                    return;
                }
                if(_usertel == _tel)
                {
                    QMessageBox::information(NULL,tr("提醒"),tr("该手机号码已被注册！！！"),QMessageBox::Yes);
                    return;
                }
            }

            query.prepare("INSERT INTO usersdata(userid,username,userpd,userem,usertel,userphoto,usergold,userwin,userlose) VALUES(?,?,?,?,?,?,?,?,?)");
            QString last = _userid.mid(4);
            int num = last.toInt()+1;
            QString _userid_ = "user"+QString::number(num,10);
            QString _imagePath = PHO->currentText().mid(7);
            QByteArray _data;
            QFile* _file=new QFile(_imagePath); //file为二进制数据文件名
            _file->open(QIODevice::ReadOnly);
            _data = _file->readAll();
            _file->close();
            QVariant _var(_data);
            //qDebug()<<_formalid_;
            query.addBindValue(_userid_);
            query.addBindValue(_nm);
            query.addBindValue(_pw);
            query.addBindValue(_em);
            query.addBindValue(_tel);
            query.addBindValue(_var);
            query.addBindValue(50000);
            query.addBindValue(0);
            query.addBindValue(0);
            if (!query.exec())
                QMessageBox::information(NULL,tr("提醒"),tr("对不起,您注册失败！！！"),QMessageBox::Yes);
            else
                QMessageBox::information(NULL,tr("提醒"),tr("恭喜,您已注册成功！！！"),QMessageBox::Yes);
            query.finish();
            db.close();
        }
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
    if (flag)
    {
        for (int i = 0; i < autologin.size(); i++)
        {
            QString _id = static_cast<QString>(autologin.at(i));
            if (_id == ui->userID->text().trimmed())
                accept();
        }
    }
}

QString login::getuserid()
{
    return userId;
}


void login::changePhoto(int index)
{
    qDebug()<<"index:"<<index;
    if (index == 2)
    {
        QString g_strCurrentDir;
        QString strImage = QFileDialog::getOpenFileName(this,"Please Select image file",g_strCurrentDir,"Image Format (*.png *.jpg *.bmp *.gif)");
        if (strImage.isNull())
            return;
        g_strCurrentDir = QDir(strImage).absolutePath();
        QString preText = PHO->currentText();
        QString nowText = preText.left(7) + g_strCurrentDir;
        PHO->setItemText(2,nowText);
    }
}

