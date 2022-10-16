#include "RoleSetup/Admin/RoleManagment.hpp"
#include "ui_RoleManagment.h"

RoleManagment::RoleManagment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoleManagment)
{
    ui->setupUi(this);
}

RoleManagment::~RoleManagment()
{
    delete ui;
}
