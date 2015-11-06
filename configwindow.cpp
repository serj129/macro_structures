#include "configwindow.h"
#include "ui_configwindow.h"
#include "mainwindow.h"
#include "cellscene.h"

configWindow::configWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::configWindow)
{
    ui->setupUi(this);
		this->setWindowTitle("Settings");
		QObject::connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(saveConfig()));


		dbHost="localhost";
		dbName="dbcell";
		dbUser="root";
		dbPass="root";
		dbPort=3306;
		dbProvider="QMYSQL";
		drawDistrubution=true;
		drawHamiltonian=true;
		writeToFiles=true;
		x_count=15;
		y_count=15;

}

void configWindow::saveConfig ()
{
	dbHost=ui->lineEdit_6->text();
	dbName=ui->lineEdit_3->text();
	dbUser=ui->lineEdit_4->text();
	dbPass=ui->lineEdit_5->text();
	dbPort=ui->lineEdit_7->text().toInt();
	drawDistrubution=ui->checkBox->isChecked();
	drawHamiltonian=ui->checkBox_2->isChecked();
	writeToFiles=ui->checkBox_3->isChecked();
	x_count = ui->lineEdit_2->text().toInt();
	y_count = ui->lineEdit->text().toInt();

	this->hide();
}



configWindow::~configWindow()
{
    delete ui;
}
