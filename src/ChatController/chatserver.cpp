#include "chatserver.h"
#include "common.h"
#include "qbluetoothaddress.h"

ChatServer::ChatServer(QObject *parent)
    : QObject { parent }
    , m_rfCommServer { nullptr }
{
    LOG_DBG;
}

ChatServer::~ChatServer()
{
    LOG_DBG;
    stop();
}

void ChatServer::start(const QBluetoothAddress &localAddress)
{
    // Create new instance of server if it not runnign
    if (nullptr != m_rfCommServer)
    {
        LOG_WRN << "Server is running.";
        return;
    }

    m_rfCommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(m_rfCommServer, &QBluetoothServer::newConnection,
            this, &ChatServer::clientConnected);

    if (!m_rfCommServer->listen(localAddress))
    {
        LOG_WRN << "Cannot bind server to address" << localAddress.toString();
        return;
    }

    // The rest of this shit is make some characteritics and register service
    // I have no idea what it is
    // Just copy and paste

    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    classId << QVariant::fromValue(quint16(0x100));
    profileSequence.append(QVariant::fromValue(classId));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                               profileSequence);

    classId.clear();
    classId << QVariant::fromValue(SERVICE_UUID);
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    //! [Service name, description and provider]
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Bt Chat Server"));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                               tr("Example bluetooth chat server"));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("qt-project.org"));
    //! [Service name, description and provider]

    //! [Service UUID set]
    m_serviceInfo.setServiceUuid(SERVICE_UUID);
    //! [Service UUID set]

    //! [Service Discoverability]
    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                               publicBrowse);
    //! [Service Discoverability]

    //! [Protocol descriptor list]
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(m_rfCommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                               protocolDescriptorList);
    //! [Protocol descriptor list]

    //! [Register service]
    m_serviceInfo.registerService(localAddress);
    //! [Register service]

    /**
     * [ ChatGPT explained ]
     * This code snippet continues from the previous context and explains how the service information for the Bluetooth chat server is configured and registered using the QBluetoothServiceInfo class.
     * This part of the code focuses on setting attributes related to the server's characteristics and registering the service.
     *
     * Bluetooth Profile Descriptor List (BluetoothProfileDescriptorList Attribute):
     *
     * A QBluetoothServiceInfo::Sequence named profileSequence is created to hold attributes for the Bluetooth profile descriptors.
     * Inside profileSequence, another QBluetoothServiceInfo::Sequence named classId is created.
     * Two values are added to classId: the UUID for the Serial Port profile (QBluetoothUuid::SerialPort) and a quint16 value 0x100.
     * The classId sequence is then appended to the profileSequence to set the profile descriptor.
     * Service Class Identifiers (ServiceClassIds Attribute):
     *
     * A new classId sequence is created and populated with the chat service's UUID and the Serial Port profile UUID.
     * The ServiceClassIds attribute of the service information (m_serviceInfo) is set to this classId sequence.
     * Service Name, Description, and Provider:
     *
     * Attributes for the service's name, description, and provider are set using the setAttribute function.
     * These attributes help identify the service to clients and provide information about the service.
     * Service UUID Set (setServiceUuid Function):
     *
     * The service's UUID is set using the setServiceUuid function. This UUID uniquely identifies the Bluetooth service provided by the chat server.
     * Service Discoverability (BrowseGroupList Attribute):
     *
     * A QBluetoothServiceInfo::Sequence named publicBrowse is created and populated with the Public Browse Group UUID.
     * The BrowseGroupList attribute is set to this publicBrowse sequence. This attribute makes the service discoverable by other devices.
     * Protocol Descriptor List (ProtocolDescriptorList Attribute):
     *
     * A QBluetoothServiceInfo::Sequence named protocolDescriptorList is created to hold attributes for the protocol descriptors.
     * Inside this sequence, a protocol sequence is created, and L2CAP and RFCOMM protocol UUIDs are added.
     * The server's RFCOMM port number is also included in the protocol sequence.
     * The protocol sequence is added to the protocolDescriptorList.
     * The ProtocolDescriptorList attribute is set to this protocolDescriptorList.
     * Register Service:
     *
     * After setting up the service information attributes, the service is registered using the registerService function of m_serviceInfo.
     * The local Bluetooth address of the adapter (localAddress) is provided as an argument to the registerService function.
     * Overall, this part of the code configures various attributes of the Bluetooth service provided by the chat server, such as the UUID, name, description, protocol descriptors, and discoverability information. Once all the attributes are set, the service is registered with the local Bluetooth adapter, making it available for discovery by other devices.
    **/
}

void ChatServer::stop()
{
    LOG_DBG;
    m_serviceInfo.unregisterService();
    qDeleteAll(m_connectedSockets);
}

void ChatServer::sendMessage(const QString &message)
{
    // send message to every connected sockets
    QByteArray text = message.toUtf8() + '\n';

    for (QBluetoothSocket *socket : qAsConst(m_connectedSockets))
        socket->write(text);
}

void ChatServer::clientConnected()
{
    LOG_DBG;
    QBluetoothSocket *socket = m_rfCommServer->nextPendingConnection();
    if (nullptr == socket)
    {
        LOG_WRN << "ROSES ARE RED";
        return;
    }

    connect(socket, &QBluetoothSocket::readyRead, this, &ChatServer::readSocket);
    connect(socket, &QBluetoothSocket::disconnected, this, &ChatServer::clientDisconnected);

    m_connectedSockets.append(socket);
    emit notifyClientConnected(socket->peerName());
}

void ChatServer::clientDisconnected()
{
    LOG_DBG;
    // get the sender of signal disconnect and remove it from list of connected socket
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (nullptr == socket)
    {
        LOG_WRN << "VIOLETS ARE BLUE";
        return;
    }

    emit notifyClientDisconnected(socket->peerName());

    m_connectedSockets.removeOne(socket);
    socket->deleteLater();
}

void ChatServer::readSocket()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (nullptr == socket)
    {
        LOG_WRN << "HALO IS INFINITE BUT DOOM IS ETERNAL.";
        return;
    }

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        emit notifyMessageReceived(socket->peerName(),
                                   QString::fromUtf8(line.constData(), line.length()));
    }
}
