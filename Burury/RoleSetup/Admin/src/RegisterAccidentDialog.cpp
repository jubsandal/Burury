#include "RoleSetup/Admin/RegisterAccidentDialog.hpp"
#include "ui_RegisterAccidentDialog.h"

RegisterAccidentDialog::RegisterAccidentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterAccidentDialog)
{
    ui->setupUi(this);
}

RegisterAccidentDialog::~RegisterAccidentDialog()
{
    delete ui;
}
