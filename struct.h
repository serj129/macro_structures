#ifndef STRUCT_H
#define STRUCT_H

#include <QWidget>
#include <QGraphicsView>
#include "abstractstructure.h"

namespace Ui {
	 class structure;
}

class CellScene;


class structure : public AbstractStructure
{
    Q_OBJECT

public:
	 explicit structure(QWidget *parent = 0);

	 unsigned giveNumber();
	 std::vector<std::vector<double> > & giveSynapses();
	 ~structure();

private:
	 Ui::structure *ui;
	 CellScene * scene;
	 QGraphicsView * view;

 public slots:
	 void saveImage();
	 void zoomIn();
	 void zoomOut();
	 void color();
	 void save();
};

#endif // STRUCT_H
