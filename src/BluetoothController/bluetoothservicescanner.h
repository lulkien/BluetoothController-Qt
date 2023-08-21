#ifndef BLUETOOTHSERVICESCANNER_H
#define BLUETOOTHSERVICESCANNER_H

#include <QObject>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QBluetoothAddress>
#include <QBluetoothUuid>

class BluetoothServiceScanner : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothServiceScanner(const QBluetoothAddress &localAddress, QObject *parent = nullptr);
    virtual ~BluetoothServiceScanner();

    void startScan(const QBluetoothUuid &uuid);
    void stopScan();

private slots:
    void scanCompleted();
    void serviceFound(const QBluetoothServiceInfo &serviceInfo);

signals:

private:
    QBluetoothServiceDiscoveryAgent     *m_scanner;
    QList<QBluetoothServiceInfo>        m_listServicesFound;

};

#endif // BLUETOOTHSERVICESCANNER_H
