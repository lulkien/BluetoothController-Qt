#ifndef MESSAGESMODEL_H
#define MESSAGESMODEL_H

#include <QAbstractListModel>

struct MessageData {
    QString sender;
    QString message;
    bool sendByThisDevice;
    MessageData(QString _sender, QString _msg, bool _owner = false)
        : sender { _sender }
        , message { _msg }
        , sendByThisDevice { _owner }
    { }
};

class MessagesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DeviceRoles {
        SenderRole = Qt::UserRole + 1,
        MessageRole,
        OwnerRole,
    };

    explicit MessagesModel(QObject *parent = nullptr);
    virtual ~MessagesModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void addMessage(const QString &sender, const QString &message, bool sendByThisDevice = false);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<MessageData> m_conversation;

};

#endif // MESSAGESMODEL_H
