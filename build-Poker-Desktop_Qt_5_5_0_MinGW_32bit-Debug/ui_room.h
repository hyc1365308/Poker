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
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_room
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *room)
    {
        if (room->objectName().isEmpty())
            room->setObjectName(QStringLiteral("room"));
        room->resize(800, 600);
        pushButton = new QPushButton(room);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(260, 180, 75, 23));

        retranslateUi(room);

        QMetaObject::connectSlotsByName(room);
    } // setupUi

    void retranslateUi(QDialog *room)
    {
        room->setWindowTitle(QApplication::translate("room", "Dialog", 0));
        pushButton->setText(QApplication::translate("room", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class room: public Ui_room {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROOM_H
