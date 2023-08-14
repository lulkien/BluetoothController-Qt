#ifndef BLUETOOTHDEVICESMODEL_H
#define BLUETOOTHDEVICESMODEL_H

#include <QBluetoothDeviceInfo>
#include <QAbstractListModel>

struct BluetoothDeviceData {
    QBluetoothDeviceInfo info;
    int pairStatus;                 // 0: unpair, 1: paired, 2: authorized pair
};

class BluetoothDevicesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DeviceRoles {
        DeviceNameRole = Qt::UserRole + 1,
        DeviceAddressRole
    };

    explicit BluetoothDevicesModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    void addDevice(const QBluetoothDeviceInfo &device);
    void clearDevicesList();
    QBluetoothDeviceInfo at(const int &index) const;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:

private:
    QList<QBluetoothDeviceInfo> m_devices;
};

#endif // BLUETOOTHDEVICESMODEL_H
