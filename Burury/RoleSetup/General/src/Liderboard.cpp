#include "RoleSetup/General/Liderboard.hpp"
#include "ui_Liderboard.h"

Liderboard::Liderboard(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Liderboard)
{
        ui->setupUi(this);
}

Liderboard::~Liderboard()
{
        delete ui;
}
