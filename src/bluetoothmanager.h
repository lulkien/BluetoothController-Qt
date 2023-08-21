#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QObject>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QBluetoothAddress>
#include <QBluetoothUuid>
#include "listservicesmodel.h"

class BluetoothManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString statusString READ statusString WRITE setStatusString NOTIFY statusStringChanged FINAL)
public:
    static BluetoothManager &instance();
    virtual ~BluetoothManager();

public:
    void startScan();
    void stopScan();
    ListServicesModel *getServiceModel() const;

    QString statusString() const;
    void setStatusString(const QString &newStatusString);

private slots:
    void scanCompleted();
    void serviceFound(const QBluetoothServiceInfo &serviceInfo);

private:
    BluetoothManager();

signals:

    void statusStringChanged();

private:
    QBluetoothServiceDiscoveryAgent     m_scanner;
    ListServicesModel                   *m_serviceModel;
    QString m_statusString;
};

#endif // BLUETOOTHMANAGER_H
