#include <cassert>
#include <QDateTime>
#include <QFileInfo>
#include <qnamespace.h>

#include "Logger/logger.hpp"
#include "Network/iiNPack.hpp"
#include "iiServer/iiServer.hpp"

iiServer::iiServer(QObject * parent)
    : QObject(parent),
        _threadPool(new QThreadPool(this))
{
    if (!QSslSocket::supportsSsl() && _forseUseSsl) {
        Q_EMIT logMessage("Missing SSL. Please install it.", Fatal);
        throw "Missing SSL.";
    }

    _threadPool->setMaxThreadCount(QThread::idealThreadCount());

    connect(&_server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

iiServer::~iiServer()
{
    if (_server.isListening()) {
        _server.close();
    }

    delete _threadPool;

    _clients.clear();
}

int  iiServer::maxPendingConnections() const     { return _server.maxPendingConnections(); }
void iiServer::setMaxPendingConnections(int max) { _server.setMaxPendingConnections(max); }
bool iiServer::isListening() const               { return _server.isListening(); }

void
iiServer::setForseUseSsl(bool use)
{
    if (_forseUseSsl != use) {
        Q_EMIT logMessage(tr((use ? "Enabling %1" : "Disabling %1")).arg("forse use SSL"), Debug);
        _forseUseSsl = use;
        if (_server.isListening()) {
            Q_EMIT logMessage("Rebooting server", Debug);
            ToggleStartStopListening(_address, _port);
        }
    }
}

void iiServer::ToggleStartStopListening(const QHostAddress &address,
                                        quint16 port)
{
    _address = address;
    _port = port;

    if (_server.isListening()) {
        _server.close();
        Q_EMIT listeningStateChanged(address, port, false);
        Q_EMIT logMessage("Server shutdowned", LoggingLevel::Debug);
        return;
    }

    if (_server.listen(address, port)) {
        Q_EMIT logMessage("Start serving on: " + address.toString() + ":" + QString::number(port), Debug);
        Q_EMIT listeningStateChanged(address, port, true);
    } else {
        Q_EMIT logMessage("Could not bind " + _server.errorString(), Fatal);
    }
}

void iiServer::LoadCertificates(const QString& certPath, const QString& keyPath)
{
    _key = keyPath;
    _certificate = certPath;

    QFileInfo keyInfo(keyPath);
    QFileInfo certificateInfo(certPath);

    if (keyInfo.exists() == false) {
        Q_EMIT logMessage("Key file does not exist " + keyPath, Error);
        return;
    }
    if (keyInfo.isReadable() == false) {
        Q_EMIT logMessage("Key file is not readable " + keyPath, Error);
        return;
    }
    if (certificateInfo.exists() == false) {
        Q_EMIT logMessage("certificate file does not exist " + certPath, Error);
        return;
    }
    if (certificateInfo.isReadable() == false) {
        Q_EMIT logMessage("certificate file is not readable " + certPath, Error);
        return;
    }
}

// Accept connection from server and initiate the SSL handshake
void iiServer::acceptConnection()
{
    QSslSocket *socket =
        reinterpret_cast<QSslSocket *>(_server.nextPendingConnection());
    assert(socket);

    iiClient * client(new iiClient(socket));

    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    connect(client, SIGNAL(recivedMessage(iiNPack::Header, QByteArray)),
            this, SLOT(recivedMessage(iiNPack::Header, QByteArray)));

    connect(client, SIGNAL(logMessage(QString, int)),
            this, SIGNAL(logMessage(QString, int)));

    connect(client, SIGNAL(addCommand(API::DriverCmd)),
            this, SIGNAL(addCommand(API::DriverCmd)),
            Qt::QueuedConnection);

    if (QSslSocket::supportsSsl() && _forseUseSsl)
    {
        connect(client, SIGNAL(encrypted()), this, SLOT(handshakeComplete()));
        client->setPrivateKey(_key);
        client->setLocalCertificate(_certificate);

        client->setPeerVerifyMode(QSslSocket::VerifyNone);
        client->startServerEncryption();
    }
    else
    {
        _clients.append(client);

        Q_EMIT logMessage("Accepted connection from "
                  + client->peerAddress().toString() + ":"
                  + QString::number(client->peerPort())
                  + " .Encrypted : " + (client->isEncrypted() ? "yes" : "no"), Debug);
    }
}

// Receive notification that the SSL handshake has completed successfully
void
iiServer::handshakeComplete()
{
    iiClient * client = reinterpret_cast<iiClient *>(sender());
    assert(client);

    Q_EMIT logMessage("Accepted connection from "
              + client->peerAddress().toString() + ":"
              + QString::number(client->peerPort())
              + " .Encrypted : " + (client->isEncrypted() ? "yes" : "no"), Debug);

    _clients.append(client);
}

// unpack, identify message, and send to next node in process chain
void
iiServer::recivedMessage(iiNPack::Header header, QByteArray msg)
{
    iiClient * client = dynamic_cast<iiClient *>(sender());
    assert(client);

    Q_EMIT logMessage("Recived message " + msg, Debug);

    _threadPool->start([this, client, header, msg]() {
        //check send stamp?
        //add experation time?

        if (header.PacketLoadType != iiNPack::JSON) {
            // dont supported msg
            Q_EMIT logMessage("Now implemented only json message format", Error);
            // send it to client
            return;
        }

        QByteArray responce;
        QJsonParseError err;
        QJsonObject load = QJsonDocument::fromJson(msg, &err).object();

        switch (header.PacketType) {
            case iiNPack::AUTORIZATION_REQUEST:
                {
                    Q_EMIT logMessage("Recived an autorization request", Trace);
                    if (client->identified()) {
                        Q_EMIT logMessage("Resived double time identification request", Warning);
                        responce = iiNPack::packError(
                            "Double time identification request",
                            iiNPack::REQUEST_ERROR,
                            QDateTime::currentMSecsSinceEpoch(), header.ClientStamp);
                        client->sendMessage(responce);
                        return;
                    }

                    if (load["login"].isString() && load["password"].isString()) {
                        client->identify(load["login"].toString(), load["password"].toString());
                    } else {
                        Q_EMIT logMessage("Null param identification request recived", Warning);
                        // send err msg
                    }
                }
                break;
            case iiNPack::REGISTRATION_REQUEST:
                {
                    Q_EMIT logMessage("Recived an reqistraction request", Trace);
                    if (client->identified()) {
                        Q_EMIT logMessage("Recived registraction request from autorized client", Warning);
                        responce = iiNPack::packError("Your already registred", iiNPack::REQUEST_ERROR,
                            QDateTime::currentMSecsSinceEpoch(), header.ClientStamp);
                        client->sendMessage(responce);
                    } else {
                        client->doregister(load);
                    }
                    break;
                }
            case iiNPack::REQUEST: // request from remote
                {
                    Q_EMIT logMessage("Recived an request", Trace);
                    if (!client->identified()) {
                        responce = iiNPack::packError(
                            "Access denied", iiNPack::REQUEST_ERROR,
                            QDateTime::currentMSecsSinceEpoch(), header.ClientStamp);
                        client->sendMessage(responce);
                    } else {
                        client->processRequest(load, header.ClientStamp);
                    }
                }
                break;
            /* case iiNPack::RESPONSE: // responce from remote */
            /*     if (!client->identified()) { */
            /*         responce = iiNPack::packError("Access denied", iiNPack::REQUEST_ERROR); */
            /*         client->sendMessage(responce); */
            /*         return; */
            /*     } */
            /*     client->processResponce(load); */
            /*     break; */
            default:
                Q_EMIT logMessage("Unknown packet: " + QString::number(header.PacketType), Error);
                break;
        }
    });
}

void
iiServer::clientDisconnected()
{
    iiClient *client = dynamic_cast<iiClient *>(sender());
    assert(client);

    Q_EMIT logMessage("Client disconnect: " + client->peerAddress().toString(), Debug);
    _clients.removeOne(client); //mem lack?
}
