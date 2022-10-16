#include "RoleSetup/Admin/Accidents.hpp"
#include "ui_Accidents.h"

Accidents::Accidents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Accidents)
{
    ui->setupUi(this);
}

Accidents::~Accidents()
{
    delete ui;
}
