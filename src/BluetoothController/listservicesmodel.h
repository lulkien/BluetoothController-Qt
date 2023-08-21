#ifndef LISTSERVICESMODEL_H
#define LISTSERVICESMODEL_H

#include <QAbstractListModel>
#include <QObject>

struct DeviceInfo {
    QString address;
    QString remoteName;
};

class ListServicesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DeviceRoles {
        AddressRole = Qt::UserRole + 1,
        RemoteNameRole
    };

    explicit ListServicesModel(QObject *parent = nullptr);
    virtual ~ListServicesModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addDevice(const DeviceInfo &dev);
    void clearListDevice();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<DeviceInfo> m_devicesList;
};

#endif // LISTSERVICESMODEL_H
