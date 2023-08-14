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
    Q_PROPERTY(bool isPairing READ isPairing WRITE setIsPairing NOTIFY isPairingChanged FINAL)
public:
    static BluetoothManager &instance();

    virtual ~BluetoothManager();

    bool isScanning() const;
    void setIsScanning(bool newIsScanning);

    bool isPairing() const;
    void setIsPairing(bool newIsPairing);

    BluetoothDevicesModel *devicesList() const;

public slots:
    void startScan();
    void stopScan();

    void connectToDevice(const int &deviceIndex);

    void clearDevicesList();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanFinished();
    void scanStopped();

signals:
    void isScanningChanged();
    void isPairingChanged();

private:
    explicit BluetoothManager(QObject *parent = nullptr);
    void initConnections();
    bool isBusy() const;
    void prepareDevice();
    void makeNewConnection(const QBluetoothDeviceInfo &device);

    QBluetoothLocalDevice  m_localDevice;
    QBluetoothDeviceDiscoveryAgent m_discoveryAgent;
    QLowEnergyController *m_bleController;

    bool m_isScanning;
    bool m_isPairing;
    std::unique_ptr<BluetoothDevicesModel> m_devicesModel;
};



#endif // BLUETOOTHMANAGER_H
