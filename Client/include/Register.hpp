#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <QDialog>
#include <QJsonObject>
#include <QMimeDatabase>
#include <QFileDialog>
#include <QSsl>

namespace Ui {
    class Register;
}

struct RegisterOpts {
    struct connection {
        QString host;
        quint16 port;
        int encryption;
    };
    connection conn;
    int role;
    QString name;
    QString email;
    QString login;
    QString password;
    QString avatar_path;
};

class Register : public QDialog
{
    Q_OBJECT

    public:
        explicit Register(QWidget *parent = nullptr);
        ~Register();

    Q_SIGNALS:
        void tryRegister(RegisterOpts);

        void imgFileNotValid();
        void imgFileValid();

    private Q_SLOTS:
        void on_registerClicked();

        void openImgFileChooer();

        void on_imgFilePathValid();
        void on_imgFilePathNotValid();

        void validateImgFilePath(QString file);

    private:
        Ui::Register *ui;
        QFileDialog * _fileChooser;
};

#endif // REGISTER_HPP
