#include <QtGlobal>
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
    LOG_DBG;
    m_localDevice.setHostMode(QBluetoothLocalDevice::HostConnectable);
}

void BluetoothManager::startScan()
{
    LOG_DBG;
    if (isBusy())
    {
        LOG_DBG << "DEVICE IS BUSY... TRY AGAIN LATER.";
        return;
    }
    setIsScanning(true);
    m_devicesModel->clearDevicesList();
    m_discoveryAgent.start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);
}

void BluetoothManager::stopScan()
{
    LOG_DBG << "Request stop discovery";
    if (!m_isScanning)
    {
        LOG_DBG << "NO SCANNING, HOW TO STOP DAWG...";
        return;
    }
    if (m_isPairing)
    {
        LOG_DBG << "DEVICE IS BUSY... TRY AGAIN LATER.";
        return;
    }
    m_discoveryAgent.stop();
}

void BluetoothManager::connectToDevice(const int &deviceIndex)
{
    // get the info of device we want to connect
    // get the info of current connected device
    // if there is no connection -> make new one
    // if there is an connection with same device -> ignore and return
    // if there is an connection -> disconnect it and make new one

    QBluetoothDeviceInfo targetDevice = m_devicesModel->at(deviceIndex);
    QBluetoothDeviceInfo *currentConnectedDevice = m_devicesModel->connectedDevice();

    // Dont care about anything, just disconnect the BLE Controller
    if (nullptr != m_bleController)
    {
        m_bleController->disconnectFromDevice();
        delete m_bleController;
    }
    m_bleController = nullptr;

    // if current connected device is NULL -> no connection -> make new connection
    if (nullptr == currentConnectedDevice)
    {
        makeNewConnection(targetDevice);
        return;
    }

    if (currentConnectedDevice->address() == targetDevice.address())
    {
        LOG_WRN << "Trying to make new connection with same device.";
        return;
    }

    m_devicesModel->setConnectedDevice(nullptr);
    makeNewConnection(targetDevice);
}

void BluetoothManager::clearDevicesList()
{
    LOG_DBG;
    m_devicesModel->clearDevicesList();
}

void BluetoothManager::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    // Process the discovered device
    LOG_DBG << "Discovered: Name -" << device.name() << "| Address -" << device.address().toString();
    LOG_DBG << "Pair Status: " << m_localDevice.pairingStatus(device.address());
    if (m_localDevice.pairingStatus(device.address()) == QBluetoothLocalDevice::Unpaired)
        m_devicesModel->addDevice(device);
}

void BluetoothManager::scanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    LOG_ERR << "An error occur while scanning:" << error;
    LOG_ERR << "String: " << m_discoveryAgent.errorString();
    setIsScanning(false);
}

void BluetoothManager::scanFinished()
{
    LOG_DBG << "Scan finished";
    setIsScanning(false);
}

void BluetoothManager::scanStopped()
{
    LOG_DBG << "Scan stopped";
    setIsScanning(false);
}

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject           { parent }
    , m_isScanning      { false }
    , m_isPairing       { false }
    , m_devicesModel     { std::make_unique<BluetoothDevicesModel>() }
{
    LOG_DBG << "Create";
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
}

bool BluetoothManager::isBusy() const
{
    return isScanning() || isPairing();
}

void BluetoothManager::prepareDevice()
{
    LOG_DBG;
    if (m_localDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff)
        m_localDevice.powerOn();
    m_localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    //    m_discoveryAgent.setLowEnergyDiscoveryTimeout(BLE_SCAN_TIMEOUT);
}

void BluetoothManager::makeNewConnection(const QBluetoothDeviceInfo &device)
{
    LOG_DBG;

    // Stop discovery agent
    m_discoveryAgent.stop();

    m_bleController = QLowEnergyController::createCentral(device, this);
    m_bleController->setRemoteAddressType(QLowEnergyController::PublicAddress);

    // Connect signal/slot
    connect(m_bleController, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
            this, [](QLowEnergyController::Error error) {
                LOG_ERR << "Connection error: " << error;
                return;
            });

    connect(m_bleController, &QLowEnergyController::connected,
            this, [device, this]() {
                LOG_INF << "Controller connected.";
                m_devicesModel->setConnectedDevice(new QBluetoothDeviceInfo(device));
            });

    connect(m_bleController, &QLowEnergyController::disconnected,
            this, [this]() {
                LOG_INF << "LowEnergy controller disconnected.";
                m_devicesModel->setConnectedDevice(nullptr);
            });

    m_bleController->connectToDevice();
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
    return m_devicesModel.get();
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
