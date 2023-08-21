#include "bluetoothservicescanner.h"
#include "common.h"

BluetoothServiceScanner::BluetoothServiceScanner(const QBluetoothAddress &localAddress, QObject *parent)
    : QObject { parent }
{
    m_scanner = new QBluetoothServiceDiscoveryAgent(localAddress, this);
    connect(m_scanner, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
            this, &BluetoothServiceScanner::serviceFound);
    connect(m_scanner, &QBluetoothServiceDiscoveryAgent::finished,
            this, &BluetoothServiceScanner::scanCompleted);
    connect(m_scanner, &QBluetoothServiceDiscoveryAgent::canceled,
            this, &BluetoothServiceScanner::scanCompleted);
}

BluetoothServiceScanner::~BluetoothServiceScanner()
{

}

void BluetoothServiceScanner::startScan(const QBluetoothUuid &uuid)
{
    LOG_DBG;
    if (m_scanner->isActive())
        m_scanner->stop();
    m_scanner->setUuidFilter(uuid);
    m_scanner->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
}

void BluetoothServiceScanner::stopScan()
{
    LOG_DBG;
    if (m_scanner)
        m_scanner->stop();
}

void BluetoothServiceScanner::scanCompleted()
{
    LOG_INF << "Scan completed";
}

void BluetoothServiceScanner::serviceFound(const QBluetoothServiceInfo &serviceInfo)
{
    const QBluetoothAddress address = serviceInfo.device().address();
    for (const QBluetoothServiceInfo &info : qAsConst(m_listServicesFound))
    {
        if (info.device().address() == address)
            return;
    }

    QString remoteName;
    if (serviceInfo.device().name().isEmpty())
        remoteName = address.toString();
    else
        remoteName = serviceInfo.device().name();

    LOG_DBG << "Found:" << remoteName << "|" << address.toString();

}
