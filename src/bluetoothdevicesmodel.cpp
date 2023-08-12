#include "bluetoothdevicesmodel.h"
#include <QBluetoothAddress>

BluetoothDevicesModel::BluetoothDevicesModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int BluetoothDevicesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_devices.count();
}

QVariant BluetoothDevicesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_devices.count())
        return QVariant();

    const QBluetoothDeviceInfo &device = m_devices[index.row()];

    if (role == DeviceNameRole)
        return device.name();
    else if (role == DeviceAddressRole)
        return device.address().toString();

    return QVariant();
}

void BluetoothDevicesModel::addDevice(const QBluetoothDeviceInfo &device)
{
    beginInsertRows(QModelIndex(), m_devices.count(), m_devices.count());
    m_devices.append(device);
    endInsertRows();
}

void BluetoothDevicesModel::clearDevicesList()
{
    if (!m_devices.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_devices.count() - 1);
        m_devices.clear();
        endRemoveRows();
    }
}

QBluetoothDeviceInfo BluetoothDevicesModel::at(const int &index) const
{
    return m_devices.at(index);
}

QHash<int, QByteArray> BluetoothDevicesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DeviceNameRole] = "deviceName";
    roles[DeviceAddressRole] = "deviceAddress";
    return roles;
}
