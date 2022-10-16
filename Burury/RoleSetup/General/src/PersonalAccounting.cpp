#include "RoleSetup/General/PersonalAccounting.hpp"
#include "ui_PersonalAccounting.h"

PersonalAccounting::PersonalAccounting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonalAccounting)
{
    ui->setupUi(this);
}

PersonalAccounting::~PersonalAccounting()
{
    delete ui;
}
