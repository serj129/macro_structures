#include "cellscene.h"
#include <math.h>
#include <QtGui>
#include <vector>
#include <QDebug>
#include <fstream>
#include <QFileDialog>
#include <algorithm>

unsigned x_count = 15;
unsigned y_count = 15;

void CellScene::setXcount(QString s)
{
	x_count=s.toInt();
}
void CellScene::setYcount(QString s)
{
	y_count=s.toInt();
}


const double a = 20;
double A =0;
double lamda = 0;
void CellScene::drawPoints()
{
	points.clear();
	for (unsigned i = 0; i < y_count; i++)
		for (unsigned j = 0; j < x_count; j++) {
			QPointF temp;
			temp.setY(i * a * sin(M_PI / 3));
			if (i % 2 != 0) {
				if (j == 0)
					temp.setX(a * cos(M_PI / 3));
				if (j > 0) {
					temp.setX(0);
					for (unsigned k = 1; k <= j; k++)
						temp.setX(temp.x() + ((k - 1) % 2 + 1) * a);
					temp.setX(temp.x() + a * cos(M_PI / 3));
				}
			}
			if (i % 2 == 0) {
				if (j == 0)
					temp.setX(0);
				if (j > 0) {
					temp.setX(0);
					for (unsigned k = 1; k <= j; k++)
						temp.setX(temp.x() + ((k) % 2 + 1) * a);
				}
			}
			points.push_back(temp);
		}
	QGraphicsEllipseItem * pEllipseItem;
	QGraphicsLineItem * pLineItem;
	for (unsigned i = 0; i < points.size(); i++)
	{
		pEllipseItem = new QGraphicsEllipseItem(0, this);
		pEllipseItem->setPen(QPen(Qt::blue, 2));
		pEllipseItem->setRect(QRect(points[i].x(), points[i].y(), 4, 4));
		pEllipseItem->setData(0, "cellpoint");
		pEllipseItem->setData(1, "");
		pEllipseItem->setData(3,points[i]/a);
	}

//for (unsigned j=1;j<=y_count/2;j++)
//	{
//	for (unsigned i=0;i<x_count/2;i++)
//	{
//	pLineItem = new QGraphicsLineItem(0, this);
//	pLineItem->setLine(points[2*i].x(),points[2*i].y(),points[2*i+j*x_count].x(),points[2*i+j*x_count].y());
//	pLineItem->setPen(QPen(Qt::blue, 1));
//	pLineItem->setData(0, "line");

//	pLineItem = new QGraphicsLineItem(0, this);
//	pLineItem->setLine(points[2*i+j*x_count].x(),points[2*i+j*x_count].y(),points[2*i+j*x_count+1].x(),points[2*i+j*x_count+1].y());
//	pLineItem->setPen(QPen(Qt::blue, 1));
//	pLineItem->setData(0, "line");

//	pLineItem = new QGraphicsLineItem(0, this);
//	pLineItem->setLine(points[2*i+1].x(),points[2*i+1].y(),points[2*i+j*x_count+1].x(),points[2*i+j*x_count+1].y());
//	pLineItem->setPen(QPen(Qt::blue, 1));
//	pLineItem->setData(0, "line");

//	//pLineItem = new QGraphicsLineItem(points[2*i].x(),points[2*i].y(),points[2*i+j*x_count].x(),points[2*i+j*x_count].y(),this);

//	}

//}
}

CellScene::CellScene(QObject * parent) :
	QGraphicsScene(parent) {
//	setSceneRect(-10, -10, 300, 300);
//	setSceneRect(10, 10, 300, 300);
	struct_num=0;
	drawPoints();

}

void CellScene::setStruct1(bool n) {
	struct_num=0;
//	qDebug()<<struct_num;
}

void CellScene::setStruct2(bool n) {
	struct_num=1;
//	qDebug()<<struct_num;
}
std::vector<QPointF> CellScene::giveMarkedPoints() {
	std::vector < QPointF > points;
	foreach (QGraphicsItem * it, this->items())
		if (it->data(1)=="marked" && it->data(0) == "cellpoint") {
		points.push_back(it->data(3).toPointF());
//		qDebug() <<it->data(3);
	}
//	qDebug() <<points;
	return points;
}

bool sortByNumber(QGraphicsItem * it1, QGraphicsItem * it2)
{
	return (it1->data(4).toInt()<it2->data(4).toInt());
}

//bool myfunction (int i,int j) { return (i<j); }

	std::vector<QPair<QPointF, int> > CellScene::giveMarkedPointsWithType() {
		std::vector<QPair<QPointF, int> > points;
		QPair<QPointF, int> point;
		std::vector<QGraphicsItem *> items;
		foreach (QGraphicsItem * it, this->items())
			if (it->data(1)=="marked" && it->data(0) == "cellpoint")
				items.push_back(it);
		std::sort (items.begin(), items.end(), sortByNumber);

//		foreach (QGraphicsItem * it, this->items())
		foreach (QGraphicsItem * it, items)
			if (it->data(1)=="marked" && it->data(0) == "cellpoint")
			{
			point.first=it->data(3).toPointF();
			point.second=it->data(2).toInt();
			points.push_back(point);
//				qDebug() <<it->data(3)<<"\t number"<<it->data(4);
			}
//			qDebug() <<points;
			return points;
		}

void CellScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent) {
	QGraphicsItem * item = itemAt(mouseEvent->scenePos());
	QGraphicsEllipseItem * item1;
	if (mouseEvent->button() == Qt::LeftButton)
		if (item)
			if (item->data(0) == "cellpoint" && item->data(1) == "") {
//				qDebug() << "marked";
				item1 = (QGraphicsEllipseItem*) item;
				item1->setPen(QPen(color, 3));
				item->setData(1, "marked");
				item->setData(2,struct_num);
				updatePointsNumbers();
				item->setData(4,0);

//				qDebug() <<item->data(2);
			}
	if (mouseEvent->button() == Qt::RightButton)
		if (item)
			if (item->data(1) == "marked") {
				item1 = (QGraphicsEllipseItem*) item;
				item1->setPen(QPen(Qt::blue, 2));
				item->setData(1, "");
				item->setData(2, "");
				item->setData(3,"");
				item->setData(4,"");
//				qDebug() <<item->data(2);
//				qDebug() << "unmarked";
			}
}

void CellScene::updatePointsNumbers()
{
	foreach (QGraphicsItem * it, this->items())
		if (it->data(1)=="marked")
			it->setData(4,it->data(4).toInt()+1);
}

double distance (unsigned i,unsigned j, std::vector<QPointF> & points) {
//	qDebug()<<points[i].x();
	return sqrt(pow(points[i].x()-points[j].x(),2)+pow(points[i].y()-points[j].y(),2));
}

void CellScene::save() {
	std::ofstream out;
	QString fileName = QFileDialog::getSaveFileName((QWidget *) this->parent(),
										 tr("Save as:"),
										 "synapses.txt",
										 tr("Text Files (*.txt);;All Files (*)"));
	out.open(fileName.toAscii(), std::ios::out);
	out<<std::scientific<<std::noshowpos;
	std::vector<QPointF> points = giveMarkedPoints();
	for (unsigned i=0;i<points.size();i++) {
		for(unsigned j=0;j<points.size();j++)
			if (i==j) out<<"1.000000e+000\t";
			else out<<-pow(distance(i,j,points)/a,-3)<<'\t';
			if (i<points.size()-1) out<<std::endl;
		}
	out.close();
}

void CellScene::clear() {
	QGraphicsEllipseItem* item1;
	foreach (QGraphicsItem * it, this->items())
	if (it->data(1)=="marked") {
		item1 = (QGraphicsEllipseItem*) it;
		item1->setPen(QPen(Qt::blue, 2));
		it->setData(1, "");
		it->setData(2, "");
	}

}

unsigned CellScene::giveNumber()
{
	return giveMarkedPoints().size();
}

/*

giveSynapses(ui->comboBox->currentIndex(),ui->comboBox_2->currentIndex(),
									ui->comboBox_3->currentIndex(),ui->lineEdit->text().toDouble(),
									ui->lineEdit_2->text().toDouble()
	*/


std::vector<std::vector<double> > & CellScene::giveSynapses(int s1, int s2, int s12, double A, double lamda)
{
	::A=A;
	::lamda=lamda;
//	qDebug()<<"lam"<<::lamda;
//	qDebug()<<"A"<<::A;
//	qDebug()<<"type1"<<s1;
//	qDebug()<<"type2"<<s2;
//	qDebug()<<"type12"<<s12;
	static std::vector<std::vector<double> > syn;
	std::vector<double> tmp;

	std::vector<QPair<QPointF, int> > points = this->giveMarkedPointsWithType();
	syn.clear();
	for (unsigned i=0;i<points.size();i++)
	{
//		qDebug()<<points[i].first;
		tmp.clear();
		for(unsigned j=0;j<points.size();j++)

			if (i==j) tmp.push_back(1);
			else
			{
				if (points[i].second != points[j].second)
					tmp.push_back(calc(s12,points[i].first, points[j].first));
				if (points[i].second == points[j].second && points[j].second==0)
					tmp.push_back(calc(s1,points[i].first, points[j].first));
				if (points[i].second == points[j].second && points[j].second==1)
					tmp.push_back(calc(s2,points[i].first, points[j].first));
			}
		syn.push_back(tmp);
	}
	return syn;
}

double calc(int type, QPointF a, QPointF b)
{
//	qDebug()<<"type in calc"<<type;
	switch (type)
	{
	case 1: //qDebug()<<"coloumb";
		return A/sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()));
		 break;//coulomb
	case 3: //qDebug()<<"dipole";
		return A/pow(sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y())),3);
		break;//dipole
	case 2: //qDebug()<<"hukkel";
		return A*exp(-sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()))/lamda)/pow(sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y())),3);
		break;
	default: return 0; //qDebug()<<"unknown";
	}
}
