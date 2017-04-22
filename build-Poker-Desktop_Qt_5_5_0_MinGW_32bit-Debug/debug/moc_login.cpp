/****************************************************************************
** Meta object code from reading C++ file 'login.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Poker/login.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_login_t {
    QByteArrayData data[15];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_login_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_login_t qt_meta_stringdata_login = {
    {
QT_MOC_LITERAL(0, 0, 5), // "login"
QT_MOC_LITERAL(1, 6, 22), // "on_loginButton_clicked"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 11), // "eventFilter"
QT_MOC_LITERAL(4, 42, 3), // "obj"
QT_MOC_LITERAL(5, 46, 7), // "QEvent*"
QT_MOC_LITERAL(6, 54, 2), // "ev"
QT_MOC_LITERAL(7, 57, 9), // "show_info"
QT_MOC_LITERAL(8, 67, 7), // "sign_in"
QT_MOC_LITERAL(9, 75, 21), // "on_userID_textChanged"
QT_MOC_LITERAL(10, 97, 4), // "arg1"
QT_MOC_LITERAL(11, 102, 11), // "changePhoto"
QT_MOC_LITERAL(12, 114, 5), // "index"
QT_MOC_LITERAL(13, 120, 8), // "_sign_in"
QT_MOC_LITERAL(14, 129, 7) // "_cancel"

    },
    "login\0on_loginButton_clicked\0\0eventFilter\0"
    "obj\0QEvent*\0ev\0show_info\0sign_in\0"
    "on_userID_textChanged\0arg1\0changePhoto\0"
    "index\0_sign_in\0_cancel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_login[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    2,   55,    2, 0x08 /* Private */,
       7,    0,   60,    2, 0x08 /* Private */,
       8,    0,   61,    2, 0x08 /* Private */,
       9,    1,   62,    2, 0x08 /* Private */,
      11,    1,   65,    2, 0x08 /* Private */,
      13,    0,   68,    2, 0x0a /* Public */,
      14,    0,   69,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QObjectStar, 0x80000000 | 5,    4,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void login::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        login *_t = static_cast<login *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_loginButton_clicked(); break;
        case 1: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: _t->show_info(); break;
        case 3: _t->sign_in(); break;
        case 4: _t->on_userID_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->changePhoto((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->_sign_in(); break;
        case 7: _t->_cancel(); break;
        default: ;
        }
    }
}

const QMetaObject login::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_login.data,
      qt_meta_data_login,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *login::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *login::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_login.stringdata0))
        return static_cast<void*>(const_cast< login*>(this));
    return QDialog::qt_metacast(_clname);
}

int login::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
