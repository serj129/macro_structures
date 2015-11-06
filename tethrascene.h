#include <QGraphicsScene>
#include "point3d.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
//double calc(int type, QPointF a, QPointF b);
class QVector3D;
class tethraScene : public QGraphicsScene
{
	Q_OBJECT
public:
	tethraScene(QObject * parent = 0);
	~tethraScene();
	int currentType;
	double A;
	double lamda;
	void setColor(QColor col) {color = col;};

	std::vector<std::vector<double> > & giveSynapses(int s1, int s2, int s12, double A, double lamda);
	std::vector<Point3D> giveMarkedPoints ();
//	std::vector <QPointF> giveMarkedPoints ();
//	std::vector<QPair<QPointF, int> > giveMarkedPointsWithType();
//	unsigned giveNumber();
//	std::vector<std::vector<double> > & giveSynapses(int s1, int s2, int s3, double lamda, double A);
//	void setColor(QColor col) {color = col;};
public slots:
	void rotateOY(int angle);
	void rotateOX(int angle);
	void setStruct1() {currentType =0;};
	void setStruct2() {currentType = 1; qDebug()<<"setStruc2";};
	void readPointsFromFile();
	void writePointsToFile();
        void writeCoordinatesToFile();
    signals:
	void checkedPointSelected(int n);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
//	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

	void drawPoints();
private:
			QColor color;
	std::vector <Point3D> points;
	std::vector <Point3D> referencePoints;
	std::vector <QPoint> lines;
	QPointF mousePos;
	void restoreView();
	double calc(int type, Point3D a, Point3D b);
	void updatePointsNumbers();
	void updatePointsNumberAfterUnCheck(int n);
//				QGraphicsEllipseItem * pEllipseItem;
//			int struct_num;
//			void updatePointsNumbers();



};
	std::vector <int> concatVector (std::vector <Point3D> & first, std::vector<Point3D> second);

	bool sort_fun(Point3D p1, Point3D p2);
