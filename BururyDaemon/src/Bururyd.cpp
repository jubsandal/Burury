#include <QException>
#include <qnamespace.h>
#include <QErrorMessage>

#include "Bururyd.hpp"
#include "API/Role.hpp"

BururyDaemon::BururyDaemon(Settings settings, QWidget *parent)
        : QObject(parent),
        _settings(settings),
        _log(_settings.logginLeve, "bururyd", _settings.logDir, nullptr),
        _server(nullptr),
        _apiDriver(_settings.databasePath)
{
        {
                connect(this, SIGNAL(send_to_log(QString, int)), &_log, SLOT(logMessage(QString, int)), Qt::DirectConnection);
                _log.moveToThread(&_loggingThread);
        }

        {
                connect(&_apiDriver, SIGNAL(logMessage(QString, int)), this, SLOT(logMessage(QString, int)), Qt::DirectConnection);

                connect(&_apiDriver, SIGNAL(Inited()),
                        this, SLOT(on_apiDriverInited()), Qt::QueuedConnection);
                connect(&_apiDriver, SIGNAL(Inited()),
                        this, SLOT(setupServer()), Qt::QueuedConnection);
                connect(&_apiDriver, SIGNAL(InitizlizationFailed(QSqlError)),
                        this, SLOT(on_apiDriverInitializationFailed(QSqlError)));

                connect(&_apiDriverThread, SIGNAL(started()), &_apiDriver, SLOT(Run()), Qt::DirectConnection);
                _apiDriver.moveToThread(&_apiDriverThread);
        }

        _loggingThread.start();
        _apiDriverThread.start();

        _loggingThread.setObjectName("Logging thread");
        _serverThread.setObjectName("HTTPS BururyDaemon thread");
        _apiDriverThread.setObjectName("API driver thread");
}

BururyDaemon::~BururyDaemon()
{
        _apiDriver.Stop();

        _apiDriverThread.quit();
        _loggingThread.quit();
        // mb crash?
        _serverThread.quit();

        _apiDriverThread.wait();
        _loggingThread.wait();
        // mb crash?
        _serverThread.wait();
}

void BururyDaemon::setupServer()
{
        _server.setForseUseSsl(_settings.useSsl);
        _server.LoadCertificates(_settings.certPath, _settings.keyPath);

        connect(&_server, SIGNAL(logMessage(QString, int)), this, SLOT(logMessage(QString, int)), Qt::DirectConnection);

        connect(&_server, SIGNAL(listeningStateChanged(QHostAddress, quint16, bool)),
                this, SLOT(on_listeningStateChanged(QHostAddress, quint16, bool)));

        int cmd = qRegisterMetaType<API::DriverCmd>();
        connect(&_server, SIGNAL(addCommand(API::DriverCmd)),
                &_apiDriver, SLOT(addCommand(API::DriverCmd)), Qt::QueuedConnection);
        this->_server.ToggleStartStopListening(this->_settings.serveAddress, this->_settings.servePort);
        Q_EMIT logMessage("Now server listening");
}

void BururyDaemon::changeServeAddress(QHostAddress& add, quint16 port)
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

void BururyDaemon::changeLoggingLeve(int level)
{
        if (_log.loggingLevel() != level) {
                Q_EMIT logMessage("Logging level changed to: " + QString::number(level), Fatal);
                _log.setLoggingLevel(level);
        }
}

void BururyDaemon::on_listeningStateChanged(QHostAddress addr, quint16 port, bool isListening) {
        Q_EMIT logMessage(isListening ? QString("BururyDaemon online") : QString("BururyDaemon offline"), LoggingLevel::Info);
}


void BururyDaemon::on_actionQuit_triggered() { QCoreApplication::quit(); }

void BururyDaemon::on_apiDriverError(API::HndlError err)
{
        Q_EMIT logMessage(err.String(), LoggingLevel::Error);
}

void BururyDaemon::on_apiDriverInited() {
        API::DriverAssistant * waiter = new API::DriverAssistant(this);
        _apiDriver.addCommand({
                                      .handler_id = API::HNDL::Config::GetUserInfo,
                                      .caller_role_id = API::Roles::SYS,
                                      .payload = { },
                                      .waiter = waiter
                              });

        connect(waiter, &API::DriverAssistant::success, this, [=](QJsonObject res){
                        auto jusers = res["users"].toArray();

                        bool admin_ex = false;
                        for (auto v : jusers) {
                                if (v.toObject()["role"] == API::Roles::Admin) {
                                        admin_ex = true;
                                        break;
                                }
                        }

                        if (!admin_ex) {
                                _apiDriver.addCommand({
                                                              .handler_id = API::HNDL::Config::CreateUser,
                                                              .caller_role_id = API::Roles::SYS,
                                                              .payload = {
                                                                      { "name", _settings.defaultUser.name },
                                                                      { "login", _settings.defaultUser.login },
                                                                      { "password", _settings.defaultUser.password },
                                                                      { "role", API::Roles::Admin }
                                                              },
                                                              .waiter = waiter
                                                      });

                                connect(waiter, &API::DriverAssistant::success, this, [=](QJsonObject) { delete waiter; });
                                connect(waiter, &API::DriverAssistant::failed, this, [=](API::HndlError err) {
                                                qDebug() << "Cannot create default user: " << err.String();
                                                delete waiter;
                                        });
                        } else {
                                delete waiter;
                        }
                });
}

void BururyDaemon::on_apiDriverInitializationFailed(QSqlError e)
{
        QString error = "API driver initialization error: " + e.text() +
                " Driver reason: " + e.driverText();
        logMessage(error, LoggingLevel::Fatal);
        QErrorMessage msg = QErrorMessage();
        msg.showMessage("Cannot initialize api driver!\nTerminating");
        this->on_actionQuit_triggered();
}

void BururyDaemon::logMessage(QString str, int level)
{
        Q_EMIT send_to_log(str, level);
}
