/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#include <bluetoothmanager.h>
#include <chatserver.h>


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_Bluetooth()
{
    qmlRegisterTypesAndRevisions<BluetoothManager>("Bluetooth", 1);
    qmlRegisterTypesAndRevisions<ChatServer>("Bluetooth", 1);
    qmlRegisterModule("Bluetooth", 1, 0);
}

static const QQmlModuleRegistration registration("Bluetooth", qml_register_types_Bluetooth);
