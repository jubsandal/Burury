#ifndef SSLCLIENTBASE_HPP_NQAPT2TY
#define SSLCLIENTBASE_HPP_NQAPT2TY

#include "Network/iiNPack.hpp"
#include <QSslKey>
#include <QSslSocket>
#include <QSharedPointer>
#include <QPointer>
#include <QIODevice>
#include <QAbstractSocket>

// Big QSslSocket wrapper
class SslClientBase : public QObject {
    Q_OBJECT

    public:
        SslClientBase(QSslSocket * socket, QObject * parent = nullptr);
        virtual ~SslClientBase();

        void create();

        void disconnectFromHost();

        bool         isValid() const;
        QHostAddress localAddress() const;
        quint16      localPort() const;
        QHostAddress peerAddress() const;
        QString      peerName() const;
        quint16      peerPort() const;

        void connectToHost(
            const QString &hostName, quint16 port,
            QIODeviceBase::OpenMode openMode = QAbstractSocket::ReadWrite,
            QAbstractSocket::NetworkLayerProtocol protocol =
                QAbstractSocket::AnyIPProtocol);
        void connectToHost(
            const QHostAddress &address, quint16 port,
            QIODeviceBase::OpenMode openMode = QAbstractSocket::ReadWrite);

        void connectToHostEncrypted(
            const QString &hostName, quint16 port,
            QIODeviceBase::OpenMode mode = QAbstractSocket::ReadWrite,
            QAbstractSocket::NetworkLayerProtocol protocol =
                QAbstractSocket::AnyIPProtocol);
        void connectToHostEncrypted(
            const QString &hostName, quint16 port, const QString &sslPeerName,
            QIODeviceBase::OpenMode mode = QAbstractSocket::ReadWrite,
            QAbstractSocket::NetworkLayerProtocol protocol =
                QAbstractSocket::AnyIPProtocol);

        qint64  bytesAvailable() const;
        qint64  bytesToWrite() const;

        bool waitForBytesWritten(int msecs = 30000);
        bool waitForConnected(int msecs = 30000);
        /* bool waitForDisconnected(int msecs = 30000); */
        /* bool waitForReadyRead(int msecs = 30000); */
        bool waitForEncrypted(int msecs = 30000);

        bool isEncrypted() const;

        void setLocalCertificate(const QSslCertificate &certificate);
        void setLocalCertificate(const QString &path, QSsl::EncodingFormat format = QSsl::Pem);
        void setLocalCertificateChain(const QList<QSslCertificate> &localChain);
        void setPeerVerifyDepth(int depth);
        void setPeerVerifyMode(QSslSocket::PeerVerifyMode mode);
        void setPeerVerifyName(const QString &hostName);
        void setPrivateKey(const QSslKey &key);
        void setPrivateKey(const QString &fileName,
                           QSsl::KeyAlgorithm algorithm = QSsl::Rsa,
                           QSsl::EncodingFormat format = QSsl::Pem,
                           const QByteArray &passPhrase = QByteArray());
        void setProtocol(QSsl::SslProtocol protocol);
        void setSslConfiguration(const QSslConfiguration &configuration);

        void ignoreSslErrors();
        void startClientEncryption();
        void startServerEncryption();

    Q_SIGNALS:
        void recivedMessage(iiNPack::Header, QByteArray _data);

        void connected();
        void disconnected();
        void encrypted();

        void peerVerifyError(const QSslError &);
        void sslErrors(const QList<QSslError>&);
        void errorOccurred(QAbstractSocket::SocketError);
        void proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *);
        void stateChanged(QAbstractSocket::SocketState);

        void logMessage(QString, int);

    public Q_SLOTS:
        virtual void sendMessage(QByteArray);

    protected Q_SLOTS:
        virtual void dataAvailable();
        virtual void socketStateChanged(QAbstractSocket::SocketState);

        virtual void onSocketError(QAbstractSocket::SocketError);
        virtual void onSslErrors(const QList<QSslError>&);

    protected:
        virtual void sendData(const QByteArray& header, const QByteArray& load);

        iiNPack::Header _unfinishedMessageHeader;
        QByteArray _buffer;

        QSslSocket * _control;
};

#endif /* end of include guard: SSLCLIENTBASE_HPP_NQAPT2TY */
