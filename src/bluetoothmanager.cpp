#include "bluetoothmanager.h"
#include "common.h"

BluetoothManager &BluetoothManager::instance()
{
    static BluetoothManager self;
    return self;
}

BluetoothManager::~BluetoothManager()
{

}

void BluetoothManager::startScan()
{
    if (m_scanner.isActive())
        m_scanner.stop();
    m_serviceModel->clearListDevice();

    m_scanner.setUuidFilter(SERVICE_UUID);
    m_scanner.start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
}

void BluetoothManager::stopScan()
{
    m_scanner.stop();
}

ListServicesModel *BluetoothManager::getServiceModel() const
{
    return m_serviceModel;
}

void BluetoothManager::scanCompleted()
{
    setStatusString("Scan finished");
}

void BluetoothManager::serviceFound(const QBluetoothServiceInfo &serviceInfo)
{
    const QString address = serviceInfo.device().address().toString();
    QString remoteName;
    if (serviceInfo.device().name().isEmpty())
        remoteName = address;
    else
        remoteName = serviceInfo.device().name();

    m_serviceModel->addDevice(DeviceInfo{address, remoteName});
}

BluetoothManager::BluetoothManager()
    : m_statusString { QString() }
{
    m_serviceModel = new ListServicesModel();

    connect(&m_scanner, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
            this, &BluetoothManager::serviceFound);
    connect(&m_scanner, &QBluetoothServiceDiscoveryAgent::finished,
            this, &BluetoothManager::scanCompleted);
    connect(&m_scanner, &QBluetoothServiceDiscoveryAgent::canceled,
            this, &BluetoothManager::scanCompleted);
}

QString BluetoothManager::statusString() const
{
    return m_statusString;
}

void BluetoothManager::setStatusString(const QString &newStatusString)
{
    if (m_statusString == newStatusString)
        return;
    m_statusString = newStatusString;
    emit statusStringChanged();
}
