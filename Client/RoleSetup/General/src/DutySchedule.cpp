#include "RoleSetup/General/DutySchedule.hpp"
#include "ui_DutySchedule.h"

DutySchedule::DutySchedule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DutySchedule)
{
    ui->setupUi(this);
}

DutySchedule::~DutySchedule()
{
    delete ui;
}
