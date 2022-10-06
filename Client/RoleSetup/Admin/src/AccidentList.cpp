#include "RoleSetup/Admin/AccidentList.hpp"
#include "ui_accidentlist.h"

AccidentList::AccidentList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccidentList)
{
    ui->setupUi(this);
}

AccidentList::~AccidentList()
{
    delete ui;
}
