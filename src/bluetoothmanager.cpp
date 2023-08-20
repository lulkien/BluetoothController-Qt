#include <QGuiApplication>
#include <QQmlEngine>
#include "bluetoothmanager.h"
#include "common.h"

#define BLE_SCAN_TIMEOUT    5000   // 5 secs

BluetoothManager &BluetoothManager::instance()
{
    static BluetoothManager self;
    return self;
}

BluetoothManager::~BluetoothManager()
{
    LOG_DBG;
}

BluetoothManager::BluetoothManager()
{
    LOG_DBG << "Create bluetooth manager";

    // get adaptors info
    m_listAdaptors = m_localDevices.allDevices();
    if (m_listAdaptors.count() < 1)
    {
        LOG_ERR << "Bluetooth adaptor not found";
        QGuiApplication::quit();
    }

    // init chat server and connect signals/slots
    m_server = new ChatServer(this);
    m_server->start(m_listAdaptors.at(0).address());

    // get local device name
    m_deviceName = m_listAdaptors.at(0).name();

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}
