#include "MainPage.hpp"
#include "ui_MainPage.h"

MainPage::MainPage(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::MainPage)
{
        ui->setupUi(this);
        QPixmap userLP = QPixmap(":logo/big-logo");
        userLP.scaled(128, 128);
        ui->icon->setPixmap(userLP);

        connect(ui->signin, SIGNAL(clicked()), this, SIGNAL(loginClicked()));
        connect(ui->signup, SIGNAL(clicked()), this, SIGNAL(registerCliecked()));
}

MainPage::~MainPage()
{
        delete ui;
}
