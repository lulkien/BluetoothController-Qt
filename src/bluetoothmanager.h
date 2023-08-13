#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration>
#include <QBluetoothHostInfo>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothServer>
#include <QBluetoothServiceInfo>
#include <memory>
#include "bluetoothdevicesmodel.h"

class BluetoothManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(bool isScanning READ isScanning WRITE setIsScanning NOTIFY isScanningChanged FINAL)
    Q_PROPERTY(bool isPairing READ isPairing WRITE setIsPairing NOTIFY isPairingChanged FINAL)
public:
    static BluetoothManager &instance();
    static BluetoothManager *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    virtual ~BluetoothManager();

    bool isScanning() const;
    void setIsScanning(bool newIsScanning);

    bool isPairing() const;
    void setIsPairing(bool newIsPairing);

    BluetoothDevicesModel *devicesList() const;

public slots:
    void showHostInfo();
    void startScan();
    void stopScan();
    void requestPairing(const int &deviceIndex);
    void clearDevicesList();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanFinished();
    void scanStopped();

    void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing);
    void pairingError(QBluetoothLocalDevice::Error error);

signals:
    void isScanningChanged();
    void isPairingChanged();

private:
    explicit BluetoothManager(QObject *parent = nullptr);
    void initConnections();
    bool isBusy() const;
    void prepareDevice();

    QBluetoothLocalDevice m_localDevice;
    QBluetoothDeviceDiscoveryAgent m_discoveryAgent;

    bool m_isScanning;
    bool m_isPairing;
    std::unique_ptr<BluetoothDevicesModel> m_devicesList;
};



#endif // BLUETOOTHMANAGER_H
