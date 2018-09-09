#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QAbstractSocket>
#include <QHash>
#include <QHashIterator>
#include <QHostAddress>

#include "server.h"

class PeerManager;

class Client:public QObject
{
    Q_OBJECT

public:
    Client(bool nochat=true);

    void sendMessage(const QString &message);
    void sendImage(const QByteArray &message);
    void sendFile(const QByteArray &message);
    QString nickName() const;
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;
    void setPrivacy(bool flag);
    void addList(const QHostAddress &address);

signals:
    void newMessage(const QString &from, const QString &message);
    void newImage(const QString&, QByteArray&);
    void newFile(const QString&, QByteArray&);
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);

private slots:
    void newConnection(Connection *connection);
    void connectionError(QAbstractSocket::SocketError socketError);
    void disconnected();
    void readyForUse();

private:
    void removeConnection(Connection *connection);

    PeerManager *peerManager;
    Server server;
    QMultiHash<QHostAddress, Connection *> peers;
    QList<QHostAddress> iplist;
    bool privacy;
};

#endif // CLIENT_H
