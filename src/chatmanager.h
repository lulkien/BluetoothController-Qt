#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QObject>
#include <QBluetoothAddress>
#include <QBluetoothServiceInfo>
#include <QBluetoothUuid>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>
#include "chatserver.h"
#include "chatclient.h"
#include "messagesmodel.h"
#include "bluetoothservicescanner.h"

class ChatManager : public QObject
{
    Q_OBJECT
public:
    static ChatManager &instance();
    virtual ~ChatManager();

    MessagesModel *getConversation() const;

private:
    ChatManager();

public slots:
    void sendMessage(const QString &msg);

private slots:
    void showMessage(const QString &sender, const QString &msg);

    // other device connect/disconnect to this server
    void onClientConnectedToServer(const QString &name);
    void onClientDisconnectedFromServer(const QString &name);

    // this device connect/disconnect to other server
    void onDisconnected();
    void onConnected(const QString &name);
    void onSocketError(const QString &error);

signals:
    void reqSendMessage(const QString &msg);

private:
    QBluetoothLocalDevice       m_localDevices;
    QList<QBluetoothHostInfo>   m_listAdaptors;

    ChatServer                  *m_server;
    QList<ChatClient *>         m_clients;
    QString                     m_deviceName;

    MessagesModel               *m_conversationModel;

};



#endif // CHATMANAGER_H
