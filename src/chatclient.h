#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>

class ChatClient : public QObject
{
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);
    virtual ~ChatClient();

    void start(const QBluetoothServiceInfo &remoteService);
    void stop();

public slots:
    void sendMessage(const QString &message);

private slots:
    void readSocket();
    void connected();
    void onSocketErrorOccurred(QBluetoothSocket::SocketError error);

signals:
    void notifyMessageReceived(const QString &sender, const QString &message);
    void notifyClientConnected(const QString &name);
    void notifyClientDisconnected();
    void notifySocketErrorOccurred(const QString &errorString);

private:
    QBluetoothSocket    *m_socket;

};

#endif // CHATCLIENT_H
