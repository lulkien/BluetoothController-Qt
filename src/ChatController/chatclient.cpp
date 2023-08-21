#include <QMetaEnum>
#include "chatclient.h"
#include "common.h"

ChatClient::ChatClient(QObject *parent)
    : QObject {parent}
    , m_socket { nullptr }
{

}

ChatClient::~ChatClient()
{
    stop();
}

void ChatClient::start(const QBluetoothServiceInfo &remoteService)
{
    if (nullptr != m_socket)
    {
        return;
    }

    // Connect to service
    m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    LOG_DBG<< "Create socket";
    m_socket->connectToService(remoteService);
    LOG_DBG << "ConnectToService done";

    connect(m_socket, &QBluetoothSocket::readyRead, this, &ChatClient::readSocket);
    connect(m_socket, &QBluetoothSocket::connected, this, &ChatClient::connected);
    connect(m_socket, &QBluetoothSocket::disconnected, this, &ChatClient::notifyClientDisconnected);
    connect(m_socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &ChatClient::onSocketErrorOccurred);
}

void ChatClient::stop()
{
    if (nullptr != m_socket)
        delete m_socket;
    m_socket = nullptr;
}

void ChatClient::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    m_socket->write(text);
}

void ChatClient::readSocket()
{
    if (nullptr == m_socket)
    {
        return;
    }

    while (m_socket->canReadLine()) {
        QByteArray line = m_socket->readLine();
        emit notifyMessageReceived(m_socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}

void ChatClient::connected()
{
    emit notifyClientConnected(m_socket->peerName());
}

void ChatClient::onSocketErrorOccurred(QBluetoothSocket::SocketError error)
{
    if (error == QBluetoothSocket::NoSocketError)
        return;

    QMetaEnum metaEnum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();
    QString errorString = m_socket->peerName() + QLatin1Char(' ')
                          + metaEnum.valueToKey(error) + QLatin1String(" occurred");

    emit notifySocketErrorOccurred(errorString);
}
