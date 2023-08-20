#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QBluetoothAddress>
#include <QBluetoothServer>
#include <QObject>

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    virtual ~ChatServer();

    void start(const QBluetoothAddress &localAddress);
    void stop();

public slots:
    void clientConnected();
    void clientDisconnected();
    void readSocket();

signals:
    void notifyClientConnected(const QString &name);
    void notifyClientDisconnected(const QString &name);
    void notifyMessageReceived(const QString &name, const QString &msg);

private:
    QBluetoothServer            *m_rfCommServer;
    QBluetoothServiceInfo       m_serviceInfo;
    QList<QBluetoothSocket *>   m_clientSockets;

};

#endif // CHATSERVER_H
