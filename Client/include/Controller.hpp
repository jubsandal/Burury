#ifndef QT_SSL_EXAMPLE_CLIENT_Controller_H__
#define QT_SSL_EXAMPLE_CLIENT_Controller_H__

#include <QMainWindow>
#include <QWidget>
#include <QPointer>
#include <QPushButton>
#include <QTextEdit>
#include <QThread>

#include "Client/Service/Service.hpp"

#include "Widgets/Notifier/NotifyManager.hpp"
#include "Widgets/ClickableLabel.hpp"
#include "PagesManager/PagesManager.hpp"
#include "Logger/logger.hpp"
#include "Network/iiNPack.hpp"

#include "Widgets/Notifier/NotifyProgressItem.hpp"
#include "Widgets/Notifier/NotifyProgressItemFactory.hpp"
#include "Widgets/Notifier/NotifyMsgItem.hpp"
#include "Widgets/Notifier/NotifyMsgItemFactory.hpp"

#include "Login.hpp"
#include "Register.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Controller; }
QT_END_NAMESPACE

struct Settings {
    QString certPath = "./cert.pem";
    QString caPath = "./ca.pem";
    QString keyPath = "./key.pem";

    int logginLeve = LoggingLevel::Trace;
    QString logDir = "./log";

    QString authFile = "./.auth";

    int maxThreads = QThread::idealThreadCount();
};

class Controller : public QMainWindow {
    Q_OBJECT

    public:
        Controller(const Settings&, QWidget *parent = NULL);

        int userId() const;

        virtual ~Controller();

    Q_SIGNALS:
        void send_to_log(QString message, int level);
        void createNotifyItem(NotifyItemFactory*, int&);
        void setNotifyItemPropery(int, const QByteArray&, const QVariant&);
        void resized(QResizeEvent*);

    public Q_SLOTS:
        void on_actionLogoutTriggered();

        void tryLogin(ConnectSettings);
        void tryRegister(RegisterOpts);

        void saveAuth();
        void readAuth();

    private Q_SLOTS:
        void showLogin();
        void showRegister();

        void on_conneted();
        void on_logined(QString name, int role, int id);
        void on_loginFailed(int, QString);
        void on_logouted();

        void on_registred(int id, int role);
        void on_registerFailed(int, QString);

        void logMessage(QString _message, int);

        void resizeEvent(QResizeEvent *);

    protected:
        QThread _loggingThread;
        QThread _serviceThread;
        logger  _log;
        Service _service;

        bool _conn_policy_login = true;
            // the crutch for register and login quering

        Settings _settings;

        PagesManager * _pageman;
        NotifyManager * _notifier;

        ConnectSettings _conn_s;
        RegisterOpts _reg_opts;
        int _login_puid;

        int _user_id;

        Ui::Controller * ui;
};

#endif // QT_SSL_EXAMPLE_CLIENT_Controller_H__
