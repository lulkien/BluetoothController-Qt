#include "listservicesmodel.h"
#include "common.h"

ListServicesModel::ListServicesModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

ListServicesModel::~ListServicesModel()
{

}

int ListServicesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return m_devicesList.count();
}

QVariant ListServicesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_devicesList.count())
    {
        return QVariant();
    }

    const DeviceInfo &device = m_devicesList[index.row()];

    switch (role) {
    case AddressRole:
        return device.address;
    case RemoteNameRole:
        return device.remoteName;
    default:
        return QVariant();
    }
}

void ListServicesModel::addDevice(const DeviceInfo &dev)
{
    foreach (DeviceInfo info, m_devicesList)
    {
        if (info.address == dev.address)
        {
            LOG_DBG << "Device" << dev.address << "was found before.";
            return;
        }
    }

    LOG_INF << "Add device" << dev.address << "|" << dev.remoteName;
    beginInsertRows(QModelIndex(), m_devicesList.count(), m_devicesList.count());
    m_devicesList.append(dev);
    endInsertRows();
}

void ListServicesModel::clearListDevice()
{
    beginResetModel();
    m_devicesList.clear();
    endResetModel();
}

QHash<int, QByteArray> ListServicesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AddressRole] = "address";
    roles[RemoteNameRole] = "remoteName";
    return roles;
}
