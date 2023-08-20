#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QObject>
#include <QBluetoothAddress>
#include <QBluetoothServiceInfo>
#include <QBluetoothUuid>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>
#include "chatserver.h"

class BluetoothManager : public QObject
{
    Q_OBJECT
public:
    static BluetoothManager &instance();
    virtual ~BluetoothManager();

private:
    BluetoothManager();

public Q_SLOTS:

private Q_SLOTS:

signals:

private:
    QBluetoothLocalDevice       m_localDevices;
    QList<QBluetoothHostInfo>   m_listAdaptors;

    ChatServer  *m_server;
    QString     m_deviceName;

};



#endif // BLUETOOTHMANAGER_H
