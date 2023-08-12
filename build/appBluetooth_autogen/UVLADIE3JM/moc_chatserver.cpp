/****************************************************************************
** Meta object code from reading C++ file 'chatserver.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/chatserver.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSChatServerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSChatServerENDCLASS = QtMocHelpers::stringData(
    "ChatServer",
    "QML.Element",
    "auto",
    "messageReceived",
    "",
    "sender",
    "message",
    "notifyClientConnected",
    "name",
    "notifyClientDisconnected",
    "sendMessage",
    "clientConnected",
    "clientDisconnected",
    "readSocket"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSChatServerENDCLASS_t {
    uint offsetsAndSizes[28];
    char stringdata0[11];
    char stringdata1[12];
    char stringdata2[5];
    char stringdata3[16];
    char stringdata4[1];
    char stringdata5[7];
    char stringdata6[8];
    char stringdata7[22];
    char stringdata8[5];
    char stringdata9[25];
    char stringdata10[12];
    char stringdata11[16];
    char stringdata12[19];
    char stringdata13[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSChatServerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSChatServerENDCLASS_t qt_meta_stringdata_CLASSChatServerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "ChatServer"
        QT_MOC_LITERAL(11, 11),  // "QML.Element"
        QT_MOC_LITERAL(23, 4),  // "auto"
        QT_MOC_LITERAL(28, 15),  // "messageReceived"
        QT_MOC_LITERAL(44, 0),  // ""
        QT_MOC_LITERAL(45, 6),  // "sender"
        QT_MOC_LITERAL(52, 7),  // "message"
        QT_MOC_LITERAL(60, 21),  // "notifyClientConnected"
        QT_MOC_LITERAL(82, 4),  // "name"
        QT_MOC_LITERAL(87, 24),  // "notifyClientDisconnected"
        QT_MOC_LITERAL(112, 11),  // "sendMessage"
        QT_MOC_LITERAL(124, 15),  // "clientConnected"
        QT_MOC_LITERAL(140, 18),  // "clientDisconnected"
        QT_MOC_LITERAL(159, 10)   // "readSocket"
    },
    "ChatServer",
    "QML.Element",
    "auto",
    "messageReceived",
    "",
    "sender",
    "message",
    "notifyClientConnected",
    "name",
    "notifyClientDisconnected",
    "sendMessage",
    "clientConnected",
    "clientDisconnected",
    "readSocket"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSChatServerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       1,   14, // classinfo
       7,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    2,   58,    4, 0x06,    1 /* Public */,
       7,    1,   63,    4, 0x06,    4 /* Public */,
       9,    1,   66,    4, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      10,    1,   69,    4, 0x0a,    8 /* Public */,
      11,    0,   72,    4, 0x08,   10 /* Private */,
      12,    0,   73,    4, 0x08,   11 /* Private */,
      13,    0,   74,    4, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ChatServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSChatServerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSChatServerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_metaTypeArray<
        // Q_OBJECT / Q_GADGET
        ChatServer,
        // method 'messageReceived'
        void,
        const QString &,
        const QString &,
        // method 'notifyClientConnected'
        void,
        const QString &,
        // method 'notifyClientDisconnected'
        void,
        const QString &,
        // method 'sendMessage'
        void,
        const QString &,
        // method 'clientConnected'
        void,
        // method 'clientDisconnected'
        void,
        // method 'readSocket'
        void
    >,
    nullptr
} };

void ChatServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatServer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->messageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->notifyClientConnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->notifyClientDisconnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->sendMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->clientConnected(); break;
        case 5: _t->clientDisconnected(); break;
        case 6: _t->readSocket(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChatServer::*)(const QString & , const QString & );
            if (_t _q_method = &ChatServer::messageReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ChatServer::*)(const QString & );
            if (_t _q_method = &ChatServer::notifyClientConnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ChatServer::*)(const QString & );
            if (_t _q_method = &ChatServer::notifyClientDisconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *ChatServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSChatServerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ChatServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ChatServer::messageReceived(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ChatServer::notifyClientConnected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ChatServer::notifyClientDisconnected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
