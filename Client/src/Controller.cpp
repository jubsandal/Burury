#include "Controller.hpp"
#include "./ui_Controller.h"

#include "PagerPresets.hpp"

#include <QMessageBox>
#include <QErrorMessage>

#include <QTimer>
#include <qnamespace.h>
#include <qssl.h>

        Controller::Controller(const Settings &settings, QWidget *_parent)
    : QMainWindow(_parent),
      ui(new Ui::Controller),
      _settings(settings),
      _log(settings.logginLeve, "siisty-Client", settings.logDir, nullptr),
      _pageman(new PagesManager(this)) {
    ui->setupUi(this);

    connect(ui->actionLogout, SIGNAL(triggered()), this, SLOT(on_actionLogoutTriggered()));

    {
        _notifier = new NotifyManager(
                this,
                QMargins(0, 0, 2, ui->statusbar->size().height() + 2),
                {120, 40},
                -1,
                NotifyManager::StackAbove);
        connect(this, SIGNAL(resized(QResizeEvent*)), _notifier, SIGNAL(windowResized(QResizeEvent*)));
        connect(this,
                SIGNAL(createNotifyItem(NotifyItemFactory *, int&)),
                _notifier,
                SLOT(createNotifyItem(NotifyItemFactory *, int&)));
        connect(this,
                SIGNAL(setNotifyItemPropery(int, const QByteArray &, const QVariant &)),
                _notifier,
                SLOT(setItemPropery(int, const QByteArray &, const QVariant &)));
    }

    {
        connect(this, SIGNAL(send_to_log(QString, int)), &_log, SLOT(logMessage(QString, int)));
        _log.moveToThread(&_loggingThread);
    }

    {
        connect(&_service, SIGNAL(logMessage(QString, int)), this, SLOT(logMessage(QString, int)));
    }
    {
        if (!QFile(_settings.authFile).exists()) {
            auto af = QFile(_settings.authFile);
            if (!af.open(QIODevice::ReadWrite)) {
                auto nmf = NotifyMsgItemFactory();
                nmf.setCompleteTimeout(5000);
                nmf.setTitle("Cannot create auth file");
                nmf.setMsg("No write permissions"); // TODO
                int dummy;
                _notifier->createNotifyItem(&nmf, dummy);
            } else {
                af.setPermissions(QFile::Permission::WriteOwner | QFile::Permission::ReadOwner);
                    // work on windows?
            }
        }
    }

    {
        _pageman = new PagesManager(this);
        this->centralWidget()->layout()->addWidget(_pageman);
    }

    _loggingThread.start();

    // add check if saved auth data
    pagerPresets[MAIN_PAGE_ID](this, _pageman, &_service);
}

Controller::~Controller()
{
    _serviceThread.quit();
    _loggingThread.quit();

    _serviceThread.wait();
    _loggingThread.wait();
}

int Controller::userId() const { return _user_id; }

void
Controller::resizeEvent(QResizeEvent * e)
{
    e->accept();
    Q_EMIT resized(e);
}

void
Controller::showLogin()
{
    readAuth();
    Login * login = new Login(_conn_s, this);

    _conn_policy_login = true;

    connect(login, SIGNAL(tryLogin(ConnectSettings)), this, SLOT(tryLogin(ConnectSettings)), Qt::SingleShotConnection);

    login->show();
    if (login->exec()) {

    }
    delete login;
}

void
Controller::showRegister()
{
    Register * reg = new Register(this);

    _conn_policy_login = false;

    connect(reg, SIGNAL(tryRegister(RegisterOpts)), this, SLOT(tryRegister(RegisterOpts)), Qt::SingleShotConnection);

    reg->show();
    if (reg->exec()) {

    }
    delete reg;
}

void
Controller::tryLogin(ConnectSettings cs)
{
    NotifyProgressItemFactory npf;
    npf.setTitle("Connecting to host");
    npf.setExitOnCompleted(false);
    npf.setMaximum(0);
    _notifier->createNotifyItem(&npf, _login_puid);
    _notifier->setItemPropery(_login_puid, "progress", 0);

    _conn_s = cs;
    saveAuth();

    connect(&_service, SIGNAL(loginFailed(int, QString)),
            this, SLOT(on_loginFailed(int, QString)),
            Qt::SingleShotConnection);

    if (cs.protocol > 0) { // QSsl::Protocol::NoProtocol
        connect(&_service, SIGNAL(encrypted()), this, SLOT(on_conneted()), Qt::SingleShotConnection);
        _service.connectToHostEncrypted(cs.host.toString(), cs.port);
    } else {
        connect(&_service, SIGNAL(connected()), this, SLOT(on_conneted()), Qt::SingleShotConnection);
        _service.connectToHost(cs.host, cs.port);
    }
    if (!_service.waitForConnected()) {
        on_loginFailed(-1, "Connection Timeout");
    }
}

