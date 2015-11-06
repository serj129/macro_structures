#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QTabWidget>
#include "abstractstructure.h"

namespace Ui {
    class configWindow;
//		class MainWindow;
}



class configWindow : public QTabWidget

{
	friend class MainWindow;
	Q_OBJECT

public:
    explicit configWindow(QWidget *parent = 0);
		bool drawDistrubution;
		bool drawHamiltonian;
		bool writeToFiles;
		QString dbName;
		QString dbHost;
		int dbPort;
		QString dbProvider;
		QString dbUser;
		QString dbPass;
		int x_count;
		int y_count;
				~configWindow();
	public slots:
		void saveConfig ();





private:
Ui::configWindow *ui;
};

#endif // CONFIGWINDOW_H
