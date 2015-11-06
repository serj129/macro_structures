#include "struct3d.h"
#include "ui_struct3d.h"
#include <QSlider>
#include <QGraphicsView>

#include <QtSvg>
#include <QtGui>
#include <fstream>
#include <QColorDialog>

struct3d::struct3d(QWidget *parent) :
		AbstractStructure(parent),
    ui(new Ui::struct3d)
{

    ui->setupUi(this);
		setWindowTitle("Edit 3D structures");
		resize(800,600);

		view = new QGraphicsView;
		scene = new tethraScene;
		view->setMouseTracking(true);
		view->setScene(scene);

		xSlider = new QSlider(Qt::Horizontal);
		xSlider->setRange(-90, 90);
		xSlider->setSingleStep(1);
		xSlider->setPageStep(10);
		xSlider->setTickInterval(10);
		xSlider->setTickPosition(QSlider::TicksBelow);

		ySlider = new QSlider(Qt::Horizontal);
		ySlider->setRange(-180, 180);
		ySlider->setSingleStep(1);
		ySlider->setPageStep(10);
		ySlider->setTickInterval(10);
		ySlider->setTickPosition(QSlider::TicksBelow);


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
		ui->gridLayout->addWidget(ui->saveButton_2,9,0,1,1);
		ui->gridLayout->addWidget(ui->okButton,10,0,1,1);
		ui->gridLayout->addWidget(ui->cancelButton,11,0,1,1);
		ui->gridLayout->addWidget(ui->label,0,2,1,1);
		ui->gridLayout->addWidget(ui->lineEdit,0,3,1,1);
		ui->gridLayout->addWidget(ui->label_2,0,4,1,1);
		ui->gridLayout->addWidget(ui->lineEdit_2,0,5,1,1);
		ui->gridLayout->addWidget(view,1,1,10,5);
		ui->gridLayout->addWidget(xSlider,11,1,1,5);
		ui->gridLayout->addWidget(ySlider,12,1,1,5);

		this->setLayout(ui->gridLayout);

		connect(xSlider, SIGNAL(valueChanged(int)), scene, SLOT(rotateOX(int)));
		connect(ySlider, SIGNAL(valueChanged(int)), scene, SLOT(rotateOY(int)));
		QObject::connect(ui->cancelButton,SIGNAL(clicked()),this, SLOT(close()));
		QObject::connect(ui->zoomInButton,SIGNAL(clicked()),this,SLOT(zoomIn()));
		QObject::connect(ui->zoomOutButton,SIGNAL(clicked()),this,SLOT(zoomOut()));
		QObject::connect(ui->colorButton,SIGNAL(clicked()),this,SLOT(color()));
		QObject::connect(ui->clearButton,SIGNAL(clicked()),scene,SLOT(clear()));
		QObject::connect(ui->radioButton,SIGNAL(clicked()),scene,SLOT(setStruct1()));
		QObject::connect(ui->radioButton_2,SIGNAL(clicked()),scene,SLOT(setStruct2()));

		QObject::connect(ui->okButton,SIGNAL(clicked()),this,SLOT(hide()));
//		QObject::connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(save()));
                //QObject::connect(ui->saveButton,SIGNAL(clicked()),scene,SLOT(writePointsToFile()));
                QObject::connect(ui->saveButton,SIGNAL(clicked()),scene,SLOT(writeCoordinatesToFile()));
		QObject::connect(ui->saveButton_2,SIGNAL(clicked()),scene,SLOT(readPointsFromFile()));
		QObject::connect(scene,SIGNAL(checkedPointSelected(int)),this,SLOT(showOrder(int)));
}

struct3d::~struct3d()
{
    delete ui;
}


void struct3d::zoomIn() {
	view->scale(1.1,1.1);
}

void struct3d::zoomOut() {
	view->scale(1/1.1,1/1.1);
}

void struct3d::showOrder(int n) {
    ui->label->setText(QString("%1 A:").arg(n));
}

void struct3d::saveImage() {
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
void struct3d::save()
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

std::vector<std::vector<double> > & struct3d::giveSynapses()
{
return scene->giveSynapses(ui->comboBox->currentIndex(),ui->comboBox_2->currentIndex(),
									ui->comboBox_3->currentIndex(),ui->lineEdit->text().toDouble(),
									ui->lineEdit_2->text().toDouble());
}

void struct3d::color()
{
		 QColor col;
		 col = QColorDialog::getColor(Qt::green, this);
		 if (col.isValid())
		 scene->setColor(col);
}



