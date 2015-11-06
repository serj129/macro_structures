#include "widget.h"
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QSlider>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
	view = new QGraphicsView;
	scene = new tethraScene;
	layout = new QVBoxLayout;
	view->setScene(scene);
	layout->addWidget(view);

	xSlider = new QSlider(Qt::Horizontal);
	xSlider->setRange(-90, 90);
	xSlider->setSingleStep(1);
	xSlider->setPageStep(10);
	xSlider->setTickInterval(10);
	xSlider->setTickPosition(QSlider::TicksBelow);

	ySlider = createSlider();

	layout->addWidget(xSlider);
	layout->addWidget(ySlider);

	this->setLayout(layout);
	connect(xSlider, SIGNAL(valueChanged(int)), scene, SLOT(rotateOX(int)));
	connect(ySlider, SIGNAL(valueChanged(int)), scene, SLOT(rotateOY(int)));
	this->resize(QSize(500,600));
}

QSlider *Widget::createSlider()
{
		QSlider *slider = new QSlider(Qt::Horizontal);
		slider->setRange(-180, 180);
		slider->setSingleStep(1);
		slider->setPageStep(10);
		slider->setTickInterval(10);
		slider->setTickPosition(QSlider::TicksBelow);
		return slider;
}
Widget::~Widget()
{

}
