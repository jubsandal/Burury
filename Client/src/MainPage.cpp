#include "MainPage.hpp"
#include "ui_MainPage.h"

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    QPixmap userLP = QPixmap(":login/cusomerLogin");
    userLP.scaled(64, 64);
    ui->userLogin->setIcon(QIcon(userLP));
    /* ui->outsiderRegister->setIcon(QIcon(employeeLP)); */

    connect(ui->userLogin, SIGNAL(clicked()), this, SIGNAL(loginClicked()));
    connect(ui->outsiderRegister, SIGNAL(clicked()), this, SIGNAL(registerCliecked()));
}

MainPage::~MainPage()
{
    delete ui;
}
