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
    void setConnectedDevice(QBluetoothDeviceInfo *targetDevice);
    QBluetoothDeviceInfo *connectedDevice() const;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:

private:
    QList<QBluetoothDeviceInfo> m_devices;
    QBluetoothDeviceInfo *m_connectedDevice;    // nullptr if not connect with any device
};

#endif // BLUETOOTHDEVICESMODEL_H
