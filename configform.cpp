#include "configform.h"
#include "ui_configform.h"

configform::configform(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::configform)
{
    ui->setupUi(this);
}

configform::~configform()
{
    delete ui;
}
