#include <QtNetwork>

#include "client.h"
#include "connection.h"
#include "peermanager.h"

Client::Client(bool nochat)
{
    peerManager = new PeerManager(this);
    peerManager->setServerPort(server.serverPort());
    privacy=nochat;
    peerManager->startBroadcasting();

    QObject::connect(peerManager, SIGNAL(newConnection(Connection*)),
                     this, SLOT(newConnection(Connection*)));
    QObject::connect(&server, SIGNAL(newConnection(Connection*)),
                     this, SLOT(newConnection(Connection*)));
}

void Client::sendMessage(const QString &message)
{
    if (message.isEmpty())
        return;

    QList<Connection *> connections = peers.values();
    foreach (Connection *connection, connections)
        connection->sendMessage(message);
}

void Client::sendImage(const QByteArray &message)
{
    if (message.isEmpty())
        return;

    QList<Connection *> connections = peers.values();
    foreach (Connection *connection, connections)
        connection->sendImage(message);
}

void Client::sendFile(const QByteArray &message)
{
    if (message.isEmpty())
        return;

    QList<Connection *> connections = peers.values();
    foreach (Connection *connection, connections)
        connection->sendFile(message);
}

QString Client::nickName() const
{
    return QString(peerManager->userName()) + '@' + QHostInfo::localHostName()
           + ':' + QString::number(server.serverPort());
}

bool Client::hasConnection(const QHostAddress &senderIp, int senderPort) const
{
    if (senderPort == -1)
        return peers.contains(senderIp);

    if (!peers.contains(senderIp))
        return false;

    QList<Connection *> connections = peers.values(senderIp);
    foreach (Connection *connection, connections) {
        if (connection->peerPort() == senderPort)
            return true;
    }

    return false;
}

void Client::newConnection(Connection *connection)
{
    connection->setGreetingMessage(peerManager->userName());

    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
}

void Client::readyForUse()
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->peerAddress(),
                                     connection->peerPort()))
        return;

    bool canadd=true;
    if(privacy)
    {
        canadd=false;
        foreach(QHostAddress testip,iplist)
        {
            if(testip.isEqual(connection->peerAddress(),QHostAddress::ConvertV4MappedToIPv4))
            {
                canadd=true;
                break;
            }
        }
    }
    if(canadd)
    {
        connect(connection, SIGNAL(newMessage(QString,QString)),
                this, SIGNAL(newMessage(QString,QString)));
        connect(connection, SIGNAL(newImage(const QString&,QByteArray&)),
                this, SIGNAL(newImage(const QString&,QByteArray&)));
        connect(connection, SIGNAL(newFile(const QString&,QByteArray&)),
                this, SIGNAL(newFile(const QString&,QByteArray&)));
        peers.insert(connection->peerAddress(), connection);
        QString nick = connection->name();
        if (!nick.isEmpty())
            emit newParticipant(nick);
    }
}

void Client::disconnected()
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::connectionError(QAbstractSocket::SocketError /* socketError */)
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::removeConnection(Connection *connection)
{
    if (peers.contains(connection->peerAddress())) {
        peers.remove(connection->peerAddress());
        QString nick = connection->name();
        if (!nick.isEmpty())
            emit participantLeft(nick);
    }
    connection->deleteLater();
}

void Client::setPrivacy(bool flag)
{
    privacy=flag;
}

void Client::addList(const QHostAddress &address)
{
    iplist.append(address);
}
