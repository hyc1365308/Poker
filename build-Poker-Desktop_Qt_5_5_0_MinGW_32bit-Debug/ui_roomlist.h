/********************************************************************************
** Form generated from reading UI file 'roomlist.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROOMLIST_H
#define UI_ROOMLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_roomlist
{
public:
    QPushButton *roomlist1;
    QPushButton *roomlist2;
    QPushButton *roomlist3;
    QPushButton *roomlist4;
    QPushButton *roomlist5;
    QPushButton *rank;
    QPushButton *setting;
    QPushButton *tips;
    QLabel *label_rank;
    QLabel *label_setting;
    QLabel *label_tips;
    QPushButton *person;
    QLabel *userid;
    QLabel *usergold;
    QLabel *label;
    QFrame *line;
    QWidget *room_list;
    QPushButton *pageup;
    QPushButton *pagedown;
    QLabel *_currentpage;

    void setupUi(QDialog *roomlist)
    {
        if (roomlist->objectName().isEmpty())
            roomlist->setObjectName(QStringLiteral("roomlist"));
        roomlist->resize(1200, 700);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(roomlist->sizePolicy().hasHeightForWidth());
        roomlist->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral("../source/picture/icon/beauty.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        roomlist->setWindowIcon(icon);
        roomlist->setStyleSheet(QStringLiteral("#roomlist{border-image: url(:/source/picture/background/roomList.jpg);}"));
        roomlist1 = new QPushButton(roomlist);
        roomlist1->setObjectName(QStringLiteral("roomlist1"));
        roomlist1->setGeometry(QRect(80, 280, 240, 65));
        roomlist1->setCursor(QCursor(Qt::OpenHandCursor));
        roomlist1->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/button1.jpg);"));
        roomlist2 = new QPushButton(roomlist);
        roomlist2->setObjectName(QStringLiteral("roomlist2"));
        roomlist2->setGeometry(QRect(80, 360, 240, 65));
        roomlist2->setCursor(QCursor(Qt::OpenHandCursor));
        roomlist2->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/button2.jpg);"));
        roomlist3 = new QPushButton(roomlist);
        roomlist3->setObjectName(QStringLiteral("roomlist3"));
        roomlist3->setGeometry(QRect(80, 440, 240, 65));
        roomlist3->setCursor(QCursor(Qt::OpenHandCursor));
        roomlist3->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/button3.jpg);"));
        roomlist4 = new QPushButton(roomlist);
        roomlist4->setObjectName(QStringLiteral("roomlist4"));
        roomlist4->setGeometry(QRect(80, 520, 240, 65));
        roomlist4->setCursor(QCursor(Qt::OpenHandCursor));
        roomlist4->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/button4.jpg);"));
        roomlist5 = new QPushButton(roomlist);
        roomlist5->setObjectName(QStringLiteral("roomlist5"));
        roomlist5->setGeometry(QRect(80, 600, 240, 65));
        roomlist5->setCursor(QCursor(Qt::OpenHandCursor));
        roomlist5->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/button5.jpg);"));
        rank = new QPushButton(roomlist);
        rank->setObjectName(QStringLiteral("rank"));
        rank->setGeometry(QRect(770, 60, 50, 50));
        rank->setCursor(QCursor(Qt::OpenHandCursor));
        rank->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/icon/rank.jpg);"));
        setting = new QPushButton(roomlist);
        setting->setObjectName(QStringLiteral("setting"));
        setting->setGeometry(QRect(870, 60, 50, 50));
        setting->setCursor(QCursor(Qt::OpenHandCursor));
        setting->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/icon/setting.jpg);"));
        tips = new QPushButton(roomlist);
        tips->setObjectName(QStringLiteral("tips"));
        tips->setGeometry(QRect(970, 60, 50, 50));
        tips->setCursor(QCursor(Qt::OpenHandCursor));
        tips->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/icon/tips.jpg);"));
        label_rank = new QLabel(roomlist);
        label_rank->setObjectName(QStringLiteral("label_rank"));
        label_rank->setGeometry(QRect(770, 110, 50, 20));
        label_rank->setContextMenuPolicy(Qt::DefaultContextMenu);
        label_rank->setLayoutDirection(Qt::LeftToRight);
        label_rank->setStyleSheet(QString::fromUtf8("border-image: url();\n"
"color: rgb(255, 255, 255);\n"
"font: 75 12pt \"\345\256\213\344\275\223\";"));
        label_setting = new QLabel(roomlist);
        label_setting->setObjectName(QStringLiteral("label_setting"));
        label_setting->setGeometry(QRect(870, 110, 50, 20));
        label_setting->setStyleSheet(QString::fromUtf8("border-image: url();\n"
"color: rgb(255, 255, 255);\n"
"font: 75 12pt \"\345\256\213\344\275\223\";"));
        label_tips = new QLabel(roomlist);
        label_tips->setObjectName(QStringLiteral("label_tips"));
        label_tips->setGeometry(QRect(970, 110, 50, 20));
        label_tips->setStyleSheet(QString::fromUtf8("border-image: url();\n"
"color: rgb(255, 255, 255);\n"
"font: 75 12pt \"\345\256\213\344\275\223\";"));
        person = new QPushButton(roomlist);
        person->setObjectName(QStringLiteral("person"));
        person->setGeometry(QRect(420, 55, 70, 70));
        person->setCursor(QCursor(Qt::OpenHandCursor));
        person->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/icon/person.png);"));
        userid = new QLabel(roomlist);
        userid->setObjectName(QStringLiteral("userid"));
        userid->setGeometry(QRect(530, 60, 150, 30));
        userid->setStyleSheet(QString::fromUtf8("border-image: url();\n"
"font: 75 12pt \"\345\256\213\344\275\223\";\n"
"color: rgb(255, 255, 255);"));
        usergold = new QLabel(roomlist);
        usergold->setObjectName(QStringLiteral("usergold"));
        usergold->setGeometry(QRect(570, 100, 110, 30));
        usergold->setStyleSheet(QString::fromUtf8("border-image: url();\n"
"font: 75 10pt \"\345\256\213\344\275\223\";\n"
"color: rgb(255, 255, 255);"));
        label = new QLabel(roomlist);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(530, 100, 30, 30));
        label->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/icon/chouma.png);"));
        line = new QFrame(roomlist);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(535, 92, 120, 3));
        line->setStyleSheet(QLatin1String("border-image: url();\n"
"background-color: rgb(85, 170, 0);"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        room_list = new QWidget(roomlist);
        room_list->setObjectName(QStringLiteral("room_list"));
        room_list->setGeometry(QRect(500, 240, 450, 360));
        pageup = new QPushButton(roomlist);
        pageup->setObjectName(QStringLiteral("pageup"));
        pageup->setGeometry(QRect(1030, 260, 75, 75));
        pageup->setCursor(QCursor(Qt::OpenHandCursor));
        pageup->setStyleSheet(QLatin1String("border-image: url(:/source/picture/icon/pageup.png);\n"
"background-color: rgb(147, 98, 0);"));
        pagedown = new QPushButton(roomlist);
        pagedown->setObjectName(QStringLiteral("pagedown"));
        pagedown->setGeometry(QRect(1030, 500, 75, 75));
        pagedown->setCursor(QCursor(Qt::OpenHandCursor));
        pagedown->setStyleSheet(QLatin1String("border-image: url(:/source/picture/icon/pagedown.png);\n"
"background-color: rgb(147, 98, 0);"));
        _currentpage = new QLabel(roomlist);
        _currentpage->setObjectName(QStringLiteral("_currentpage"));
        _currentpage->setGeometry(QRect(1030, 385, 75, 45));

        retranslateUi(roomlist);

        QMetaObject::connectSlotsByName(roomlist);
    } // setupUi

    void retranslateUi(QDialog *roomlist)
    {
        roomlist->setWindowTitle(QApplication::translate("roomlist", "\346\254\242\350\277\216\346\235\245\345\210\260\345\276\267\345\267\236\346\211\221\345\205\213", 0));
        roomlist1->setText(QString());
        roomlist2->setText(QString());
        roomlist3->setText(QString());
        roomlist4->setText(QString());
        roomlist5->setText(QString());
#ifndef QT_NO_TOOLTIP
        rank->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        rank->setText(QString());
        setting->setText(QString());
        tips->setText(QString());
        label_rank->setText(QApplication::translate("roomlist", "\346\216\222\350\241\214", 0));
        label_setting->setText(QApplication::translate("roomlist", "\350\256\276\347\275\256", 0));
        label_tips->setText(QApplication::translate("roomlist", "\346\217\220\347\244\272", 0));
        person->setText(QString());
        userid->setText(QApplication::translate("roomlist", "\346\210\221\347\232\204\345\220\215\345\255\227", 0));
        usergold->setText(QApplication::translate("roomlist", "1000000", 0));
        label->setText(QString());
        pageup->setText(QString());
        pagedown->setText(QString());
        _currentpage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class roomlist: public Ui_roomlist {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROOMLIST_H
