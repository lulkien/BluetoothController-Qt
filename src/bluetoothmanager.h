#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <memory>
#include "bluetoothdevicesmodel.h"

class BluetoothManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isScanning READ isScanning WRITE setIsScanning NOTIFY isScanningChanged FINAL)
public:
    static BluetoothManager &instance();

    virtual ~BluetoothManager();

    bool isScanning() const;
    void setIsScanning(bool newIsScanning);

    BluetoothDevicesModel *devicesList() const;

public slots:
    void startScan();
    void stopScan();

    void connectToDevice(const int &deviceIndex);

    void clearDevicesList();

private slots:
    // discovery agent
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanFinished();
    void scanStopped();

    // local device
    void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing);
    void pairingDisplayPinCode(const QBluetoothAddress &address, QString pin);
    void pairingDisplayConfirmation(const QBluetoothAddress &address, QString pin);
    void pairingError(QBluetoothLocalDevice::Error error);

signals:
    void isScanningChanged();

private:
    explicit BluetoothManager(QObject *parent = nullptr);
    void initConnections();
    void prepareDevice();
    void makeNewConnection(const QBluetoothDeviceInfo &device);

    QBluetoothLocalDevice  m_localDevice;
    QBluetoothDeviceDiscoveryAgent m_discoveryAgent;
    QLowEnergyController *m_bleController;

    bool m_isScanning;
    bool m_connectAfterPaired;
    std::unique_ptr<BluetoothDevicesModel> m_devicesModel;
};



#endif // BLUETOOTHMANAGER_H
