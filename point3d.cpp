#include "point3d.h"
#include <math.h>
#include <QDebug>



std::vector<std::vector<double> > multiply (const std::vector<std::vector<double> > &a, const std::vector<std::vector<double> >&b)
{
	std::vector<std::vector<double> > result;
	if (a[0].size()!=b.size()) return result;
	std::vector<double> tmp;
	double s=0;
	for (unsigned i=0;i<a.size(); i++)
	{
		for (unsigned j=0;i<b[0].size();j++)
		{
			for (unsigned k=0;k<a[0].size();k++)
				s+=a[i][k]*b[k][j];
			tmp.push_back(s);
			s=0;
		}
			result.push_back(tmp);
			tmp.clear();
	}
	return result;
}

std::vector <double> Point3D::vector4D ()
{
	std::vector <double> result;
	result.push_back(x());
	result.push_back(y());
	result.push_back(z());
	result.push_back(1);
	return result;
}

Point3D Point3D::rotateXYZ(double a, double b, double c)
{
	Point3D point;
	point.setX(x());
	point.setY(y()*cos(a)-z()*sin(a));
	point.setZ(y()*sin(a)+z()*cos(a));

	point.setX(point.x()*cos(b)-point.y()*sin(b));
	point.setY(point.y());
	point.setZ(point.x()*sin(b)+point.z()*cos(b));

	point.setX(point.x()*cos(c)-point.y()*sin(c));
	point.setY(point.x()*sin(c)+point.y()*cos(c));
	point.setZ(point.z());

 return point;

}

Point3D Point3D::rotateCustom(const QVector3D & axis, double angle)
{
	double b = acos (axis.z()/sqrt(axis.x()*axis.x()+axis.z()*axis.z()));
	double a = acos (sqrt(axis.x()*axis.x()+axis.z()*axis.z())/axis.length());
//	qDebug()<<"alfa"<<a*180/M_PI;
//	qDebug()<<"beta"<<b*180/M_PI;
	Point3D point = rotateXYZ(0.0,b,0.0);
	point = point.rotateXYZ(a,0.0,0.0);
	point= point.rotateXYZ(0.0,0.0,angle);
	point= point.rotateXYZ(-a,0.0,0.0);
	point= point.rotateXYZ(0.0,-b,0.0);
	return point;
}
std::vector <Point3D> Point3D::makePoints (int a1,int a2, int a3, double dz)
{
	std::vector <Point3D> result;
	double Rxy= a/sqrt(3);
	double A1=a1*M_PI/180;
	double A2=a2*M_PI/180;
	double A3=a3*M_PI/180;
	double Dz=sqrt(6)*a/4;

	result.push_back(Point3D(x()+Rxy*cos(A1),y()+Rxy*sin(A1),z()+dz));
	result.push_back(Point3D(x()+Rxy*cos(A2),y()+Rxy*sin(A2),z()+dz));
	if (a3==-1 || a3==1)
	{
		result.push_back(Point3D(x(),y(),z()+a3*Dz));
	qDebug()<<"vertshift";
	}
	else
		result.push_back(Point3D(x()+Rxy*cos(A3),y()+Rxy*sin(A3),z()+dz));
	return result;
}

std::vector <Point3D> Point3D::createNextPoints(Point3D &prevP)
{
	double dz;

	QVector3D shift (x()-prevP.x(),y()-prevP.y(),z()-prevP.z());
	shift.normalize();
	qDebug()<<"shift"<<shift;
	if (fabs(shift.x())<1e-5)
	{
	if (fabs(shift.y())<1e-5)
		{
		if (shift.z()>0)
			{
				dz=sqrt(6)*a/12;
				return makePoints(-90,30,150,dz);
			}
		if (shift.z()<0)
			{
				dz=-sqrt(6)*a/12;
				return makePoints(90,-30,-150,dz);
			}
		}
	if (shift.y()>0)
		{
			if (shift.z()<0)
			{dz=sqrt(6)*a/12;	return makePoints(30,150,-1,dz);}
			else
			{dz=-sqrt(6)*a/12;	return makePoints(30,150,1,dz);}
		}
	if (shift.y()<0)
		{
			if (shift.z()<0)
		{dz=sqrt(6)*a/12;return makePoints(-30,-150,-1,dz);}
			else {dz=-sqrt(6)*a/12;return makePoints(-30,-150,1,dz);}
		}
	}
	if (shift.x()>0)
	{
		if (shift.y()>0)//30
		{
			if (shift.z()<0)
			{dz=sqrt(6)*a/12;return makePoints(-30,90,-1,dz);}
			else {dz=-sqrt(6)*a/12;return makePoints(-30,90,1,dz);}
		}
		if (shift.y()<0) //-30
		{
			if (shift.z()<0)
			{dz=sqrt(6)*a/12;return makePoints(-90,30,-1,dz);}
			else {dz=-sqrt(6)*a/12;return makePoints(-90,30,1,dz);}
		}

	}
	if (shift.x()<0)
	{
		if (shift.y()>0) //-150
		{
			if (shift.z()>0)
			{dz=-sqrt(6)*a/12;return makePoints(-150,90,1,dz);}
			else {dz=sqrt(6)*a/12;return makePoints(-150,90,-1,dz);}
		}
		if (shift.y()<0)//150
		{
			if (shift.z()<0)
			{dz=sqrt(6)*a/12;return makePoints(150,-90,-1,dz);}
			else {dz=-sqrt(6)*a/12;return makePoints(150,-90,1,dz);}
		}

	}
}
