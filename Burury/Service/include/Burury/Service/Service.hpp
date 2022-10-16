#ifndef SERVICE_HPP_CBPTLJJQ
#define SERVICE_HPP_CBPTLJJQ

#include <QString>
#include <QHostAddress>
#include <QJsonObject>

#include "Network/SslClientBase.hpp"

class ResponseWaiter : public QObject {
Q_OBJECT

public:
        ResponseWaiter(QObject * p = nullptr);
        virtual ~ResponseWaiter();

public Q_SLOTS:
        void set_success(QJsonObject);
        void set_failed(int, QString);

Q_SIGNALS:
        void success(QJsonObject);
        void failed(int, QString);
};

class Service : public SslClientBase {
Q_OBJECT

public:
        Service(QObject * p = nullptr);
        virtual ~Service();

Q_SIGNALS:
        void loginSuccess(QString, int, int); // name, role, id
        void loginFailed(int, QString);

        void registerSuccess(int, int);
        void registerFailed(int, QString);

public Q_SLOTS:
        void login(const QString& login, const QString& password);
        void doregister(const QString& login,
                        const QString& password,
                        const QString& name,
                        const QString& email,
                        const QString& avatar_path,
                        int role);

        void sendCommand(int handler_id, QJsonObject payload, ResponseWaiter *, iiNPack::PacketType extra_pack_type = iiNPack::REQUEST);

private Q_SLOTS:
        void parseResonce(iiNPack::Header, QByteArray);
        void parseLoginResponce(iiNPack::Header, QByteArray);
        void parseRegisterResponce(iiNPack::Header, QByteArray);

        void on_sslError(const QList<QSslError>&);
        void on_networkError(QAbstractSocket::SocketError);

private:
        QMap<quint64, ResponseWaiter*> _waiters;
};

#endif /* end of include guard: SERVICE_HPP_CBPTLJJQ */
