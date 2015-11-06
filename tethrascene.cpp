#include "tethrascene.h"
#include <QVector3D>
#include <math.h>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QtGui>
#include <QDebug>
#include <algorithm>
#include <fstream>
//double a = 150;
double a = 80;
double xangle = 0;
double yangle = 0;



Point3D p(0,0,1);//axis of rotation
Point3D prevP(0,0,sqrt(6)*a/12);

void tethraScene::restoreView()
{
	points.resize(referencePoints.size());
	for (unsigned i=0;i<points.size();i++)
	{
		points[i]=referencePoints[i].rotateXYZ(xangle,0,0);
		points[i]=points[i].rotateCustom(p,yangle);
	}

}



tethraScene::tethraScene(QObject * parent) :
	QGraphicsScene(parent)
{
//	setSceneRect(0,0,300,300);

	currentType = 0;
	A =0;
	lamda = 0;
	referencePoints.push_back(QVector3D(0,a*cos(atan(sqrt(2))),-a*sqrt(6)/12));
	referencePoints.push_back(QVector3D(a*cos(atan(sqrt(2)))*sin(2*M_PI/3),a*cos(atan(sqrt(2)))*cos(2*M_PI/3),-a*sqrt(6)/12));
	referencePoints.push_back(QVector3D(a*cos(atan(sqrt(2)))*sin(-2*M_PI/3),a*cos(atan(sqrt(2)))*cos(-2*M_PI/3),-a*sqrt(6)/12));
	referencePoints.push_back(QVector3D(0,0,a*sqrt(6)/4));
	referencePoints.push_back(QVector3D(0,0,0));
	points.resize(referencePoints.size());
	for (unsigned i=0;i<referencePoints.size();i++)
	{
//		referencePoints[i]=QVector3D(referencePoints[i].x()+10,referencePoints[i].y()+10,referencePoints[i].z()+10);
		points[i]=referencePoints[i];
	}
	points.resize(referencePoints.size());
	drawPoints();


//	Point3D point(referencePoints[1]);
//	concatVector(referencePoints,point.createNextPoints(referencePoints[0]));
//	restoreView();
//	drawPoints();

}
tethraScene::~tethraScene()
{

}

void tethraScene::drawPoints()

{

		this->clear();
		QGraphicsEllipseItem * pEllipseItem;

//		QGraphicsLineItem * pLineItem, *pLineItem1, *pLineItem2,*pLineItem3,*pLineItem4,*pLineItem5;
//		pLineItem = new QGraphicsLineItem(0,this);
//		pLineItem->setPen(QPen(Qt::red, 1));
//		pLineItem->setLine(points[0].x(),points[0].y(),points[1].x(),points[1].y());
//		pLineItem1 = new QGraphicsLineItem(0,this);
//		pLineItem1->setLine(points[1].x(),points[1].y(),points[2].x(),points[2].y());
//		pLineItem2 = new QGraphicsLineItem(0,this);
//		pLineItem2->setLine(points[2].x(),points[2].y(),points[0].x(),points[0].y());
//		pLineItem3 = new QGraphicsLineItem(0,this);
//		pLineItem3->setPen(QPen(Qt::magenta, 1));
//		pLineItem3->setLine(points[3].x(),points[3].y(),points[0].x(),points[0].y());
//		pLineItem4 = new QGraphicsLineItem(0,this);
//		pLineItem4->setPen(QPen(Qt::cyan, 1));
//		pLineItem4->setLine(points[2].x(),points[2].y(),points[3].x(),points[3].y());
//		pLineItem5 = new QGraphicsLineItem(0,this);
//		pLineItem5->setPen(QPen(Qt::green, 1));
//		pLineItem5->setLine(points[1].x(),points[1].y(),points[3].x(),points[3].y());
		QGraphicsLineItem * pLineItem;
		for (unsigned i=0;i<lines.size();i++)
		{
			pLineItem = new QGraphicsLineItem(0,this);
                        //pLineItem->setPen(QPen(Qt::cyan, 1));
                        pLineItem->setPen(QPen(Qt::black, 2));
			pLineItem->setLine(points[lines[i].x()].x(),points[lines[i].x()].y(),points[lines[i].y()].x(),points[lines[i].y()].y());
		}
		for (unsigned i = 0; i < points.size(); i++)
		{
//			qDebug()<<points[i];
			pEllipseItem = new QGraphicsEllipseItem(0, this);
			if (referencePoints[i].isChecked())
                        {
                            pEllipseItem->setPen(QPen(referencePoints[i].color, 5));
                            pEllipseItem->setRect(QRect(points[i].x()-5, points[i].y()-5, 10, 10));
                        }
                            else
                        {
                                pEllipseItem->setPen(QPen(Qt::blue, 2));
                                pEllipseItem->setRect(QRect(points[i].x()-2, points[i].y()-2, 4, 4));
                        }

			pEllipseItem->setData(0, "point");
			pEllipseItem->setData(1,i);
//			pEllipseItem->setData(2, "");
//			pEllipseItem->setData(1, QRectF(referencePoints[i].x(),referencePoints[i].y(),referencePoints[i].z(),0));
		}
}


