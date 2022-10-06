#include "RoleSetup/General/Profile.hpp"
#include "ui_Profile.h"
#include <qnamespace.h>

#include "Database/Database.hpp"

#include <QMessageBox>

Profile::Profile(int userID, bool HideWapon, QWidget *parent) :
    QWidget(parent),
        ui(new Ui::Profile)
{
    ui->setupUi(this);
    if (HideWapon) {
        ui->waponValue->hide();
        ui->waponLabel->hide();
    }

    QPixmap pi = QPixmap(":icons/noProfileImage");
    ui->personaImage->setPixmap(pi.scaled(218, 218,
                                          Qt::AspectRatioMode::KeepAspectRatio,
                                          Qt::FastTransformation));
    _waiter = new ResponseWaiter(this);
    connect(_waiter, SIGNAL(success(QJsonObject)), this, SLOT(personInfoLoaded(QJsonObject)));
    connect(_waiter, SIGNAL(failed(int, QString)), this, SLOT(personInfoLoadFailed(int, QString)));
    QTimer::singleShot(100, [this, userID] {
        Q_EMIT loadPersonInfo(
            QJsonObject{{"command", Database::CMD_GET_USER_INFO},
                        {"arg", QJsonObject{{"id", userID}, {"takeImage", true}}}},
            _waiter);
    });
}

Profile::~Profile()
{
    delete _waiter;
    delete ui;
}

void
Profile::personInfoLoadFailed(int n, QString errstr)
{
    QMessageBox errmsg(this);
    errmsg.setIcon(QMessageBox::Critical);
    errmsg.setText( "Cannot load profile info");
    errmsg.setInformativeText(
            "Error code: " + QString::number(n) + "\n"
            "Reason: " + errstr);
    errmsg.setDefaultButton(QMessageBox::Ok);
    errmsg.exec();
}

void
Profile::personInfoLoaded(QJsonObject data)
{
    ui->entryDate->setDate(
        QDateTime::fromSecsSinceEpoch(data["entryDate"].toInteger()).date());
    ui->personaName->setText(data["name"].toString());
    if (data["image"].isString()) {
        if (data["image"].toString() != QString()) {
            ui->personaImage->setPixmap(
                QPixmapFromQString(data["image"].toString())
                    .scaled(218, 218, Qt::AspectRatioMode::KeepAspectRatio,
                            Qt::FastTransformation));
        }
    }
    ui->roleValue->setText(QString::number(data["role"].toInt()));
    ui->loginValue->setText(data["login"].toString());
    if (!ui->waponValue->isHidden()) {
        ui->waponValue->setText(QString::number(data["wapon"].toInt()));
    }
}
