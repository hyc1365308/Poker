/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QPushButton *loginButton;
    QLineEdit *Password;
    QLineEdit *userID;
    QCheckBox *autoLogin;
    QCheckBox *rememberPassword;
    QLabel *sign_in;
    QLabel *info;
    QFrame *line;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QStringLiteral("login"));
        login->resize(400, 300);
        QIcon icon;
        icon.addFile(QStringLiteral(":/source/picture/icon/icon_application.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        login->setWindowIcon(icon);
        login->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/login.jpg);"));
        login->setModal(true);
        loginButton = new QPushButton(login);
        loginButton->setObjectName(QStringLiteral("loginButton"));
        loginButton->setGeometry(QRect(240, 218, 140, 48));
        loginButton->setCursor(QCursor(Qt::OpenHandCursor));
        loginButton->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/loginButton.jpg);"));
        Password = new QLineEdit(login);
        Password->setObjectName(QStringLiteral("Password"));
        Password->setGeometry(QRect(217, 88, 165, 40));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Password->sizePolicy().hasHeightForWidth());
        Password->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Adobe Caslon Pro"));
        font.setPointSize(11);
        Password->setFont(font);
        Password->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/inputLine.jpg);"));
        Password->setEchoMode(QLineEdit::Password);
        Password->setCursorPosition(0);
        userID = new QLineEdit(login);
        userID->setObjectName(QStringLiteral("userID"));
        userID->setGeometry(QRect(217, 40, 165, 40));
        QFont font1;
        font1.setFamily(QStringLiteral("Adobe Caslon Pro"));
        font1.setPointSize(20);
        font1.setBold(true);
        font1.setWeight(75);
        userID->setFont(font1);
        userID->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/inputLine.jpg);"));
        autoLogin = new QCheckBox(login);
        autoLogin->setObjectName(QStringLiteral("autoLogin"));
        autoLogin->setGeometry(QRect(293, 130, 70, 21));
        QFont font2;
        font2.setFamily(QStringLiteral("Adobe Caslon Pro"));
        font2.setBold(true);
        font2.setWeight(75);
        autoLogin->setFont(font2);
        autoLogin->setStyleSheet(QLatin1String("\n"
"border-image: url(:/source/picture/background/pureback2.jpg);"));
        rememberPassword = new QCheckBox(login);
        rememberPassword->setObjectName(QStringLiteral("rememberPassword"));
        rememberPassword->setGeometry(QRect(215, 130, 70, 21));
        QFont font3;
        font3.setFamily(QStringLiteral("Adobe Caslon Pro"));
        font3.setPointSize(9);
        font3.setBold(true);
        font3.setWeight(75);
        rememberPassword->setFont(font3);
        rememberPassword->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/pureback.jpg);"));
        sign_in = new QLabel(login);
        sign_in->setObjectName(QStringLiteral("sign_in"));
        sign_in->setGeometry(QRect(245, 280, 55, 18));
        QFont font4;
        font4.setFamily(QStringLiteral("Adobe Caslon Pro"));
        font4.setPointSize(10);
        font4.setBold(true);
        font4.setWeight(75);
        sign_in->setFont(font4);
        sign_in->setCursor(QCursor(Qt::OpenHandCursor));
        sign_in->setStyleSheet(QStringLiteral("background-color: transparent;border-image:url();"));
        info = new QLabel(login);
        info->setObjectName(QStringLiteral("info"));
        info->setGeometry(QRect(315, 280, 55, 18));
        info->setFont(font4);
        info->setCursor(QCursor(Qt::OpenHandCursor));
        info->setStyleSheet(QStringLiteral("background-color: transparent;border-image:url();"));
        line = new QFrame(login);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(308, 280, 2, 15));
        line->setStyleSheet(QLatin1String("border-image: url();\n"
"background-color: rgb(0, 0, 0);"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QApplication::translate("login", "\345\276\267\345\267\236\346\211\221\345\205\213\347\231\273\345\275\225", 0));
        loginButton->setText(QString());
        autoLogin->setText(QApplication::translate("login", "\350\207\252\345\212\250\347\231\273\345\275\225", 0));
        rememberPassword->setText(QApplication::translate("login", "\350\256\260\344\275\217\345\257\206\347\240\201", 0));
        sign_in->setText(QApplication::translate("login", "\347\224\250\346\210\267\346\263\250\345\206\214", 0));
        info->setText(QApplication::translate("login", "\345\205\263\344\272\216\346\210\221\344\273\254", 0));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
