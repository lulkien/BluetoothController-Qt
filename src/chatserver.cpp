#include "chatserver.h"

//using namespace Qt::StringLiterals;
//! [Service UUID]
//static constexpr auto serviceUuid = "06b33a00-88e5-4389-945d-1fc0cbae1bd8"_L1;
//! [Service UUID]

ChatServer::ChatServer(QObject *parent)
    : QObject{parent}
{
    m_serviceUuid = generateUUIDv4();
}

ChatServer::~ChatServer()
{
    stopServer();
}

void ChatServer::startServer(const QBluetoothAddress &localAdapter)
{
    if (rfcommServer)
        return;

    //! [Create the server]
    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcommServer, &QBluetoothServer::newConnection,
            this, QOverload<>::of(&ChatServer::clientConnected));
    bool result = rfcommServer->listen(localAdapter);
    if (!result) {
        qWarning() << "Cannot bind chat server to" << localAdapter.toString();
        return;
    }
    //! [Create the server]

    //serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceRecordHandle, (uint)0x00010010);

    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));
    classId << QVariant::fromValue(quint16(0x100));
    profileSequence.append(QVariant::fromValue(classId));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             profileSequence);

    classId.clear();
    classId << QVariant::fromValue(QBluetoothUuid(m_serviceUuid));
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    //! [Service name, description and provider]
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Bt Chat Server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("Example bluetooth chat server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("qt-project.org"));
    //! [Service name, description and provider]

    //! [Service UUID set]
    serviceInfo.setServiceUuid(QBluetoothUuid(m_serviceUuid));
    //! [Service UUID set]

    //! [Service Discoverability]
    const auto groupUuid = QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::PublicBrowseGroup);
    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(groupUuid);
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList, publicBrowse);
    //! [Service Discoverability]

    //! [Protocol descriptor list]
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ProtocolUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ProtocolUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);
    //! [Protocol descriptor list]

    //! [Register service]
    serviceInfo.registerService(localAdapter);
    //! [Register service]
}

void ChatServer::stopServer()
{
    // Unregister service
    serviceInfo.unregisterService();

    // Close sockets
    qDeleteAll(clientSockets);
    clientNames.clear();

    // Close server
    delete rfcommServer;
    rfcommServer = nullptr;
}

QString ChatServer::generateUUIDv4() {
    // Generate a UUIDv4
    QUuid uuid = QUuid::createUuid();

    // Convert the UUID to a QString
    QString uuidString = uuid.toString();

    // Remove curly braces and hyphens to get a clean UUID string
    static QRegularExpression regex("[{}]");
    uuidString.remove(regex);

    return uuidString;
}

void ChatServer::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';

//    for (QBluetoothSocket *socket : std::as_const(clientSockets))
//        socket->write(text);
}

void ChatServer::clientConnected()
{
    QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, &QBluetoothSocket::readyRead, this, &ChatServer::readSocket);
    connect(socket, &QBluetoothSocket::disconnected,
            this, &ChatServer::clientDisconnected);
    clientSockets.append(socket);
    clientNames[socket] = socket->peerName();
    emit notifyClientConnected(socket->peerName());
}

void ChatServer::clientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    emit notifyClientDisconnected(clientNames[socket]);

    clientSockets.removeOne(socket);
    clientNames.remove(socket);

    socket->deleteLater();
}

void ChatServer::readSocket()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        emit messageReceived(clientNames[socket],
                             QString::fromUtf8(line.constData(), line.length()));
    }
}
