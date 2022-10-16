#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QThread>
#include <QScopedPointer>

#include "API/Driver.hpp"
#include "API/HndlError.hpp"

#include "Logger/logger.hpp"
#include "iiServer/iiServer.hpp"

struct Settings {
        bool useSsl = false;

        QString certPath = "./cert.pem";
        QString caPath   = "./ca.pem";
        QString keyPath  = "./key.pem";

        QHostAddress serveAddress = QHostAddress::AnyIPv4;
        quint16 servePort = 2142;

        QString databasePath = "./.bururyd.db";

        int logginLeve = LoggingLevel::Trace;
        QString logDir = "./log";

        int maxThreads = QThread::idealThreadCount();
        int maxPendingConnections = 100;

        struct DefaultUser {
                QString name = "Admin";
                QString login = "admin";
                QString password = "admin";
        } defaultUser;
};

class BururyDaemon : public QObject
{
        Q_OBJECT

        public:
                BururyDaemon(Settings settings, QWidget *parent = nullptr);

                virtual ~BururyDaemon();

        Q_SIGNALS:
                // void createNotifyItem(NotifyItemFactory *, int&);
                // void setNotifyItemPropery(int uid, const QByteArray& name, const QVariant& value);
                void send_to_log(QString, int);

        public Q_SLOTS:
                void changeServeAddress(QHostAddress&, quint16);
                void changeLoggingLeve(int level);

        private Q_SLOTS:
                void setupServer();

                void on_listeningStateChanged(QHostAddress, quint16, bool);
                void on_actionQuit_triggered();

                void on_apiDriverError(API::HndlError);
                // command exec error
                void on_apiDriverInited();
                void on_apiDriverInitializationFailed(QSqlError e);

                void logMessage(QString, int = LoggingLevel::Trace);

        private:
                Settings _settings;

                QThread _apiDriverThread;
                QThread _loggingThread;
                QThread _serverThread;
                iiServer _server;
                logger _log;
                API::Driver _apiDriver;
};
#endif // SERVERMANAGER_H
