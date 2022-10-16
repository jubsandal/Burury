#ifndef IICLIENT_HPP_JINRZSDB
#define IICLIENT_HPP_JINRZSDB

#include "Network/SslClientBase.hpp"
#include "API/Driver.hpp"
#include "API/HndlError.hpp"

#include <QMutex>
#include <QJsonObject>

class iiClient : public SslClientBase {
Q_OBJECT

public:
        iiClient(QSslSocket * socket, QObject* parent = nullptr);
        virtual ~iiClient();

        bool identified() const;

public Q_SLOTS:
        void doregister(QJsonObject);
        void identify(QString login, QString password);
        void processRequest(QJsonObject, qint64);

Q_SIGNALS:
        void identified(QJsonObject);
        void identificationFailed(API::HndlError);

        void registred(QJsonObject);
        void registerFailed(API::HndlError);

        void addCommand(API::DriverCmd);

private Q_SLOTS:
        void on_identified(QJsonObject);
        void on_identificationFailed(API::HndlError err);

        void on_registred(QJsonObject);
        void on_registerFailed(API::HndlError err);

        void on_requestSuccess(QJsonObject);
        void on_requestFailed(API::HndlError);

private:
        API::DriverAssistant * _dbAssistant;

        QMutex  _mtx;
        qint64  _curStamp;

        int     _max_tries = 10;
        int     _ident_tries = 0;
        bool    _identified = false;

        int _role = -1000; //invalid role
        QString _login;
        QString _password;
        // in advance to security managment
        // this value drops after succeccfuly identification
};

#endif /* end of include guard: IICLIENT_HPP_JINRZSDB */