void tethraScene::rotateOX(int angle)
{
	xangle = angle*M_PI/180;
	restoreView();
	p=points[3];
	drawPoints();
}

void tethraScene::rotateOY(int angle)
{
	yangle = angle*M_PI/180;
	restoreView();
	drawPoints();

}

std::vector <int> concatVector (std::vector <Point3D> &first, const std::vector<Point3D> second)
{
	bool isNotContain = true;
	std::vector <int> indexes;
	for (unsigned i=0;i<second.size();i++)
	{
		for (unsigned j=0;j<first.size();j++)
		if ((first[j]-second[i]).length()<1e-5)
			{
				isNotContain = false;
				indexes.push_back(j);

			}

		if (isNotContain)
		{
			first.push_back(second[i]);
			indexes.push_back(first.size()-1);
		}
//		qDebug()<<isNotContain;
		isNotContain = true;
	}
	return indexes;
}

void tethraScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	mousePos = mouseEvent->scenePos();
	QGraphicsItem * item = itemAt(mouseEvent->scenePos());
//	QGraphicsEllipseItem * item1;

	int index;
	std::vector <Point3D> nextPoints;
	std::vector <int> indexes;
	if (mouseEvent->button() == Qt::LeftButton)
		if (item)
			if (item->data(0) == "point")
			{
				qDebug() << "marked";
				index=item->data(1).toInt();
				if (referencePoints[index].isChecked()) return;
				referencePoints[index].setChecked();
				referencePoints[index].type=currentType;
				referencePoints[index].color = color;
				updatePointsNumbers();
				referencePoints[index].order = 0;
				if (!referencePoints[index].waschecked)
				{
					nextPoints = referencePoints[index].createNextPoints(prevP);
					indexes = concatVector(referencePoints,nextPoints);
					for (unsigned i=0;i<indexes.size();i++)
						lines.push_back(QPoint(index,indexes[i]));
				}
				prevP=referencePoints[index];

				restoreView();
				drawPoints();
			}
	if (mouseEvent->button() == Qt::RightButton)
		if (item)
		{
			index=item->data(1).toInt();
			if (referencePoints[index].isChecked())
			{
				referencePoints[index].setUnChecked();
				referencePoints[index].wasChecked();
				updatePointsNumberAfterUnCheck(referencePoints[index].order);
			}
			restoreView();
			drawPoints();
			qDebug() << "unmarked";

		}


}
void tethraScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{

//	double OXAngle=(mouseEvent->scenePos().y()-mousePos.y())/height();
//	double OYAngle=(mouseEvent->scenePos().x()-mousePos.x())/width();

//	for (unsigned i=0;i<points.size();i++)
//	{

//		points[i]=points[i].rotateXYZ(OXAngle,0,0);
//		points[i]=points[i].rotateCustom(p,OYAngle);
//	}
//	drawPoints();
//	mousePos = mouseEvent->scenePos();
    QGraphicsItem * item = itemAt(mouseEvent->scenePos());
    if (item)
	    if (item->data(0) == "point")
		if (referencePoints[item->data(1).toInt()].isChecked())
		    emit checkedPointSelected(referencePoints[item->data(1).toInt()].order);

}

std::vector<Point3D> tethraScene::giveMarkedPoints ()
{
	std::vector<Point3D> result;
	for (unsigned i=0;i<referencePoints.size();i++)
		if (referencePoints[i].isChecked())
		{
			result.push_back(referencePoints[i]);
			qDebug()<<"type"<<referencePoints[i].type<<referencePoints[i];
		}

	std::sort(result.begin(),result.end(),sort_fun);
		return result;
}

void tethraScene::updatePointsNumbers()
{
	for (unsigned i=0;i<referencePoints.size();i++)
		if (referencePoints[i].isChecked())
			referencePoints[i].order++;

}
void tethraScene::updatePointsNumberAfterUnCheck(int n) {
    for (unsigned i=0;i<referencePoints.size();i++)
	    if (referencePoints[i].isChecked() && referencePoints[i].order>n)
		referencePoints[i].order--;

}

