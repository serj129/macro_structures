#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <map>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>



class QVBoxLayout;

long long int BinArrToDec(std::vector<double> d);

class plotter : public QWidget
{
    Q_OBJECT

public:
	 plotter(QWidget *parent = 0);
	 void calculate(long n, char * name);
	 void changeCurve(QwtPlotCurve::CurveStyle n);
 public slots:
	 void save();
//	 void update(double * xval, double * yval, long long size);
	 void update(const QVector<QPointF> & data);
	 void update();
private:

	 QwtPlot plot;
	 QwtPlotCurve curve;
	 std::map<long long int, long, std::less<long long int> > distrib;
	 std::map<long long int, long, std::less<long> >::iterator iter;
	 QVBoxLayout *layout;
};

#endif // PLOTTER_H
