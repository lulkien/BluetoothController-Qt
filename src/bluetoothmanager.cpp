#include "bluetoothmanager.h"
#include "common.h"

BluetoothManager &BluetoothManager::instance()
{
    static BluetoothManager self;
    return self;
}

BluetoothManager *BluetoothManager::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    // Create the object using some custom constructor or factory.
    // The QML engine will assume ownership and delete it, eventually.
    return &instance();
}

BluetoothManager::~BluetoothManager()
{

}

void BluetoothManager::showHostInfo()
{

}

void BluetoothManager::startScan()
{
    LOG_DEBUG;
    if (isBusy())
    {
        LOG_DEBUG << "DEVICE IS BUSY... TRY AGAIN LATER.";
        return;
    }
    setIsScanning(true);
    m_devicesList->clearDevicesList();
    m_discoveryAgent.start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);
}

void BluetoothManager::requestPairing(const int &deviceIndex)
{
    if (isBusy())
    {
        LOG_DEBUG << "DEVICE IS BUSY... TRY AGAIN LATER.";
        return;
    }

    LOG_WARNING << "Index:" << deviceIndex;

    const QBluetoothDeviceInfo deviceInfo = m_devicesList->at(deviceIndex);
    LOG_DEBUG << "Name:" << deviceInfo.name();
    LOG_DEBUG << "Address:" << deviceInfo.address();

    QBluetoothLocalDevice::Pairing pairingStatus = m_localDevice.pairingStatus(deviceInfo.address());
    LOG_DEBUG << "Pairing Status: " << pairingStatus;
    if (QBluetoothLocalDevice::Unpaired == pairingStatus)
    {
        setIsPairing(true);
        m_localDevice.requestPairing(deviceInfo.address(), QBluetoothLocalDevice::Paired);
    }

}

void BluetoothManager::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    // Process the discovered device
    LOG_DEBUG << "Discovered: Name -" << device.name() << "| Address -" << device.address().toString();
    m_devicesList->addDevice(device);
}

void BluetoothManager::scanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    LOG_ERROR << "An error occur while scanning:" << error;
    setIsScanning(false);
}

void BluetoothManager::scanFinished()
{
    LOG_DEBUG << "Scan finished";
    setIsScanning(false);
}

void BluetoothManager::pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing)
{
    LOG_WARNING << "Pairing completed";
    LOG_DEBUG << "Address:" << address.toString();
    LOG_DEBUG << "Pairing Status:" << pairing;
    setIsPairing(false);
}

void BluetoothManager::pairingError(QBluetoothLocalDevice::Error error)
{
    LOG_ERROR << "An error occur while pairing:" << error;
    setIsPairing(false);
}

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject           { parent }
    , m_isScanning      { false }
    , m_isPairing       { false }
    , m_devicesList     { std::make_unique<BluetoothDevicesModel>() }
{
    LOG_DEBUG << "Create";
    initConnections();
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

void BluetoothManager::initConnections()
{
    // Discovery Agent
    connect(&m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BluetoothManager::deviceDiscovered);
    connect(&m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &BluetoothManager::scanError);
    connect(&m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BluetoothManager::scanFinished);

    // Local Device
    connect(&m_localDevice, &QBluetoothLocalDevice::pairingFinished,
            this, &BluetoothManager::pairingFinished);
    connect(&m_localDevice, &QBluetoothLocalDevice::errorOccurred,
            this, &BluetoothManager::pairingError);
}

bool BluetoothManager::isBusy() const
{
    return isScanning() || isPairing();
}

void BluetoothManager::makeVisibleForOther()
{

}

bool BluetoothManager::isScanning() const
{
    return m_isScanning;
}

void BluetoothManager::setIsScanning(bool newIsScanning)
{
    if (m_isScanning == newIsScanning)
        return;
    m_isScanning = newIsScanning;
    emit isScanningChanged();
}

BluetoothDevicesModel *BluetoothManager::devicesList() const
{
    return m_devicesList.get();
}

bool BluetoothManager::isPairing() const
{
    return m_isPairing;
}

void BluetoothManager::setIsPairing(bool newIsPairing)
{
    if (m_isPairing == newIsPairing)
        return;
    m_isPairing = newIsPairing;
    emit isPairingChanged();
}
