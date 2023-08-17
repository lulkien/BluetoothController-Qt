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
    if (m_isScanning)
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
    m_discoveryAgent.stop();
}

void BluetoothManager::connectToDevice(const int &deviceIndex)
{
    QBluetoothDeviceInfo targetDevice = m_devicesModel->at(deviceIndex);
    QString currentConnectedDevice = m_devicesModel->connectedAddress();

    // Dont care about anything, just disconnect the BLE Controller
    if (nullptr != m_bleController)
    {
        // check if trying to make a new connection to a connected device
        if (currentConnectedDevice == targetDevice.address().toString())
        {
            LOG_WRN << "Trying to make new connection with same device.";
            return;
        }

        // disconnect old connection
        m_bleController->disconnectFromDevice();
        m_bleController->deleteLater();
        m_bleController = nullptr;

        // make new connection
        m_devicesModel->setConnectedAddress(QString());
    }
    else
    {
        // if current connected device is NULL -> no connection -> make new connection
        if (currentConnectedDevice.isEmpty())
        {
            makeNewConnection(targetDevice);
            return;
        }
        else
        {
            LOG_WRN << "SOMETHING MUST HAVE GONE WRONG IF THIS LOG IS PRINTED...";
        }
    }
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
//    if (m_localDevice.pairingStatus(device.address()) == QBluetoothLocalDevice::Unpaired)
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

void BluetoothManager::pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing)
{
    LOG_INF << "Address:" << address.toString() << "| Pairing Status:" << pairing;
    if (m_connectAfterPaired)
    {
        m_connectAfterPaired = false;
        m_bleController->connectToDevice();
    }
}

void BluetoothManager::pairingDisplayPinCode(const QBluetoothAddress &address, QString pin)
{
    LOG_INF << "Address:" << address.toString() << "| PIN:" << pin;
}

void BluetoothManager::pairingDisplayConfirmation(const QBluetoothAddress &address, QString pin)
{
    LOG_INF << "Address:" << address.toString() << "| PIN:" << pin;
}

void BluetoothManager::pairingError(QBluetoothLocalDevice::Error error)
{
    LOG_ERR << error;
}

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject   { parent }
    , m_isScanning  { false }
    , m_connectAfterPaired  { false }
    , m_devicesModel    { std::make_unique<BluetoothDevicesModel>() }
{
    LOG_DBG << "Create";
    prepareDevice();
    initConnections();
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

    // Local device
    connect(&m_localDevice, &QBluetoothLocalDevice::pairingFinished,
            this, &BluetoothManager::pairingFinished);
    connect(&m_localDevice, QOverload<QBluetoothLocalDevice::Error>::of(&QBluetoothLocalDevice::error),
            this, &BluetoothManager::pairingError);
    connect(&m_localDevice, &QBluetoothLocalDevice::pairingDisplayConfirmation,
            this, &BluetoothManager::pairingDisplayConfirmation);
    connect(&m_localDevice, &QBluetoothLocalDevice::pairingDisplayPinCode,
            this, &BluetoothManager::pairingDisplayPinCode);
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

    connect(m_bleController, &QLowEnergyController::stateChanged,
            this, [](QLowEnergyController::ControllerState state) {
                LOG_DBG << "Conntroller State: ->" << state;
            });

    connect(m_bleController, &QLowEnergyController::connected,
            this, [device, this]() {
                LOG_INF << "Controller connected.";
                m_devicesModel->setConnectedAddress(device.address());
                m_bleController->discoverServices();
            });

    connect(m_bleController, &QLowEnergyController::disconnected,
            this, [this]() {
                LOG_INF << "LowEnergy controller disconnected.";
                m_devicesModel->setConnectedAddress(QString());
            });

    connect(m_bleController, &QLowEnergyController::serviceDiscovered,
            this, [](const QBluetoothUuid &newService) {
                LOG_DBG << "Found service:" << newService;
            });

    // pairing before make new connection if it not paired
    // use QBluetoothLocalDevice
#if 0
    // If you wanna pair before connect
    if (m_localDevice.pairingStatus(device.address()) != QBluetoothLocalDevice::Unpaired)
    {
        m_bleController->connectToDevice();
    }
    else
    {
        LOG_DBG << "Not paired";
        m_connectAfterPaired = true;
        m_localDevice.requestPairing(device.address(), QBluetoothLocalDevice::AuthorizedPaired);
    }
#else
    // Just connect to device
    m_bleController->connectToDevice();
#endif
}

void BluetoothManager::scanAvailableServices()
{
    if (nullptr == m_bleController)
    {
        LOG_WRN << "Cannot scan if m_bleController not available.";
        return;
    }
    m_bleController->discoverServices();
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
