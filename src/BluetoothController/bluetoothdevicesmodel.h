#ifndef BLUETOOTHDEVICESMODEL_H
#define BLUETOOTHDEVICESMODEL_H

#include <QBluetoothDeviceInfo>
#include <QAbstractListModel>

class BluetoothDevicesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DeviceRoles {
        DeviceNameRole = Qt::UserRole + 1,
        DeviceAddressRole
    };

    explicit BluetoothDevicesModel(QObject *parent = nullptr);
    virtual ~BluetoothDevicesModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void addDevice(const QBluetoothDeviceInfo &newDevice);
    void clearDevicesList();

    QBluetoothDeviceInfo at(const int &index) const;
    void setConnectedAddress(const QBluetoothAddress &newAddress);
    void setConnectedAddress(const QString &newAddress);
    QString connectedAddress() const;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:

private:
    QList<QBluetoothDeviceInfo> m_devices;
    QString m_connectedAddress;    // EMPTY string if not connect with any device
};

#endif // BLUETOOTHDEVICESMODEL_H
