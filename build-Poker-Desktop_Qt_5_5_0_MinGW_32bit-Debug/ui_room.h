/********************************************************************************
** Form generated from reading UI file 'room.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROOM_H
#define UI_ROOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_room
{
public:
    QWidget *card_1;
    QWidget *card_2;
    QWidget *card_3;
    QWidget *card_4;
    QWidget *card_5;
    QLabel *timelabel;

    void setupUi(QDialog *room)
    {
        if (room->objectName().isEmpty())
            room->setObjectName(QStringLiteral("room"));
        room->resize(1200, 700);
        QIcon icon;
        icon.addFile(QStringLiteral(":/source/picture/icon/room_icon.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        room->setWindowIcon(icon);
        room->setStyleSheet(QStringLiteral("border-image: url(:/source/picture/background/room.jpg);"));
        card_1 = new QWidget(room);
        card_1->setObjectName(QStringLiteral("card_1"));
        card_1->setGeometry(QRect(350, 310, 80, 120));
        card_1->setStyleSheet(QStringLiteral("border-image: url();"));
        card_2 = new QWidget(room);
        card_2->setObjectName(QStringLiteral("card_2"));
        card_2->setGeometry(QRect(460, 310, 80, 120));
        card_2->setStyleSheet(QStringLiteral("border-image: url();"));
        card_3 = new QWidget(room);
        card_3->setObjectName(QStringLiteral("card_3"));
        card_3->setGeometry(QRect(570, 310, 80, 120));
        card_3->setStyleSheet(QStringLiteral("border-image: url();"));
        card_4 = new QWidget(room);
        card_4->setObjectName(QStringLiteral("card_4"));
        card_4->setGeometry(QRect(680, 310, 80, 120));
        card_4->setStyleSheet(QStringLiteral("border-image: url();"));
        card_5 = new QWidget(room);
        card_5->setObjectName(QStringLiteral("card_5"));
        card_5->setGeometry(QRect(790, 310, 80, 120));
        card_5->setStyleSheet(QStringLiteral("border-image: url();"));
        timelabel = new QLabel(room);
        timelabel->setObjectName(QStringLiteral("timelabel"));
        timelabel->setGeometry(QRect(190, 40, 130, 40));
        QFont font;
        font.setPointSize(20);
        timelabel->setFont(font);
        timelabel->setStyleSheet(QLatin1String("border-image: url();\n"
"color: rgb(255, 255, 255);"));

        retranslateUi(room);

        QMetaObject::connectSlotsByName(room);
    } // setupUi

    void retranslateUi(QDialog *room)
    {
        room->setWindowTitle(QApplication::translate("room", "Dialog", 0));
        timelabel->setText(QApplication::translate("room", "22:22:22", 0));
    } // retranslateUi

};

namespace Ui {
    class room: public Ui_room {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROOM_H