// TODO merge with tryLogin
void
Controller::tryRegister(RegisterOpts opts)
{
    NotifyProgressItemFactory npf;
    npf.setTitle("Connecting to host");
    npf.setExitOnCompleted(false);
    npf.setMaximum(0);
    _notifier->createNotifyItem(&npf, _login_puid);
    _notifier->setItemPropery(_login_puid, "progress", 0);

    connect(&_service, SIGNAL(registerFailed(int, QString)),
            this, SLOT(on_registerFailed(int, QString)),
            Qt::SingleShotConnection);

    _conn_s.login = opts.login;
    _conn_s.password = opts.password;
    _conn_s.host = QHostAddress(opts.conn.host);
    _conn_s.port = opts.conn.port;
    _conn_s.protocol = opts.conn.encryption;

    _reg_opts = opts;

    if (opts.conn.encryption > 0) { // QSsl::Protocol::NoProtocol
        connect(&_service, SIGNAL(encrypted()), this, SLOT(on_conneted()), Qt::SingleShotConnection);
        _service.connectToHostEncrypted(opts.conn.host, opts.conn.port);
    } else {
        connect(&_service, SIGNAL(connected()), this, SLOT(on_conneted()), Qt::SingleShotConnection);
        _service.connectToHost(opts.conn.host, opts.conn.port);
    }
    if (!_service.waitForConnected()) {
        on_registerFailed(-1, "Connection Timeout");
    }
}

void
Controller::on_conneted()
{
    if (_conn_policy_login) {
        _notifier->setItemPropery(_login_puid, "title", "Trying sing up");
        connect(&_service, SIGNAL(loginSuccess(QString, int, int)), this,
                SLOT(on_logined(QString, int, int)), Qt::SingleShotConnection);
        _service.login(_conn_s.login, _conn_s.password);
    } else {
        _notifier->setItemPropery(_login_puid, "title", "Trying register");
        connect(&_service, SIGNAL(registerSuccess(int, int)), this,
                SLOT(on_registred(int, int)), Qt::SingleShotConnection);
        _service.doregister(_reg_opts.login, _reg_opts.password, _reg_opts.name, _reg_opts.email, _reg_opts.avatar_path, _reg_opts.role);
    }
}

void
Controller::on_logined(QString name, int id, int role)
{
    _notifier->setItemPropery(_login_puid, "title", "Logined");
    QTimer::singleShot(2000, [this]() {
        _notifier->setItemPropery(_login_puid, "forceComplete", NotifyItem::NotifySuccess);
    });
    ui->actionLogin->setEnabled(false);
    ui->actionLogout->setEnabled(true);

    _user_id = id;

    qDebug() << role;
    pagerPresets[role](this, _pageman, &_service);
}

void
Controller::on_loginFailed(int err, QString str)
{
    Q_EMIT logMessage(
        "Login failed. Error code: " + QString::number(err) + " Reason: " + str, Error);
    _notifier->setItemPropery(_login_puid, "title", "Login failed");
    _notifier->setItemPropery(_login_puid, "maxProgress", 1);
    _notifier->setItemPropery(_login_puid, "progress", 1);
    QTimer::singleShot(7000, [this]() {
        _notifier->setItemPropery(_login_puid, "forseComplete", NotifyItem::NotifySuccess);
    });
    ui->actionLogin->setEnabled(true);
    ui->actionLogout->setEnabled(false);

    QMessageBox errmsg(this);
    errmsg.setIcon(QMessageBox::Critical);
    errmsg.setText( "Cannot Loging");
    errmsg.setInformativeText(
            "Error code: " + QString::number(err) + "\n"
            "Reason: " + str);
    errmsg.setDefaultButton(QMessageBox::Ok);
    errmsg.exec();
}

