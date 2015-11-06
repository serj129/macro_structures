#include <QGraphicsScene>
#include <vector>
double calc(int type, QPointF a, QPointF b);
class CellScene : public QGraphicsScene
{
	Q_OBJECT
public:
	CellScene(QObject * parent = 0);
	std::vector <QPointF> giveMarkedPoints ();
	std::vector<QPair<QPointF, int> > giveMarkedPointsWithType();
	unsigned giveNumber();
	std::vector<std::vector<double> > & giveSynapses(int s1, int s2, int s3, double lamda, double A);
	void setColor(QColor col) {color = col;};
public slots:
	void save();
	void clear();
	void setStruct1(bool n);
	void setStruct2(bool n);
	void setXcount(QString s);
	void setYcount(QString s);
protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
	void drawPoints();
private:
		  QColor color;
        std::vector <QPointF> points;
        QGraphicsEllipseItem * pEllipseItem;
		  int struct_num;
			void updatePointsNumbers();
        //void drawEllipses(double a, unsigned x, unsigned y);

};

