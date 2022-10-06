#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QThread>
#include <QScopedPointer>

#include "Database/Database.hpp"
#include "Database/Role.hpp"

#include "Logger/logger.hpp"
#include "iiServer/iiServer.hpp"

struct Settings {
        bool useSsl = false;

        QString certPath = "./cert.pem";
        QString caPath = "./ca.pem";
        QString keyPath = "./key.pem";

        QHostAddress serveAddress = QHostAddress::AnyIPv4;
        quint16 servePort = 2142;

        QString databasePath = "./database.db";

        int logginLeve = LoggingLevel::Trace;
        QString logDir = "./log";

        QString defultPage = "Welcome";

        int maxThreads = QThread::idealThreadCount();
        int maxPendingConnections = 100;
};

class Server : public QObject
{
        Q_OBJECT

        public:
                Server(Settings settings, QWidget *parent = nullptr);

                virtual ~Server();

        Q_SIGNALS:
                void resized(QResizeEvent*);
                void createNotifyItem(NotifyItemFactory *, int&);
                void setNotifyItemPropery(int uid, const QByteArray& name, const QVariant& value);
                void send_to_log(QString, int);

        public Q_SLOTS:
                void changeServeAddress(QHostAddress&, quint16);
                void changeLoggingLeve(int level);

        private Q_SLOTS:
                void setupServer();

                void on_listeningStateChanged(QHostAddress, quint16, bool);
                void on_actionQuit_triggered();

                void on_databaseError(Database::CmdError);
                // command exec error
                void on_databaseInited();
                void on_databaseInitializationFailed(QSqlError e);

                void logMessage(QString, int = LoggingLevel::Trace);

                virtual void resizeEvent(QResizeEvent*);

        private:
                Settings _settings;

                QThread _databaseThread;
                QThread _loggingThread;
                QThread _serverThread;
                iiServer _server;
                logger   _log;
                Database::Driver _database;
};
#endif // SERVERMANAGER_H
