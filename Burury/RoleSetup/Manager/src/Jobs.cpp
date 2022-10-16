#include "Jobs.h"
#include "ui_Jobs.h"

Jobs::Jobs(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Jobs)
{
        ui->setupUi(this);
}

Jobs::~Jobs()
{
        delete ui;
}
