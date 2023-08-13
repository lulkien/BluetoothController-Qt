#include "bluetoothmanager.h"
#include "common.h"

#define BLE_SCAN_TIMEOUT    5000   // 5 secs

BluetoothManager &BluetoothManager::instance()
{
    static BluetoothManager self;
    return self;
}

BluetoothManager::~BluetoothManager()
{
    LOG_DEBUG;
    m_localDevice.setHostMode(QBluetoothLocalDevice::HostConnectable);
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

void BluetoothManager::stopScan()
{
    LOG_DEBUG << "Request stop discovery";
    if (!m_isScanning)
    {
        LOG_DEBUG << "NO SCANNING, HOW TO STOP DAWG...";
        return;
    }
    if (m_isPairing)
    {
        LOG_DEBUG << "DEVICE IS BUSY... TRY AGAIN LATER.";
        return;
    }
    m_discoveryAgent.stop();
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
    LOG_DEBUG << "Pair Status: " << pairingStatus;
    if (QBluetoothLocalDevice::Unpaired == pairingStatus)
    {
        setIsPairing(true);
        m_localDevice.requestPairing(deviceInfo.address(), QBluetoothLocalDevice::AuthorizedPaired);
    }

}

void BluetoothManager::clearDevicesList()
{
    LOG_DEBUG;
    m_devicesList->clearDevicesList();
}

void BluetoothManager::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    // Process the discovered device
    LOG_DEBUG << "Discovered: Name -" << device.name() << "| Address -" << device.address().toString();
    LOG_DEBUG << "Pair Status: " << m_localDevice.pairingStatus(device.address());
    if (m_localDevice.pairingStatus(device.address()) == QBluetoothLocalDevice::Unpaired)
        m_devicesList->addDevice(device);
}

void BluetoothManager::scanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    LOG_ERROR << "An error occur while scanning:" << error;
    LOG_ERROR << "String: " << m_discoveryAgent.errorString();
    setIsScanning(false);
}

void BluetoothManager::scanFinished()
{
    LOG_DEBUG << "Scan finished";
    setIsScanning(false);
}

void BluetoothManager::scanStopped()
{
    LOG_DEBUG << "Scan stopped";
    setIsScanning(false);
}

void BluetoothManager::pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing)
{
    LOG_WARNING << "Pairing completed";
    LOG_DEBUG << "Address:" << address.toString();
    LOG_DEBUG << "Pairing Status:" << pairing;
    setIsPairing(false);
}

void BluetoothManager::pairingDisplayPinCode(const QBluetoothAddress &address, QString pin)
{
    LOG_DEBUG << "Address:" << address.toString() << "| PIN:" << pin;
}

void BluetoothManager::pairingDisplayConfirmation(const QBluetoothAddress &address, QString pin)
{
    LOG_DEBUG << "Address:" << address.toString() << "| PIN:" << pin;
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
    prepareDevice();
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

void BluetoothManager::initConnections()
{
    // Discovery Agent
    connect(&m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BluetoothManager::deviceDiscovered);
    connect(&m_discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &BluetoothManager::scanError);
    connect(&m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BluetoothManager::scanFinished);
    connect(&m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
            this, &BluetoothManager::scanStopped);

    // Local Device
    connect(&m_localDevice, &QBluetoothLocalDevice::pairingFinished,
            this, &BluetoothManager::pairingFinished);
    connect(&m_localDevice, &QBluetoothLocalDevice::pairingDisplayPinCode,
            this, &BluetoothManager::pairingDisplayPinCode);
    connect(&m_localDevice, &QBluetoothLocalDevice::pairingDisplayConfirmation,
            this, &BluetoothManager::pairingDisplayConfirmation);
    connect(&m_localDevice, &QBluetoothLocalDevice::error,
            this, &BluetoothManager::pairingError);
}

bool BluetoothManager::isBusy() const
{
    return isScanning() || isPairing();
}

void BluetoothManager::prepareDevice()
{
    LOG_DEBUG;
    if (m_localDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff)
        m_localDevice.powerOn();
    m_localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

//    m_discoveryAgent.setLowEnergyDiscoveryTimeout(BLE_SCAN_TIMEOUT);
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
