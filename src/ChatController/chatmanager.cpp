#include <QGuiApplication>
#include <QQmlEngine>
#include "chatmanager.h"
#include "common.h"

#define BLE_SCAN_TIMEOUT    5000   // 5 secs

ChatManager &ChatManager::instance()
{
    static ChatManager self;
    return self;
}

ChatManager::~ChatManager()
{
    LOG_DBG;
}

MessagesModel *ChatManager::getConversation() const
{
    return m_conversationModel;
}

ChatManager::ChatManager()
{
    LOG_DBG << "Create bluetooth manager";

    // get adaptors info
    m_listAdaptors = m_localDevices.allDevices();
    if (m_listAdaptors.count() < 1)
    {
        LOG_ERR << "Bluetooth adaptor not found";
        QGuiApplication::quit();
    }

    // init chat server and connect signals/slots
    m_server = new ChatServer(this);
    // server to controller
    connect(m_server, &ChatServer::notifyClientConnected,
            this, &ChatManager::onClientConnectedToServer);
    connect(m_server, &ChatServer::notifyClientDisconnected,
            this, &ChatManager::onClientDisconnectedFromServer);
    connect(m_server, &ChatServer::notifyMessageReceived,
            this, &ChatManager::showMessage);

    // controller to server
    connect(this, &ChatManager::reqSendMessage,
            m_server, &ChatServer::sendMessage);
    m_server->start(m_listAdaptors.at(0).address());

    // get local device name
    m_deviceName = m_listAdaptors.at(0).name();

    // create conversation model
    m_conversationModel = new MessagesModel(this);

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

void ChatManager::sendMessage(const QString &msg)
{
    LOG_INF << "Send message:" << msg;
//    showMessage(m_deviceName, msg);
    m_conversationModel->addMessage(m_deviceName, msg, true);
    emit reqSendMessage(msg);
}

void ChatManager::showMessage(const QString &sender, const QString &msg)
{
    LOG_DBG << "Sender:" << sender << "| Message:" << msg;
}

void ChatManager::onClientConnectedToServer(const QString &name)
{
    LOG_INF << name << "joined the chat.";
}

void ChatManager::onClientDisconnectedFromServer(const QString &name)
{
    LOG_INF << name << "left the chat.";
}

void ChatManager::onDisconnected()
{

}

void ChatManager::onConnected(const QString &name)
{

}

void ChatManager::onSocketError(const QString &error)
{

}
