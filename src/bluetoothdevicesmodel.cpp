#include "bluetoothdevicesmodel.h"
#include "common.h"
#include <QBluetoothAddress>

BluetoothDevicesModel::BluetoothDevicesModel(QObject *parent)
    : QAbstractListModel    { parent }
    , m_connectedDevice     { nullptr }
{

}

BluetoothDevicesModel::~BluetoothDevicesModel()
{
    if (nullptr != m_connectedDevice)
        delete m_connectedDevice;
    m_connectedDevice = nullptr;
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

void BluetoothDevicesModel::addDevice(const QBluetoothDeviceInfo &newDevice)
{
#if 0
    if (!m_devices.contains(device))
    {
        beginInsertRows(QModelIndex(), m_devices.count(), m_devices.count());
        m_devices.append(device);
        endInsertRows();
    }
    else
    {
        LOG_INFO << "Device found before.";
    }
#else
    foreach (QBluetoothDeviceInfo device, m_devices)
    {
        if (device.address() == newDevice.address())
        {
            LOG_DBG << "Address" << newDevice.address() << "was found. Ignore.";
            return;
        }
    }
    beginInsertRows(QModelIndex(), m_devices.count(), m_devices.count());
    m_devices.append(newDevice);
    endInsertRows();
#endif
}

void BluetoothDevicesModel::clearDevicesList()
{
    if (!m_devices.isEmpty())
    {
        beginRemoveRows(QModelIndex(), 0, m_devices.count() - 1);
        m_devices.clear();
        endRemoveRows();
    }
}

QBluetoothDeviceInfo BluetoothDevicesModel::at(const int &index) const
{
    return m_devices.at(index);
}

void BluetoothDevicesModel::setConnectedDevice(QBluetoothDeviceInfo *targetDevice)
{
    // delete old info and make new one
    if (nullptr != m_connectedDevice)
        delete m_connectedDevice;

    if (nullptr != targetDevice)
        LOG_INF << "Connected to" << targetDevice->name()
                << ", address:" << targetDevice->address();
    else
        LOG_INF << "Removed connection.";

    m_connectedDevice = targetDevice;
}

QBluetoothDeviceInfo *BluetoothDevicesModel::connectedDevice() const
{
    return m_connectedDevice;
}

QHash<int, QByteArray> BluetoothDevicesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DeviceNameRole] = "deviceName";
    roles[DeviceAddressRole] = "deviceAddress";
    return roles;
}
