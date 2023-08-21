#include "messagesmodel.h"

MessagesModel::MessagesModel(QObject *parent)
    : QAbstractListModel { parent }
{

}

MessagesModel::~MessagesModel()
{

}

int MessagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_conversation.count();
}

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_conversation.count())
        return QVariant();

    if (SenderRole == role)
        return m_conversation.at(index.row()).sender;
    else if (MessageRole == role)
        return m_conversation.at(index.row()).message;
    else if (OwnerRole == role)
        return m_conversation.at(index.row()).sendByThisDevice;
    else
        return QVariant();
}

void MessagesModel::addMessage(const QString &sender, const QString &message, bool sendByThisDevice)
{
    beginInsertRows(QModelIndex(), m_conversation.count(), m_conversation.count());
    m_conversation.append(MessageData(sender, message, sendByThisDevice));
    endInsertRows();
}

QHash<int, QByteArray> MessagesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SenderRole] = "sender";
    roles[MessageRole] = "message";
    roles[OwnerRole] = "owner";
    return roles;
}
