#include "Register.hpp"
#include "ui_Register.h"
#include <Database/Role.hpp>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    connect(this->ui->reg, SIGNAL(clicked()), this, SLOT(on_registerClicked()));

    connect(ui->avatar_btn, SIGNAL(clicked()), this, SLOT(openImgFileChooer()));

    connect(ui->avatar_path, SIGNAL(textChanged(QString)), this, SLOT(validateImgFilePath(QString)));

    connect(this, SIGNAL(imgFileValid()), this, SLOT(on_imgFilePathValid()));
    connect(this, SIGNAL(imgFileNotValid()), this, SLOT(on_imgFilePathNotValid()));

    { // icon stup
        ui->logo_3->setPixmap(QIcon(":logo/big-logo").pixmap(QSize(128, 128)));
    }

    ui->encryptionType_3->addItem("Any", QSsl::AnyProtocol);
    ui->encryptionType_3->addItem("Disable", QSsl::UnknownProtocol);
    ui->encryptionType_3->addItem("TLSv1.2", QSsl::TlsV1_2);
    ui->encryptionType_3->addItem("TLSv1.3", QSsl::TlsV1_3);
}

Register::~Register()
{
    delete ui;
}

void
Register::on_registerClicked()
{
    RegisterOpts opts;
    opts.conn.host = this->ui->host_3->text();
    opts.conn.port = this->ui->port_3->text().toUInt();
    opts.conn.encryption = this->ui->encryptionType_3->currentData().toInt();
    opts.role = Database::RoleId::ROLE_Customer;
    opts.name = this->ui->name_le->text();
    opts.email = this->ui->email_le->text();
    opts.login = this->ui->login_le->text();
    opts.password = this->ui->password_le->text();
    opts.avatar_path = this->ui->avatar_path->text();
    Q_EMIT tryRegister(opts);
}

void
Register::validateImgFilePath(QString file)
{
    if (file.length()) {
        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(file);

        QRegularExpression r("(image/)");
        if (!QFile(file).exists() ||
                !r.match(mime.name()).hasMatch())
        {
            Q_EMIT imgFileNotValid();
            return;
        }
    }
    Q_EMIT imgFileValid();
}

void Register::on_imgFilePathValid() { ui->avatar_path->setStyleSheet(""); }
void Register::on_imgFilePathNotValid() { ui->avatar_path->setStyleSheet("color: #ba3939; background: #ffe0e0; border: 1px solid #a33a3a;"); }

void
Register::openImgFileChooer()
{
    _fileChooser = new QFileDialog(this,
            tr("Open image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));
    _fileChooser->setFileMode(QFileDialog::FileMode::ExistingFile);
    _fileChooser->show();
    if (_fileChooser->exec() == QDialog::Accepted) {
        ui->avatar_path->setText(_fileChooser->selectedFiles()[0]);
    }
}
