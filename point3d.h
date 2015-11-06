#ifndef point3d_h
#define point3d_h

#include <QVector3D>
#include <vector>
#include <QColor>

//class QVector3D;
class Point3D : public QVector3D
{
private:
	bool checked;


	double a;

public:
	bool waschecked;
	int order;
	int type;
	QColor color;
	bool isChecked() { return checked;};
	void setChecked() { checked= true;};
	void setUnChecked() { checked= false;};
	void wasChecked() {waschecked=true;};
	Point3D (double x, double y, double z): QVector3D(x,y,z), checked(false),waschecked(false),type(0),a(150), order(0) {};
	Point3D (QVector3D p): QVector3D(p), checked(false),waschecked(false),type(0),a(150), order(0) {};
	Point3D (): QVector3D(),checked(false),waschecked(false), type(0),a(150), order(0) {};
	qreal distance (const QVector3D * neib) {return QVector3D(this->x()-neib->x(),this->y()-neib->y(),this->z()-neib->z()).length();};
	Point3D rotateXYZ(double a, double b, double c);
	Point3D rotateCustom(const QVector3D & axis, double angle);
	std::vector <double> vector4D();
	QVector3D toVector3D () { return QVector3D (x(),y(),z());};
	std::vector <Point3D> createNextPoints(Point3D &prevP);
	std::vector <Point3D> makePoints (int a1,int a2, int a3, double dz);



};


#endif // point3d_h
