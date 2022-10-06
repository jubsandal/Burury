#include "Network/SslClientBase.hpp"
#include "Network/iiNPack.hpp"
#include "Logger/logger.hpp"

SslClientBase::SslClientBase(QSslSocket * socket, QObject * parent)
    : QObject(parent),
        _control(socket)
{
    connect(_control, SIGNAL(readyRead()), this, SLOT(dataAvailable()));

    connect(_control, SIGNAL(connected()),    this, SIGNAL(connected()));
    connect(_control, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(_control, SIGNAL(encrypted()),    this, SIGNAL(encrypted()));

    connect(_control, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(_control, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SIGNAL(stateChanged(QAbstractSocket::SocketState)));

    connect(_control, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(_control, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            this, SIGNAL(errorOccurred(QAbstractSocket::SocketError)));

    connect(_control, SIGNAL(sslErrors(const QList<QSslError>&)),
            this, SLOT(onSslErrors(const QList<QSslError>&)));
    connect(_control, SIGNAL(sslErrors(const QList<QSslError>&)),
            this, SIGNAL(sslErrors(const QList<QSslError>&)));

    connect(_control, SIGNAL(peerVerifyError(const QSslError &)), this, SIGNAL(peerVerifyError(const QSslError&)));
    connect(_control, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),
            this, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));

    _unfinishedMessageHeader.Size = 0;
}

SslClientBase::~SslClientBase()
{
    if (_control) {
        delete _control;
    }
}

bool         SslClientBase::isValid() const      { return _control->isValid(); }
QHostAddress SslClientBase::localAddress() const { return _control->localAddress(); }
quint16      SslClientBase::localPort() const    { return _control->localPort(); }
QHostAddress SslClientBase::peerAddress() const  { return _control->peerAddress(); }
QString      SslClientBase::peerName() const     { return _control->peerName(); }
quint16      SslClientBase::peerPort() const     { return _control->peerPort(); }

void SslClientBase::disconnectFromHost() { _control->disconnectFromHost(); }

void SslClientBase::connectToHost(
    const QString &hostName, quint16 port,
    QIODeviceBase::OpenMode openMode,
    QAbstractSocket::NetworkLayerProtocol protocol)
{
    _control->connectToHost(hostName, port, openMode, protocol);
}

void SslClientBase::connectToHost(
    const QHostAddress &address, quint16 port,
    QIODeviceBase::OpenMode openMode)
{
    _control->connectToHost(address, port, openMode);
}

void SslClientBase::connectToHostEncrypted(
    const QString &hostName, quint16 port,
    QIODeviceBase::OpenMode mode,
    QAbstractSocket::NetworkLayerProtocol protocol)
{
    _control->connectToHostEncrypted(hostName, port, mode, protocol);
}

void SslClientBase::connectToHostEncrypted(
    const QString &hostName, quint16 port, const QString &sslPeerName,
    QIODeviceBase::OpenMode mode,
    QAbstractSocket::NetworkLayerProtocol protocol)
{
    _control->connectToHostEncrypted(hostName, port, sslPeerName, mode, protocol);
}

qint64  SslClientBase::bytesAvailable() const { return _control->bytesAvailable(); }
qint64  SslClientBase::bytesToWrite() const   { return _control->bytesToWrite(); }

bool SslClientBase::waitForBytesWritten(int msecs) { return _control->waitForBytesWritten(msecs); }
bool SslClientBase::waitForConnected(int msecs) { return _control->waitForConnected(msecs); }
/* bool SslClientBase::waitForDisconnected(int msecs = 30000) */
/* bool SslClientBase::waitForReadyRead(int msecs = 30000) */
bool SslClientBase::waitForEncrypted(int msecs) { return _control->waitForEncrypted(msecs); }

bool SslClientBase::isEncrypted() const { return _control->isEncrypted(); }

void SslClientBase::setLocalCertificate(const QSslCertificate &certificate)               { _control->setLocalCertificate(certificate); }
void SslClientBase::setLocalCertificate(const QString &path, QSsl::EncodingFormat format) { _control->setLocalCertificate(path, format); }
void SslClientBase::setLocalCertificateChain(const QList<QSslCertificate> &localChain)    { _control->setLocalCertificateChain(localChain); }
void SslClientBase::setPeerVerifyDepth(int depth)                                         { _control->setPeerVerifyDepth(depth); }
void SslClientBase::setPeerVerifyMode(QSslSocket::PeerVerifyMode mode)                    { _control->setPeerVerifyMode(mode); }
void SslClientBase::setPeerVerifyName(const QString &hostName)                            { _control->setPeerVerifyName(hostName); }
void SslClientBase::setPrivateKey(const QSslKey &key)                                     { _control->setPrivateKey(key); }
void SslClientBase::setPrivateKey(const QString &fileName,
                   QSsl::KeyAlgorithm algorithm,
                   QSsl::EncodingFormat format,
                   const QByteArray &passPhrase)                                          { _control->setPrivateKey(fileName, algorithm, format, passPhrase); }
void SslClientBase::setProtocol(QSsl::SslProtocol protocol)                               { _control->setProtocol(protocol); }
void SslClientBase::setSslConfiguration(const QSslConfiguration &configuration)           { _control->setSslConfiguration(configuration); }
void SslClientBase::ignoreSslErrors()                                                     { _control->ignoreSslErrors(); }
void SslClientBase::startClientEncryption()                                               { _control->startClientEncryption(); }
void SslClientBase::startServerEncryption()                                               { _control->startServerEncryption(); }

void
SslClientBase::sendMessage(QByteArray data)
{
    Q_EMIT logMessage("Sending data...", Debug);

    QDataStream io(&data, QIODevice::ReadOnly);

    QByteArray header(iiNPack::HeaderSize, 0);
    QByteArray load(data.size() - header.size(), 0);

    io.device()->seek(0);
    io.readRawData(header.data(), iiNPack::HeaderSize);
    io.device()->seek(header.size());
    io.readRawData(load.data(), data.size() - header.size());

    sendData(header, load);
}

void
SslClientBase::sendData(const QByteArray& _messageHeader, const QByteArray& _data)
{
    qint64 header_write = _control->write(_messageHeader);

    if (header_write == _messageHeader.length()) {
        qint64 data_write = _control->write(_data);

    } else {
        Q_EMIT logMessage(
            tr("DEBUG: Failed to write message header. Only wrote %1 bytes")
                .arg(header_write), Debug);
    }
}

void
SslClientBase::dataAvailable()
{
    Q_EMIT logMessage("Recived some data...", Debug);

    while (this->bytesAvailable() > 0) {
        _buffer.append(_control->readAll());

        if (_unfinishedMessageHeader.Size == 0) {
            QDataStream io(&_buffer, QIODevice::ReadOnly);
            io >>
                _unfinishedMessageHeader.Size >>
                _unfinishedMessageHeader.ServerStamp >>
                _unfinishedMessageHeader.ClientStamp >>
                _unfinishedMessageHeader.PacketType >>
                _unfinishedMessageHeader.PacketLoadType;
        }

        if (_buffer.length() >= _unfinishedMessageHeader.Size) {
            Q_EMIT logMessage(tr("DEBUG: Recived Message"), Debug);
            Q_EMIT recivedMessage(
                _unfinishedMessageHeader,
                _buffer.mid(iiNPack::HeaderSize, _unfinishedMessageHeader.Size -
                                                     iiNPack::HeaderSize));
            _unfinishedMessageHeader.Size = 0;
            _buffer.clear();
        }
    }
}

void
SslClientBase::socketStateChanged(QAbstractSocket::SocketState _state)
{
    switch (_state) {
        case QAbstractSocket::UnconnectedState:
            Q_EMIT logMessage("Socket Disconnected", Debug);
            break;

        case QAbstractSocket::HostLookupState:
            Q_EMIT logMessage("Looking up host...", Debug);
            break;

        case QAbstractSocket::ConnectingState:
            Q_EMIT logMessage("Connecting socket...", Debug);
            break;

        case QAbstractSocket::ConnectedState:
            Q_EMIT logMessage("Socket Connected", Debug);
            break;

        case QAbstractSocket::BoundState:
            Q_EMIT logMessage("Socket Bound (Server)", Debug);
            break;

        case QAbstractSocket::ClosingState:
            Q_EMIT logMessage("Closing Socket", Debug);
            break;

        case QAbstractSocket::ListeningState:
            Q_EMIT logMessage("Socket Listening for sockets...", Debug);
            break;

        default:
            Q_EMIT logMessage(QString("Unknown Socket State: %1").arg(_state), Debug);
            break;
    };
}

void
SslClientBase::onSslErrors(const QList<QSslError>& errors)
{
    for (auto iter = errors.begin(); iter != errors.end(); ++iter) {
        Q_EMIT logMessage(QString("SSL ERROR: %1").arg(iter->errorString()), Error);
    }
}

void
SslClientBase::onSocketError(QAbstractSocket::SocketError _error)
{
    switch (_error) {
        case QAbstractSocket::ConnectionRefusedError:
            Q_EMIT logMessage(
                "ERROR: _socket Refused by peer or time-out occurred", Error);
            break;
        case QAbstractSocket::RemoteHostClosedError:
            Q_EMIT logMessage("ERROR: Remote Host closed the _socket", Error);
            break;
        case QAbstractSocket::HostNotFoundError:
            Q_EMIT logMessage("ERROR: Remote Host Not Found", Error);
            break;
        case QAbstractSocket::SocketAccessError:
            Q_EMIT logMessage(
                "ERROR: Socket Operation because application lacked "
                "priviledges", Error);
            break;
        case QAbstractSocket::SocketResourceError:
            Q_EMIT logMessage("ERROR: Local system is out of resources", Error);
            break;
        case QAbstractSocket::SocketTimeoutError:
            Q_EMIT logMessage("ERROR: Socket Timed out", Error);
            break;
        case QAbstractSocket::DatagramTooLargeError:
            Q_EMIT logMessage(
                "ERROR: Datagram was larger than the system allowed", Error);
            break;
        case QAbstractSocket::NetworkError:
            Q_EMIT logMessage(
                "ERROR: Network Error occurred. Cable plugged in?", Error);
            break;
        case QAbstractSocket::AddressInUseError:
            Q_EMIT logMessage("ERROR: Address Already In Use", Error);
            break;
        case QAbstractSocket::SocketAddressNotAvailableError:
            Q_EMIT logMessage(
                "ERROR: Specified Address does not belong to the host", Error);
            break;
        case QAbstractSocket::UnsupportedSocketOperationError:
            Q_EMIT logMessage(
                "ERROR: Socket operation not supported by host system", Error);
            break;
        case QAbstractSocket::ProxyAuthenticationRequiredError:
            Q_EMIT logMessage(
                "ERROR: Intermediate Proxy requires authentication", Error);
            break;
        case QAbstractSocket::SslHandshakeFailedError:
            Q_EMIT logMessage("ERROR: SSL/TLS handshake failed.", Error);
            break;
        case QAbstractSocket::UnfinishedSocketOperationError:
            Q_EMIT logMessage(
                "ERROR: Last operation in progress has not yet completed", Error);
            break;
        case QAbstractSocket::ProxyConnectionRefusedError:
            Q_EMIT logMessage(
                "ERROR: Could not contact the intermediate Proxy Server", Error);
            break;
        case QAbstractSocket::ProxyConnectionClosedError:
            Q_EMIT logMessage(
                "ERROR: Proxy _socket was unexpectedly closed", Error);
            break;
        case QAbstractSocket::ProxyConnectionTimeoutError:
            Q_EMIT logMessage("ERROR: Proxy _socket timed-out", Error);
            break;
        case QAbstractSocket::ProxyNotFoundError:
            Q_EMIT logMessage("ERROR: Proxy address was not found", Error);
            break;
        case QAbstractSocket::ProxyProtocolError:
            Q_EMIT logMessage(
                "ERROR: Message from intermediate Proxy Server could not be "
                "understood", Error);
            break;
        case QAbstractSocket::OperationError:
            Q_EMIT logMessage(
                "ERROR: Operation attempted while not in a valid state for "
                "that operation", Error);
            break;
        case QAbstractSocket::SslInternalError:
            Q_EMIT logMessage(
                "ERROR: SSL library reported an internal error. Check SSL "
                "library installation.", Error);
            break;
        case QAbstractSocket::SslInvalidUserDataError:
            Q_EMIT logMessage(
                "ERROR: Invalid data provided to SSL library that resulted in "
                "an error. Check configuration.", Error);
            break;
        case QAbstractSocket::TemporaryError:
            Q_EMIT logMessage(
                "ERROR: Temporary Error Occurred. Please try again", Error);
            break;
        case QAbstractSocket::UnknownSocketError:
            Q_EMIT logMessage("ERROR: Unknown Socket Error occurred", Error);
            break;
        default:
            Q_EMIT logMessage(
                QString("ERROR: Unknown Error (%1) occurred").arg(_error), Error);
            break;
    };
}
