#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <QWidget>
#include <QDialog>
#include <QHostAddress>
#include <QSsl>

namespace Ui {
class Login;
}

struct ConnectSettings {
    QHostAddress host;
    quint16 port;
    int protocol;
    QString login;
    QString password;
    bool rememberLogin;
    bool rememberPassword;
};

class Login : public QDialog
{
    Q_OBJECT

    public:
        explicit Login(ConnectSettings cs, QWidget *parent = nullptr);
        ~Login();

    Q_SIGNALS:
        void tryLogin(ConnectSettings);

    private Q_SLOTS:
        void on_submit_clicked();
        void on_auth_changed(QString);

    private:
        Ui::Login *ui;
};

#endif // LOGIN_HPP
