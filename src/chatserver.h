#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QBluetoothAddress>
#include <QBluetoothServer>
#include <QObject>
#include <QQmlEngine>
#include <QRegularExpression>
#include <QUuid>

class ChatServer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ChatServer(QObject *parent = nullptr);
    ~ChatServer();

    void startServer(const QBluetoothAddress &localAdapter = QBluetoothAddress());
    void stopServer();
    QString generateUUIDv4();

public slots:
    void sendMessage(const QString &message);

signals:
    void messageReceived(const QString &sender, const QString &message);
    void notifyClientConnected(const QString &name);
    void notifyClientDisconnected(const QString &name);

private slots:
    void clientConnected();
    void clientDisconnected();
    void readSocket();

private:
    QBluetoothServer *rfcommServer = nullptr;
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket *> clientSockets;
    QMap<QBluetoothSocket *, QString> clientNames;
    QString m_serviceUuid;
};

#endif // CHATSERVER_H
