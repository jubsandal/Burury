#include "RoleSetup/Admin/ObjectTypeAdministration.hpp"
#include "ui_ObjectTypeAdministration.h"

ObjectTypeAdministration::ObjectTypeAdministration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectTypeAdministration)
{
    ui->setupUi(this);
}

ObjectTypeAdministration::~ObjectTypeAdministration()
{
    delete ui;
}
