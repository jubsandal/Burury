#ifndef IICLIENT_HPP_JINRZSDB
#define IICLIENT_HPP_JINRZSDB

#include "API/API.hpp"
#include "Network/SslClientBase.hpp"

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
        void identificationFailed(API::CmdError);

        void registred(QJsonObject);
        void registerFailed(API::CmdError);

        void addCommand(API::DriverCmd);

    private Q_SLOTS:
        void on_identified(QJsonObject);
        void on_identificationFailed(API::CmdError err);

        void on_registred(QJsonObject);
        void on_registerFailed(API::CmdError err);

        void on_requestSuccess(QJsonObject);
        void on_requestFailed(API::CmdError);

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
