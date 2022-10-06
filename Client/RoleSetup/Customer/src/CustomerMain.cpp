#include "RoleSetup/Customer/CustomerMain.hpp"
#include "ui_CustomerMain.h"

CustomerMain::CustomerMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerMain)
{
    ui->setupUi(this);
}

CustomerMain::~CustomerMain()
{
    delete ui;
}
