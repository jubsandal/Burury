#include "RoleSetup/Admin/AdminMain.hpp"
#include "ui_AdminMain.h"

AdminMain::AdminMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminMain)
{
    ui->setupUi(this);
}

AdminMain::~AdminMain()
{
    delete ui;
}