bool sort_fun(Point3D p1, Point3D p2)
{
	return (p1.order<p2.order);
}

std::vector<std::vector<double> > & tethraScene::giveSynapses(int s1, int s2, int s12, double A, double lamda)
{
	this->A=A;
	this->lamda=lamda;
//	qDebug()<<"lam"<<::lamda;
//	qDebug()<<"A"<<::A;
//	qDebug()<<"type1"<<s1;
//	qDebug()<<"type2"<<s2;
//	qDebug()<<"type12"<<s12;
	static std::vector<std::vector<double> > syn;
	std::vector<double> tmp;

	std::vector<Point3D> points = giveMarkedPoints ();
	syn.clear();
	for (unsigned i=0;i<points.size();i++)
	{
//		qDebug()<<points[i].first;
		tmp.clear();
		for(unsigned j=0;j<points.size();j++)

			if (i==j) tmp.push_back(1);
			else
			{
				if (points[i].type != points[j].type)
					tmp.push_back(calc(s12,points[i], points[j]));
				if (points[i].type == points[j].type && points[j].type==0)
					tmp.push_back(calc(s1,points[i], points[j]));
				if (points[i].type == points[j].type && points[j].type==1)
					tmp.push_back(calc(s2,points[i], points[j]));
			}
		syn.push_back(tmp);
	}
	return syn;
}

double tethraScene::calc(int type, Point3D a, Point3D b)
{
//	qDebug()<<"type in calc"<<type;
	double distance =(a-b).length()*4/::a/sqrt(6);
	distance = distance*1.54e-10; // single C-C link length
	switch (type)
	{
	case 1: qDebug()<<"coloumb";
		return A/distance;
		 break;//coulomb
	case 3: qDebug()<<"dipole";
		return A/pow(distance,3);
		break;//dipole
	case 2: qDebug()<<"hukkel";
		return A*exp(-distance/lamda)/distance;
		break;
	default: return 0; qDebug()<<"unknown";
	}
}

void tethraScene::writePointsToFile()
{
	std::ofstream out;
	QString fileName = QFileDialog::getSaveFileName((QWidget*)this->parent(),
										 tr("Save as:"),
										 "struct3d.dat",
										 tr("Binary files (*.dat);;All Files (*)"));
	out.open(fileName.toAscii(), std::ios::binary);
	int numberOfPoints=referencePoints.size();
	int numberOfLines=lines.size();
	out.write(reinterpret_cast<char *> (&numberOfPoints),sizeof(int));
	for (unsigned i=0;i<referencePoints.size();i++)
		out.write(reinterpret_cast<char *> (&referencePoints[i]),sizeof(Point3D));
	out.write(reinterpret_cast<char *> (&numberOfLines),sizeof(int));
	for (unsigned i=0;i<lines.size();i++)
		out.write(reinterpret_cast<char *> (&lines[i]),sizeof(QPoint));
	out.close();
}
void tethraScene::writeCoordinatesToFile()
{
    std::ofstream out;
    QString fileName = QFileDialog::getSaveFileName((QWidget*)this->parent(),
                                                    tr("Save as:"),
                                                    "struct3d.txt",
                                                    tr("Text files (*.txt);;All Files (*)"));
    out.open(fileName.toAscii(),std::ios::out);
    for (unsigned i = 0;i<referencePoints.size(); i++)
        if (referencePoints[i].isChecked())
        out<<referencePoints[i].x()<<"\t"<<referencePoints[i].y()<<"\t"<<referencePoints[i].z()<<"\n";
    out.close();
}

void tethraScene::readPointsFromFile()
{
	std::ifstream in;

	QString fileName = QFileDialog::getOpenFileName((QWidget*)this->parent(),tr("Open file with structure data"),".",tr("Binary files (*.dat);;All Files (*)"));
	if (fileName.isEmpty()) return;

	in.open(fileName.toAscii(), std::ios::binary);
	int numberOfPoints;
	int numberOfLines;
	Point3D point;
	QPoint l;
	referencePoints.clear();
	lines.clear();
	in.read (reinterpret_cast<char *>(&numberOfPoints), sizeof(int));
	for (int i=0;i<numberOfPoints;i++)
	{
		in.read(reinterpret_cast<char *> (&point),sizeof (Point3D));
		referencePoints.push_back(point);
	}
	in.read (reinterpret_cast<char*> (&numberOfLines),sizeof(int));
	for (int i=0;i<numberOfLines;i++)
	{
		in.read(reinterpret_cast<char *> (&l),sizeof(QPoint));
		lines.push_back(l);
	}
	in.close();
	restoreView();
	drawPoints();

}




