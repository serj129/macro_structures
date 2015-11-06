#ifndef STRUCT3D_H
#define STRUCT3D_H

#include <QWidget>
#include "tethrascene.h"
#include "abstractstructure.h"

namespace Ui {
    class struct3d;
}
class QGraphicsView;
class QSlider;

class struct3d : public AbstractStructure
{
    Q_OBJECT

public:
    explicit struct3d(QWidget *parent = 0);
    ~struct3d();
		unsigned giveNumber(){return giveSynapses().size();};
		std::vector<std::vector<double> > & giveSynapses();


	public slots:
		void saveImage();
		void zoomIn();
		void zoomOut();
		void color() ;
		void save();
		void showOrder(int n);

private:
    Ui::struct3d *ui;
		QSlider *xSlider;
		QSlider *ySlider;
		QGraphicsView * view;
		tethraScene * scene;
};

#endif // STRUCT3D_H

