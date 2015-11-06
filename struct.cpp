#include "struct.h"
#include "ui_struct.h"
#include <QtSvg>
#include <QtGui>
#include <fstream>
#include "cellscene.h"
#include <QColorDialog>



structure::structure(QWidget *parent) :
		AbstractStructure(parent),
	 ui(new Ui::structure)
{
    ui->setupUi(this);
	 setWindowTitle("Edit structures");
	 scene = new CellScene;
	 view = new QGraphicsView;
	 view->setScene(scene);
	 ui->comboBox->setCurrentIndex(3);
	 ui->lineEdit->setText("-1");
	 ui->gridLayout->addWidget(ui->groupBox,0,0,2,1);
	 ui->gridLayout->addWidget(ui->comboBox,2,0,1,1);
	 ui->gridLayout->addWidget(ui->comboBox_2,3,0,1,1);
	 ui->gridLayout->addWidget(ui->comboBox_3,4,0,1,1);
	 ui->gridLayout->addWidget(ui->clearButton,5,0,1,1);
	 ui->gridLayout->addWidget(ui->zoomInButton,6,0,1,1);
	 ui->gridLayout->addWidget(ui->zoomOutButton,7,0,1,1);
	 ui->gridLayout->addWidget(ui->saveButton,8,0,1,1);
	 ui->gridLayout->addWidget(ui->okButton,9,0,1,1);
	 ui->gridLayout->addWidget(ui->cancelButton,10,0,1,1);
	 ui->gridLayout->addWidget(ui->label,0,2,1,1);
	 ui->gridLayout->addWidget(ui->lineEdit,0,3,1,1);
	 ui->gridLayout->addWidget(ui->label_2,0,4,1,1);
	 ui->gridLayout->addWidget(ui->lineEdit_2,0,5,1,1);
	 ui->gridLayout->addWidget(view,1,1,10,5);

	 this->setLayout(ui->gridLayout);

	 QObject::connect(ui->cancelButton,SIGNAL(clicked()),this, SLOT(close()));
	 QObject::connect(ui->zoomInButton,SIGNAL(clicked()),this,SLOT(zoomIn()));
	 QObject::connect(ui->zoomOutButton,SIGNAL(clicked()),this,SLOT(zoomOut()));
	 QObject::connect(ui->colorButton,SIGNAL(clicked()),this,SLOT(color()));
	 QObject::connect(ui->clearButton,SIGNAL(clicked()),scene,SLOT(clear()));
	 QObject::connect(ui->radioButton,SIGNAL(toggled(bool)),scene,SLOT(setStruct1(bool)));
	 QObject::connect(ui->radioButton_2,SIGNAL(toggled(bool)),scene,SLOT(setStruct2(bool)));

	 QObject::connect(ui->okButton,SIGNAL(clicked()),this,SLOT(hide()));
	 QObject::connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(save()));
}

unsigned structure::giveNumber()
{
return scene->giveNumber();
}

void structure::color()
{
		 QColor col;
		 col = QColorDialog::getColor(Qt::green, this);
		 if (col.isValid())
		 scene->setColor(col);
}

std::vector<std::vector<double> > & structure::giveSynapses()
{
return scene->giveSynapses(ui->comboBox->currentIndex(),ui->comboBox_2->currentIndex(),
									ui->comboBox_3->currentIndex(),ui->lineEdit->text().toDouble(),
									ui->lineEdit_2->text().toDouble());
}

void structure::zoomIn() {
	view->scale(1.1,1.1);
}

void structure::zoomOut() {
	view->scale(1/1.1,1/1.1);
}

void structure::saveImage() {
//	view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
//	QPixmap pixmap(view->size());
//	QPainter painter(&pixmap);
//	painter.setRenderHint(QPainter::Antialiasing);
//	view->render(&painter);
//	pixmap.save(QApplication::applicationDirPath() + "/image.png", "PNG",100);
	QString fileName = QFileDialog::getSaveFileName((QWidget *) this,
										 tr("Save as:"),
										 "structure.svg",
										 tr("SVG files (*.svg);;All Files (*)"));

	QRectF rect = scene->sceneRect();
	QSvgGenerator generator;
	generator.setFileName(fileName);
	generator.setSize(QSize(scene->sceneRect().width(), scene->sceneRect().height()));
	generator.setViewBox(scene->sceneRect());
	generator.setTitle(tr("Title"));
	generator.setDescription(tr("Description"));
	QPainter painter;
	painter.begin(&generator);
	scene->render(&painter, QRectF(), rect);
	painter.end();

}
void structure::save()
{

	std::ofstream out;
	QString fileName = QFileDialog::getSaveFileName((QWidget *) this,
										 tr("Save as:"),
										 "synapses.txt",
										 tr("Text Files (*.txt);;All Files (*)"));
	out.open(fileName.toAscii(), std::ios::out);
	out<<std::scientific<<std::noshowpos;
	std::vector<std::vector <double> > points = giveSynapses();
	for (unsigned i=0;i<points.size();i++) {
		for(unsigned j=0;j<points.size();j++)
			if (i==j) out<<"1.000000e+000\t";
			else out<<points[i][j]<<'\t';
			if (i<points.size()-1) out<<std::endl;
		}
	out.close();
}



structure::~structure()
{
    delete ui;
}
