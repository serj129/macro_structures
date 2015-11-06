#include "plotter.h"
#include <qwt_scale_map.h>
#include <qwt_symbol.h>
#include <qwt_math.h>
#include <fstream>
#include <QVBoxLayout>
#include <qwt_plot_zoomer.h>
#include <QFileDialog>
#include <qwt_plot_renderer.h>
#include <QImageWriter>

QwtScaleMap xMap;
QwtScaleMap yMap;
double * Xval;
double * Yval;

long long int BinArrToDec(std::vector<double> d) {
	double result = 0;
	double last_number = 0;
	long long int n = 0;
	while (!d.empty()) {
		last_number = d.back();
		d.pop_back();
		if (last_number == -1)
			last_number = 0;
		result += ((long long int) last_number) << (n++);
	}
	return result;
}

void plotter::update()
{
//	plot.updateLayout();
	qDebug()<<"update";
	curve.detach();
	curve.attach(&plot);
	plot.replot();

}

void plotter::changeCurve(QwtPlotCurve::CurveStyle n) {
	curve.setStyle(n);
}

plotter::plotter(QWidget *parent) : QWidget(parent)

{
	curve.setSymbol(new QwtSymbol(QwtSymbol::Ellipse, Qt::yellow, QPen(Qt::blue), QSize(2, 2) ) );
	curve.setPen(QColor(Qt::red));
	curve.setStyle(QwtPlotCurve::Sticks);
	plot.setFixedHeight(200);
//	plot.setTitle("Distribution of recognized sequences");
	curve.setRenderHint(QwtPlotItem::RenderAntialiased);
	curve.attach(&plot);


	QwtPlotZoomer* zoomer = new QwtPlotZoomer(plot.canvas());
	zoomer->setRubberBandPen( QColor(Qt::black));
	zoomer->setTrackerPen( QColor( Qt::black ) );
	zoomer->setTrackerMode(QwtPlotZoomer::AlwaysOn);
	zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
		 Qt::RightButton, Qt::ControlModifier );
	zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
		 Qt::RightButton );





	layout = new QVBoxLayout;
	layout->addWidget(&plot);
	this->setLayout(layout);
}

void plotter::calculate(long n, char * name)
{
	distrib.clear();
	std::ifstream in(name);
	std::vector <double> tmp;
	double temp;
	while (in)
{
	for (unsigned i=0;i<n;i++)
	{
		in>>temp;
		tmp.push_back(temp);
	}

	long long int key=BinArrToDec(tmp);
	tmp.clear();
	iter=distrib.find(key);
	if (iter!=distrib.end()) distrib[key]++;
	else distrib[key]=1;

}
	in.close();


	long long int i=0;
	if (Xval) delete Xval; Xval=NULL;
	if (Yval) delete Yval; Yval=NULL;
	Xval = new double [distrib.size()];
	Yval = new double [distrib.size()];
	for (iter=distrib.begin();iter!=distrib.end();iter++)
	{
		Xval[i]=double(iter->first);
		Yval[i]=double(iter->second);
		i++;
	}

	curve.setRawSamples(Xval, Yval, distrib.size());
	plot.replot();



	std::ofstream out("distr.dat");
	for (iter=distrib.begin();iter!=distrib.end();iter++)
		out<<iter->first<<";\t"<<iter->second<<std::endl;
	out.close();
}

void plotter::update(const QVector< QPointF > & data)
{
//	curve.setRawSamples(xval, yval,size);
//	setSamples (const QVector< QPointF > &)
	curve.setSamples(data);
	plot.replot();
}

void plotter::save()
{
	QString fileName = "distrib.svg";
	const QList<QByteArray> imageFormats =
		 QImageWriter::supportedImageFormats();

	QStringList filter;
	filter += "SVG Documents (*.svg)";
	filter += "PDF Documents (*.pdf)";
	filter += "Postscript Documents (*.ps)";
	if ( imageFormats.size() > 0 )
	{
		 QString imageFilter("Images (");
		 for ( int i = 0; i < imageFormats.size(); i++ )
		 {
			 if ( i > 0 )
					imageFilter += " ";
			 imageFilter += "*.";
			 imageFilter += imageFormats[i];
		 }
		 imageFilter += ")";

		 filter += imageFilter;
	}

	fileName = QFileDialog::getSaveFileName(
					this, "Export File Name", fileName,
					filter.join(";;"), NULL, QFileDialog::DontConfirmOverwrite);

	if ( !fileName.isEmpty() )
	{
		 QwtPlotRenderer renderer;
		 renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, false);
		 renderer.renderDocument(&plot, fileName, plot.size(), 85);//QSizeF(300, 200)
	}
}





