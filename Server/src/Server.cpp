#include "Server.hpp"

#include <QException>

#include <qnamespace.h>

#include <QErrorMessage>

#include "Database/Database.hpp"

Server::Server(Settings settings, QWidget *parent)
        : QObject(parent)
          _settings(settings),
          _log(_settings.logginLeve, "siisty-server", _settings.logDir, nullptr),
          _server(nullptr),
          _database(_settings.databasePath)
{
        {
                connect(this, SIGNAL(send_to_log(QString, int)), &_log, SLOT(logMessage(QString, int)), Qt::DirectConnection);
                _log.moveToThread(&_loggingThread);
        }

        {
                connect(&_database, SIGNAL(logMessage(QString, int)), this, SLOT(logMessage(QString, int)), Qt::DirectConnection);

                connect(&_database, SIGNAL(Inited()),
                                this, SLOT(on_databaseInited()), Qt::QueuedConnection);
                connect(&_database, SIGNAL(Inited()),
                                this, SLOT(setupServer()), Qt::QueuedConnection);
                connect(&_database, SIGNAL(InitizlizationFailed(QSqlError)),
                                this, SLOT(on_databaseInitializationFailed(QSqlError)));

                connect(&_databaseThread, SIGNAL(started()), &_database, SLOT(Run()), Qt::DirectConnection);
                _database.moveToThread(&_databaseThread);
        }

        _loggingThread.start();
        _databaseThread.start();

        _loggingThread.setObjectName("Logging thread");
        _serverThread.setObjectName("Service thread");
        _databaseThread.setObjectName("Database driver thread");
}

Server::~Server()
{
        _database.Stop();

        _databaseThread.quit();
        _loggingThread.quit();
        /* _serverThread.quit(); */

        _databaseThread.wait();
        _loggingThread.wait();
        /* _serverThread.wait(); */

        delete _notifier;
        delete _listenIndicator;
        delete ui;
}

void Server::setupServer()
{
        _server.setForseUseSsl(_settings.useSsl);
        _server.LoadCertificates(_settings.certPath, _settings.keyPath);

        connect(&_server, SIGNAL(logMessage(QString, int)), this, SLOT(logMessage(QString, int)), Qt::DirectConnection);

        connect(&_server, SIGNAL(listeningStateChanged(QHostAddress, quint16, bool)),
                        this, SLOT(changeIndicatorState(QHostAddress, quint16, bool)));
        connect(&_server, SIGNAL(listeningStateChanged(QHostAddress, quint16, bool)),
                        this, SLOT(on_listeningStateChanged(QHostAddress, quint16, bool)));

        int cmd = qRegisterMetaType<Database::DatabaseCmd>();
        connect(&_server, SIGNAL(addCommand(Database::DatabaseCmd)),
                        &_database, SLOT(addCommand(Database::DatabaseCmd)), Qt::QueuedConnection);
}

void Server::changeServeAddress(QHostAddress& add, quint16 port)
{
        if (_settings.serveAddress != add || _settings.servePort != port ) {
                _settings.serveAddress = add;
                _settings.servePort = port;

                if (_server.isListening()) {

                        Q_EMIT logMessage("Rebooting server after change Serve address and port", Debug);
                        _server.ToggleStartStopListening(add, port);
                        _server.ToggleStartStopListening(add, port);
                }
        }
}

void Server::changeLoggingLeve(int level)
{
        if (_log.loggingLevel() != level) {
                Q_EMIT logMessage("Logging level changed to: " + QString::number(level), Fatal);
                _log.setLoggingLevel(level);
        }
}

void Server::changeIndicatorState(QHostAddress address, quint16 port, bool listening)
{
        if (listening) {
                _listenIndicator->setLedState(QLedIndicator::LedState::Active);
        } else {
                _listenIndicator->setLedState(QLedIndicator::LedState::Inactive);
        }
}

void Server::resizeEvent(QResizeEvent * e)
{
        e->accept();
        Q_EMIT resized(e);
}

void Server::on_actionQuit_triggered() { QApplication::quit(); }

void Server::on_databaseError(Database::CmdError err)
{
        Q_EMIT logMessage(err.String(), LoggingLevel::Error);
}

void Server::on_databaseInited()
{
        ui->actionToggle_server->setEnabled(true);
        ui->statusbar->clearMessage();
}

void Server::on_databaseInitializationFailed(QSqlError e)
{
        ui->statusbar->showMessage("Database initialization error", 5*1000);
        QString error = "Database initialization error: " + e.text() +
                "\nDriver reason: " + e.driverText();
        logMessage(error, LoggingLevel::Fatal);
        QErrorMessage msg = QErrorMessage();
        msg.showMessage("Cannot initialize database!\nTerminating");
        this->on_actionQuit_triggered();
}

void Server::logMessage(QString str, int level)
{
        Q_EMIT send_to_log(str, level);
}