void
Controller::on_registred(int id, int role)
{
    _notifier->setItemPropery(_login_puid, "title", "Registred");
    QTimer::singleShot(2000, [this]() {
        _notifier->setItemPropery(_login_puid, "forceComplete", NotifyItem::NotifySuccess);
    });
    ui->actionLogin->setEnabled(false);
    ui->actionLogout->setEnabled(true);

    _user_id = id;

    pagerPresets[role](this, _pageman, &_service);
}

void
Controller::on_registerFailed(int code, QString desc)
{
    Q_EMIT logMessage(
        "Registration failed. Error code: " + QString::number(code) + " Reason: " + desc, Error);
    _notifier->setItemPropery(_login_puid, "title", "Registraction failed");
    _notifier->setItemPropery(_login_puid, "maxProgress", 1);
    _notifier->setItemPropery(_login_puid, "progress", 1);
    QTimer::singleShot(2000, [this]() {
        _notifier->setItemPropery(_login_puid, "forseComplete", NotifyItem::NotifySuccess);
    });
    ui->actionLogin->setEnabled(true);
    ui->actionLogout->setEnabled(false);

    QMessageBox errmsg(this);
    errmsg.setIcon(QMessageBox::Critical);
    errmsg.setText( "Cannot Loging");
    errmsg.setInformativeText(
            "Error code: " + QString::number(code) + "\n"
            "Reason: " + desc);
    errmsg.setDefaultButton(QMessageBox::Ok);
    errmsg.exec();
}

void
Controller::on_logouted()
{
    ui->actionLogin->setEnabled(true);
    ui->actionLogout->setEnabled(false);

    pagerPresets[MAIN_PAGE_ID](this, _pageman, &_service);
}

void
Controller::on_actionLogoutTriggered()
{
    _service.disconnectFromHost();
    // update ui
    pagerPresets[MAIN_PAGE_ID](this, _pageman, &_service);
}

void
Controller::logMessage(QString _message, int level)
{
    qDebug() << _message;

    Q_EMIT send_to_log(_message, level);
}

void
Controller::saveAuth()
{
    // TODO add optional save pass
    QFile authf(_settings.authFile);
    if (!authf.open(QIODevice::WriteOnly)) {
        int msg_pid;
        NotifyMsgItemFactory nmf;
        nmf.setTitle("Cannot save auth");
        nmf.setMsg("No write permission");
        nmf.setCompleteTimeout(3000);
        _notifier->createNotifyItem(&nmf, msg_pid);
        return;
    }
    authf.setPermissions(QFile::Permissions::enum_type::ReadOwner | QFile::Permissions::enum_type::WriteOwner);
    QJsonDocument d(QJsonObject{
        { "login", (_conn_s.rememberLogin ? _conn_s.login : "") },
        { "password", (_conn_s.rememberPassword ? _conn_s.password : "") },
        { "host", _conn_s.host.toString() },
        { "port", (int)_conn_s.port },
        { "encryption", (int)_conn_s.protocol },
        { "rememberLogin", _conn_s.rememberLogin },
        { "rememberPassword", _conn_s.rememberPassword }
    });
    authf.write(d.toJson());
}

void
Controller::readAuth()
{
    QFile authf(_settings.authFile);
    if (!authf.open(QIODevice::ReadOnly)) {
        int msg_pid;
        NotifyMsgItemFactory nmf;
        nmf.setTitle("Cannot read auth");
        nmf.setMsg("No read permission");
        nmf.setCompleteTimeout(3000);
        _notifier->createNotifyItem(&nmf, msg_pid);
        return;
    }
    QJsonParseError err;
    QJsonDocument d = QJsonDocument::fromJson(authf.readAll(), &err);

    if (err.error != QJsonParseError::NoError || d.isNull() || !d.isObject()) {
        _conn_s.host = QHostAddress("127.0.0.1");
        _conn_s.port = 2142;
        _conn_s.login = "";
        _conn_s.password = "";
        _conn_s.protocol = (int)QSsl::UnknownProtocol;
        _conn_s.rememberLogin = true;
        _conn_s.rememberPassword = false;
        return;
    }

    QJsonObject obj = d.object();
    _conn_s.host = QHostAddress(obj["host"].toString());
    _conn_s.port = obj["port"].toInteger();
    _conn_s.protocol = obj["encryption"].toInt();
    _conn_s.login = obj["login"].toString();
    _conn_s.password = obj["password"].toString();
    _conn_s.rememberLogin = obj["rememberLogin"].toBool();
    _conn_s.rememberPassword = obj["rememberPassword"].toBool();
}
