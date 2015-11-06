#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "struct.h"
#include "neuron.h"
#include "string.h"
#include "struct3d.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariant>
#include <configwindow.h>
#include <QTableWidgetItem>
#include <QClipboard>
#include <QTableWidget>
#include <QKeyEvent>
#include <QApplication>


class plotter;
class QTableWidgetItem;
class QTableWidget;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
		bool connectDB();
    ~MainWindow();
 public slots:
	 void NewStructure();
	 void New3dStructure();
	 void EditStructure();
	 void about();
	 void createNet();
	 void saveNet();
	 void openNet();
	 void runTest();
	 void runFastTest();
	 void createInputs();
	 void showDistribInfo();
	 void updateDiagramms();
	 void giveProto(QTableWidgetItem * item);



private:

	Ui::MainWindow *ui;
	 AbstractStructure *cform;
	 net_hop * net;
	 plotter * plotWidget;
	 plotter * hamil_graph;
	 configWindow * configWin;
	 QTableWidget * distrInfo;
	 void toLog(QString message);
	 QSqlDatabase db;
	 void reLayout();
	 QTableWidget * protoInfo;

};

bool mysort (const QPointF & d1, const QPointF & d2);
bool mysort1 (const QPoint & d1, const QPoint & d2);
QString toBinary(long long n, int digitNum);
void makeInputFile(long n);



class QTableWidgetLongItem : public QTableWidgetItem
{
public:
    QTableWidgetLongItem(QString text): QTableWidgetItem(text){};
    bool operator< ( const QTableWidgetItem& other ) const {return this->text().toLongLong()<other.text().toLongLong();};

};

class TableWin : public QTableWidget
{
private:
    void keyPressEvent(QKeyEvent *event){
	{
	    qDebug()<<"ctrl+C";
	    if (!(event->matches(QKeySequence::Copy) )) return;
		QString cbStr;
		QModelIndexList list =  this->selectionModel()->selectedIndexes();
		int i, j, firstRow, lastRow, rowCount;
		if( list.isEmpty() ) return;

			firstRow = list.first().row();  lastRow = list.last().row();
			rowCount = lastRow - firstRow + 1;

		      for(i = 0; i < rowCount; ++i, cbStr += '\n')
		       for(j = i; j < list.count(); j += rowCount, cbStr += '\t')
			 cbStr += this->model()->data(list[ j ], Qt::EditRole).toString();

		      QApplication::clipboard()->setText(cbStr);

		      return;

	}
    }

};

#endif // MAINWINDOW_H
