#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QMessageBox>
#include <QErrorMessage>
#include <QInputDialog>
#include <vector>
#include <sstream>
#include <QFileDialog>
#include "plotter.h"
#include <QProgressDialog>
#include <algorithm>
#include <qwt_plot_curve.h>
#include <QString>
QString toBinaryString(long n, int digitNum);

void makeInputFilebyMask(const char * str)
{
    QString mask = QString(str);
    std::vector<int> positions;
    std::list<int> data;
    std::ofstream out;
    out.open("inputs.dat",std::ios::out);
    long var;
    int n = mask.size();
    for (int i = 0; i<n; i++)
    {
      if (mask[i]=='x') positions.push_back(i);
    }
   // qDebug() <<positions;

    for (int j = 0; j<(1<<positions.size());j++)
        {
          var = 0;
          for (int k = 0; k < positions.size(); k++)
           {
             if (j&(1<<k)) var |= 1<<(n-positions[k]-1);
           }

          out<< toBinaryString(var,mask.size()).toStdString()<<'\n';
        }


    out.close();
}

class HtmlResultPrinter {
private:
	std::string html;
public:
			HtmlResultPrinter() {html="<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"mystyle.css\" /></head><body><table>";}
			void addData(const std::vector<double> &inputs, const std::vector<double> &outputs) {
		html+="<tr>";

		for (unsigned i=0;i<inputs.size();i++)
			if (inputs[i]==outputs[i])
				html+="<td class=\"equal\">"+(QString::number((inputs[i]+1)/2)).toStdString()+"</td>";
			else html+="<td>"+(QString::number((inputs[i]+1)/2)).toStdString()+"</td>";

		html+="<td>=====</td>";

		for (unsigned i=0;i<inputs.size();i++)
			if (inputs[i]==outputs[i])
				html+="<td class=\"equal\">"+(QString::number((outputs[i]+1)/2)).toStdString()+"</td>";
			else html+="<td>"+(QString::number((outputs[i]+1)/2)).toStdString()+"</td>";

		html+="</tr>";
	}
	void print(QString name) {
		html.append("</table></body></html>");
		std::ofstream out;
		out.open(name.toAscii(),std::ios::out);
		out<<html;
		out.close();
	}
};

QVector<QPointF> hamData;		//array for Hamiltonian diagramm
QVector<QPointF> distrData;		//array for Distribution diagramm

bool writeToMemory = true; //write distribution data to memory (distrData)
//bool writeToFiles = true;  //write inputs, outputs, distrib, hamiltonian logs to files


bool MainWindow::connectDB()
{
	db=QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(configWin->dbHost);
	db.setPort(configWin->dbPort);
	db.setDatabaseName(configWin->dbName);
	db.setUserName(configWin->dbUser);
	db.setPassword(configWin->dbPass);
	bool connected = db.open();
	if (!connected)
	{
		QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"),
								db.lastError().text(), 0, this);
		msgBox.addButton(tr("OK"), QMessageBox::AcceptRole);
		msgBox.exec();
	}
return connected;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->statusbar->showMessage(tr("Ready for work. Open network or create structure at first"));

	net=NULL;
	cform = new structure;
	plotWidget = new plotter;
	hamil_graph = new plotter;
	configWin = new configWindow;

	configWin->setWindowModality(Qt::WindowModal);
	distrInfo = new TableWin;
	protoInfo = new TableWin;
	protoInfo->setSortingEnabled(true);
	distrInfo->resize(300,300);
	hamil_graph->changeCurve(QwtPlotCurve::Lines);
	reLayout();


//	db=QSqlDatabase::addDatabase("QMYSQL");
////	QSqlQuery query(db);
//	db.setHostName("localhost");
//	db.setPort(3306);
//	db.setDatabaseName("dbcell");
//	db.setUserName("root");
//	db.setPassword("root");
//	bool connected = db.open();
//	if (!connected)
//	{
//		QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"),
//								db.lastError().text(), 0, this);
//		msgBox.addButton(tr("OK"), QMessageBox::AcceptRole);
//		msgBox.exec();
////		qDebug() << db.lastError();
//		return;
//	}
////	qDebug() << connected;
//	connectDB();


}

MainWindow::~MainWindow()
{
		delete ui;
		delete distrInfo;
		delete cform;
		delete net;
		delete plotWidget;
		delete hamil_graph;
		delete configWin;
}

void MainWindow::about()
{
	QMessageBox::about(this, trUtf8("О программе"),
				trUtf8("Приложение <b>Structures modeling tool</b> позволяет выполнять эмуляцию нейронных сетей, сформированных плоскостными структурами "));
}

void MainWindow::updateDiagramms()
{
	if (plotWidget) delete plotWidget;
	plotWidget = new plotter;
	if (hamil_graph) delete hamil_graph;
	hamil_graph = new plotter;
	hamil_graph->changeCurve(QwtPlotCurve::Lines);
	if (configWin->drawDistrubution)
		plotWidget->update(distrData);
	if (configWin->drawHamiltonian)
		hamil_graph->update(hamData);
	reLayout();
}

void MainWindow::reLayout()
{
	ui->verticalLayout->addWidget(ui->label_3);
	ui->verticalLayout->addWidget(plotWidget);
	ui->verticalLayout->addWidget(ui->label_4);
	ui->verticalLayout->addWidget(hamil_graph);
	ui->plainTextEdit->hide();
	ui->label_2->hide();
//	ui->verticalLayout->addWidget(ui->label_2);
//	ui->verticalLayout->addWidget(ui->plainTextEdit);
	ui->centralwidget->setLayout(ui->verticalLayout);

	QObject::connect(ui->action3D, SIGNAL(triggered()),this, SLOT(New3dStructure()));
	QObject::connect(ui->action2D, SIGNAL(triggered()),this, SLOT(NewStructure()));
	QObject::connect(ui->actionEdit_stucture, SIGNAL(triggered()),this, SLOT(EditStructure()));
	QObject::connect(ui->actionAbout_QT, SIGNAL(triggered()),qApp, SLOT(aboutQt()));
	QObject::connect(ui->actionAbout, SIGNAL(triggered()),this, SLOT(about()));
	QObject::connect(ui->actionCreate_net_from_structure, SIGNAL(triggered()),this, SLOT(createNet()));
	QObject::connect(ui->actionSave_net_to_file, SIGNAL(triggered()),this, SLOT(saveNet()));
	QObject::connect(ui->actionOpen_net, SIGNAL(triggered()),this, SLOT(openNet()));
	QObject::connect(ui->actionOrdinary_test, SIGNAL(triggered()),this, SLOT(runTest()));
	QObject::connect(ui->actionFast_test, SIGNAL(triggered()),this, SLOT(runFastTest()));
	QObject::connect(ui->actionCreate_inputs,SIGNAL(triggered()),this, SLOT(createInputs()));
	QObject::connect(ui->actionSave_distribution,SIGNAL(triggered()),plotWidget,SLOT(save()));
	QObject::connect(ui->actionSave_Hamiltonian,SIGNAL(triggered()),hamil_graph,SLOT(save()));
	QObject::connect(ui->actionUpdate_diagramm,SIGNAL(triggered()),this,SLOT(updateDiagramms()));
	QObject::connect(ui->actionShow_distribution_info,SIGNAL(triggered()),this,SLOT(showDistribInfo()));
	QObject::connect(ui->actionSettings, SIGNAL(triggered()),configWin,SLOT(show()));
	QObject::connect(ui->actionSave_structure,SIGNAL(triggered()),cform,SLOT(saveImage()));
	QObject::connect(distrInfo,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(giveProto(QTableWidgetItem *)));
//	QObject::connect(protoInfo,SIGNAL(itemSelectionChanged()))
}

void MainWindow::EditStructure()
{
	cform->show();
}

void MainWindow::saveNet() {
	if (!net)
	{
		QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"),
							tr("Neural net was not created"), 0, this);
		msgBox.addButton(tr("OK"), QMessageBox::AcceptRole);
		msgBox.exec();
		return;
	}
	std::ofstream out;
	QString fileName = QFileDialog::getSaveFileName(this,
										 tr("Save as:"),
										 "neural_net.net",
										 tr("Net Files (*.net);;All Files (*)"));
	out.open(fileName.toAscii(), std::ios::out);
	out<<std::scientific<<std::noshowpos;
	net->SaveNet(out);
	out.close();
}

void MainWindow::openNet() {

	QString fileName = QFileDialog::getOpenFileName(this,tr("Open file with net"),".",tr("Net Files (*.net);;All Files (*)"));
	if (fileName.isEmpty()) return;
	if (net) delete net;
	std::vector<net_init> init;
	init.push_back(net_init(1,1,neuron::threshold));
	net=new net_hop(init);
	std::ifstream in;

	in.open(fileName.toAscii(), std::ios::in);
	net->OpenNet(in);
	std::vector<std::vector<double> > syn = net->GiveSynapses();
	ui->tableWidget->setColumnCount(syn.size());
	ui->tableWidget->setRowCount(syn.size());
	for (unsigned i=0;i<syn.size();i++)
		for(unsigned j=0;j<syn[0].size();j++)
		{
		QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(syn[i][j]));
		ui->tableWidget->setItem(i,j,newItem);
		}
	in.close();
}



void MainWindow::runFastTest()
{
	distrData.clear();
	hamData.clear();

	if (!net)
	{
		QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"),
								tr("Network was not created!"), 0, this);
		msgBox.addButton(tr("OK"), QMessageBox::AcceptRole);
		msgBox.exec();
		return;
	}

	if (!connectDB())
		return;

	std::vector<double> outputs;
	std::vector<double> inputs;
	QList<int> in;
	int n=net->GiveSynapses().size();

	std::ofstream outputsFile, distribFile, hamilFile, inputsFile;

	if (configWin->writeToFiles)
	{
		inputsFile.open("inputs.dat", std::ios::out);
		outputsFile.open("outputs.dat",std::ios::out);
		distribFile.open("distrib.dat",std::ios::out);
		hamilFile.open("hamiltonian.dat",std::ios::out);
	}

	QProgressDialog progress("Making distribution...", "Abort", 0, 1<<n, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setValue(0);

	QSqlQuery query(db);
	query.exec("DROP TABLE distrib");
	query.exec("create table distrib (value bigint primary key, count bigint)");
	query.exec(QString("insert into distrib values(%1,%2)").arg(0).arg(0));
	query.exec(QString("insert into distrib values(%1,%2)").arg(1<<n).arg(0));

	for(long long k=0;k<(1<<n);k++)
	{
		for (unsigned i=0;i<n;i++)
			in.push_front((k&(1<<i))>>i);
		foreach(int t, in)
			inputs.push_back((t-0.5)*2);
		in.clear();
		net->SetInputs(inputs);
		net->UpdateInputs();

		net->Recognize();
		outputs=net->GiveOutputs();
		double hamilton = net->GiveHamiltonian();
		if (configWin->drawHamiltonian)
		hamData.push_back(QPointF(k,hamilton));

		if(configWin->writeToFiles)
		{
			for (unsigned i=0;i<outputs.size();i++)
			{
				outputsFile<<outputs[i]<<'\t';
				inputsFile<<inputs[i]<<'\t';
			}
			hamilFile<<hamilton<<std::endl;
			outputsFile<<std::endl;
			inputsFile<<std::endl;
		}

		inputs.clear();

		long long int key=BinArrToDec(outputs);
	//	qDebug()<<QString("select count from distrib where value=%1").arg(key);
		query.exec(QString("select count from distrib where value=%1").arg(key));
	//	qDebug()<<"query size"<<query.size();

		QSqlRecord rec;
		long long int count;
		if (query.next())
		{
			rec=query.record();
			count=rec.value(rec.indexOf("count")).toLongLong();
		}

		if (query.size()==0)
		{
	//		qDebug()<<QString("insert into distrib values(%1,%2)").arg(key).arg(1);
			query.exec(QString("insert into distrib values(%1,%2)").arg(key).arg(1));
		}
		else
		{
//			qDebug()<<count<<"count";
//			qDebug()<<QString("update distrib set count=%2 where value=%1").arg(key).arg(count+1);
			query.exec(QString("update distrib set count=%2 where value=%1").arg(key).arg(count+1));
		}
		progress.setValue(k);
		QCoreApplication::processEvents();
		if (progress.wasCanceled()) return;
	}



	QSqlRecord rec;
	query.exec(QString("select value, count from distrib order by count"));
	while (query.next())
	{
		rec=query.record();
		if (configWin->drawDistrubution)
		distrData.push_back(QPointF(rec.value(rec.indexOf("value")).toLongLong(),
																rec.value(rec.indexOf("count")).toLongLong()
																));
		if (configWin->writeToFiles)
			distribFile<<rec.value(rec.indexOf("value")).toLongLong()<<";\t"<<rec.value(rec.indexOf("count")).toLongLong()<<std::endl;
//			distribFile<<";\t"<<toBinary(rec.value(rec.indexOf("count")).toLongLong())<<std::endl;
	}

	if (configWin->writeToFiles)
	{
		distribFile.close();
		inputsFile.close();
		outputsFile.close();
		hamilFile.close();
	}

	if (configWin->drawDistrubution)
		plotWidget->update(distrData);
	if (configWin->drawHamiltonian)
		hamil_graph->update(hamData);


}


//void MainWindow::runTestWithGivenInputs()
//{
//	if (!net)
//	{
//		QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"),
//								tr("Network was not created!"), 0, this);
//		msgBox.addButton(tr("OK"), QMessageBox::AcceptRole);
//		msgBox.exec();
//		return;
//	}
//	std::ofstream outputsFile, distribFile, hamilFile;

//	if (configWin->writeToFiles)
//	{
//		outputsFile.open("outputs.dat",std::ios::out);
//		distribFile.open("distrib.dat",std::ios::out);
//		hamilFile.open("hamiltonian.dat",std::ios::out);
//	}
//	int n=net->GiveSynapses().size();
//	QProgressDialog progress("Making distribution...", "Abort", 0, 1<<n, this);
//	progress.setWindowModality(Qt::WindowModal);
//	progress.setValue(0);

//	std::ifstream in;
//	std::vector<double> outputs, inputs;

//}

void MainWindow::runTest()
{
	distrData.clear();
	hamData.clear();
	hamil_graph->update(hamData);
	plotWidget->update(distrData);

	if (!net)
	{
		QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"),
								tr("Network was not created!"), 0, this);
		msgBox.addButton(tr("OK"), QMessageBox::AcceptRole);
		msgBox.exec();
		return;
	}

	QMessageBox::StandardButton reply;
	QString inputFileName;
	int n=net->GiveSynapses().size();
	reply = QMessageBox::question(this, tr("QMessageBox::question()"),

                                      trUtf8("Генерировать файл, содержащий все возможные кодовые комбинации?"),
                                                                                         QMessageBox::Yes | QMessageBox::No );
	if (reply == QMessageBox::Yes)
        {inputFileName="inputs.dat";
         QString mask = QInputDialog::getText(this, trUtf8("Введите маску"),trUtf8("Маска"), QLineEdit::Normal,"");
          makeInputFilebyMask(mask.toAscii());;
            //makeInputFile(n);
        }
	else
		inputFileName=QFileDialog::getOpenFileName(this,tr("Open file with inputs"),".",tr("Input data (*.dat);;All Files (*)"));
			if (inputFileName.isEmpty()) return;

	std::map<long long int, long, std::less<long long int> > distrib;
	std::map<long long int, long, std::less<long> >::iterator iter;
	std::ifstream in;
	std::ofstream out, hamilFile;


	std::vector<double> outputs;

//	std::ostringstream buf;
//	buf<<"Running test:\n";

	distrib[0]=0;
	distrib[(1<<n)-1]=0;

	long long k=0;

	QProgressDialog progress("Making distribution...", "Abort", 0, 1<<n, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setValue(0);

	in.open(inputFileName.toAscii(),std::ios::in);
	net->OpenInputsFile(in);
	in.close();
	in.open(inputFileName.toAscii(),std::ios::in);
	if (configWin->writeToFiles)
	{
		out.open("outputs.dat",std::ios::out);
		hamilFile.open("hamiltonian.dat",std::ios::out);
	}
	long long int i=0;

	HtmlResultPrinter htmlPrinter;
	do
		{
			net->LoadNextInput(in);
			net->Recognize();
//			net->Print(buf);
//			qDebug()<<net->GiveHamiltonian();
			outputs=net->GiveOutputs();
			 htmlPrinter.addData(net->giveInputs(),outputs);

			long long int key=BinArrToDec(outputs);
			iter=distrib.find(key);
			if (iter!=distrib.end()) distrib[key]++;
			else distrib[key]=1;
			if (configWin->writeToFiles)
			{
			for (unsigned i=0;i<outputs.size();i++)
				out<<outputs[i]<<'\t';
			out<<std::endl;
			hamilFile<<net->GiveHamiltonian()<<std::endl;
			}
			progress.setValue(k);
			QCoreApplication::processEvents();
			if (progress.wasCanceled()) break;
			if (configWin->drawHamiltonian)
				hamData.push_back(QPointF(k,net->GiveHamiltonian()));
			k++;
		}
	while (in && k<(1<<n));
        htmlPrinter.print("output/out.html");
	out.close();
	hamilFile.close();
	in.close();



	i=0;
	if (configWin->writeToFiles)
		out.open("distrib.dat",std::ios::out);
	for (iter=distrib.begin();iter!=distrib.end();iter++)
	{
		if (configWin->drawDistrubution)
			distrData.push_back(QPoint(iter->first,iter->second));
		if (configWin->writeToFiles)
			out<<iter->first<<";\t"<<iter->second<<std::endl;
		i++;
	}
	out.close();

//	QString message=buf.str().c_str();
//	toLog(message.toLocal8Bit().data());

	if (configWin->drawDistrubution)
		plotWidget->update(distrData);
	if (configWin->drawHamiltonian)
		hamil_graph->update(hamData);
}


void MainWindow::showDistribInfo()
{
	distrInfo->resize(600,300);
	std::sort(distrData.begin(),distrData.end(),mysort);
	distrInfo->setColumnCount(6);
	distrInfo->setRowCount(distrData.size());
	int digitNum = net->GiveSynapses().size();
	QTableWidgetItem * newItem;
	QStringList lst;
	lst<<"Number"<<"Frequency"<<"Binary number"<<"Hamiltonian"<<"Left neighbour"<<"Right neighbour";
	distrInfo->setHorizontalHeaderLabels(lst);
	for (unsigned i=0;i<distrData.size();i++)
	{
		newItem = new QTableWidgetItem(QString("%1").arg(distrData[i].x()));
		distrInfo->setItem(i,0,newItem);
		newItem = new QTableWidgetItem(QString("%1").arg(distrData[i].y()));
		distrInfo->setItem(i,1,newItem);
		newItem = new QTableWidgetItem(QString("%1").arg(toBinary(distrData[i].x(),digitNum)));
		distrInfo->setItem(i,2,newItem);
//		newItem = new QTableWidgetItem(QString("%1").arg(hamData[floor(distrData[i].x())].y()));
//		distrInfo->setItem(i,3,newItem);
//		if (floor(distrData[i].x())>0)
//		{newItem = new QTableWidgetItem(QString("%1").arg(hamData[floor(distrData[i].x())-1].y()));
//			distrInfo->setItem(i,4,newItem);}
//		if (floor(distrData[i].x())<hamData.size()-1)
//		{newItem = new QTableWidgetItem(QString("%1").arg(hamData[floor(distrData[i].x())+1].y()));
//			distrInfo->setItem(i,5,newItem);}
	}
	distrInfo->resizeColumnsToContents();
	distrInfo->resizeRowsToContents();
	distrInfo->setWindowTitle(QString("Distribution information in table form"));
	distrInfo->show();

}

int hammingDistance (long n1, long n2, int digitNum)
{
	int k=0;
	for (unsigned i=0;i<digitNum;i++)
		if (((n1>>i)&1) != ((n2>>i)&1)) k++;
	return k;
}

void MainWindow::giveProto(QTableWidgetItem * item)
{
	int digitNum = net->GiveSynapses().size();
	if (item->column()!=0) return;
	long number = item->text().toLong();

	//new version
	protoInfo->setColumnCount(4);
	protoInfo->setRowCount(distrData.size());
	QTableWidgetItem * newItem;
	QStringList lst;
	lst<<"Number"<<"Binary Number"<<"Hamming distance"<<"Frequency";
	protoInfo->setHorizontalHeaderLabels(lst);

	for (unsigned i=0;i<distrData.size();i++)
	{
	   newItem = new QTableWidgetLongItem(QString("%1").arg(distrData[i].x()));
	   protoInfo->setItem(i,0,newItem);
	   newItem = new QTableWidgetItem(QString("%1").arg(toBinary(distrData[i].x(),digitNum)));
	   protoInfo->setItem(i,1,newItem);
	   newItem = new QTableWidgetLongItem(QString("%1").arg(hammingDistance(number,distrData[i].x(),digitNum)));
	   protoInfo->setItem(i,2,newItem);
	   newItem = new QTableWidgetLongItem(QString("%1").arg(distrData[i].y()));
	   protoInfo->setItem(i,3,newItem);

	}
	protoInfo->sortByColumn(2,Qt::AscendingOrder);
	protoInfo->resize(500,300);
	// end of new version
//	std::vector <QPoint> protos;
//	for (unsigned i=0;i<distrData.size()/2;i++)
//		if (i!=item->row())
//			protos.push_back(QPoint(distrData[i].x(),hammingDistance(number,distrData[i].x(),digitNum)));
//	std::sort(protos.begin(),protos.end(),mysort1);

//	protoInfo = new QTableWidget;
//	protoInfo->setColumnCount(3);
//	protoInfo->setRowCount(distrData.size()/2);
//	QTableWidgetItem * newItem;
//	QStringList lst;

//	lst<<"Binary Number"<<"Hamming distance"<<"Number";
//	protoInfo->setHorizontalHeaderLabels(lst);
//	newItem = new QTableWidgetItem(QString("%1").arg(toBinary(number,digitNum)));
//	protoInfo->setItem(0,0,newItem);
//	for (unsigned i=0;i<distrData.size()/2-1;i++)
//	{
//		newItem = new QTableWidgetItem(QString("%1").arg(toBinary(protos[i].x(),digitNum)));
//		protoInfo->setItem(i+1,0,newItem);
//		newItem = new QTableWidgetItem(QString("%1").arg(protos[i].y()));
//		protoInfo->setItem(i+1,1,newItem);
//		newItem = new QTableWidgetItem(QString("%1").arg(protos[i].x()));
//		protoInfo->setItem(i+1,2,newItem);
//	}
	protoInfo->setWindowTitle(QString("Information about neighbours to %1").arg(toBinary(number,digitNum)));
	protoInfo->resizeColumnsToContents();
	protoInfo->resizeRowsToContents();
	protoInfo->show();

}

void MainWindow::createNet()
	{
		if (net) delete net;
		if (cform->giveNumber()==0)
		{
			QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"),
								tr("Structure hasn't items"), 0, this);
			msgBox.addButton(tr("OK"), QMessageBox::AcceptRole);
			msgBox.exec();
			return;
		}
		std::vector<net_init> init;
		init.push_back(net_init(cform->giveNumber(),cform->giveNumber(),neuron::threshold));
		net=new net_hop(init);
		net->SetSynapses(cform->giveSynapses());
		std::ostringstream out;
		out<<"Net is created!\n";
		net->Print(out);
		std::vector<std::vector<double> > syn = net->GiveSynapses();
//		std::vector<std::vector<double> > syn = cform->giveSynapses();
		ui->tableWidget->setColumnCount(syn.size());
		ui->tableWidget->setRowCount(syn.size());
		for (unsigned i=0;i<syn.size();i++)
			for(unsigned j=0;j<syn[0].size();j++)
			{
			QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(syn[i][j]));
			ui->tableWidget->setItem(i,j,newItem);
			}

		QString message = out.str().c_str();
		toLog(message);

}



void MainWindow::NewStructure()
{
	if (cform) delete cform;
	cform = new structure;
	cform->show();
}
void MainWindow::New3dStructure()
{
	if (cform) delete cform;
	cform = new struct3d;
	cform->show();
}

void MainWindow::createInputs()
{
	bool ok;
	int n = QInputDialog::getInt(this, tr("Input number of binary digits"),
											tr("Number:"), 25, 0, 100, 1, &ok);
	if (!ok) return;
	QString fileName = QFileDialog::getSaveFileName(this,
										 tr("Input file name"),
										 "inputs.dat",
										 tr("Data files (*.dat);;All Files (*)"));
	if (!fileName.isEmpty()) fileName="inputs.dat";

	long long int k=0;
	std::ofstream out;
	out.open(fileName.toAscii(),std::ios::out);
	std::list<int> data;

	for(long long k=0;k<(1<<n);k++)
	{
		for (unsigned i=0;i<n;i++)
			data.push_front((k&(1<<i))>>i);
		for (unsigned i=0;i<n;i++)
		{ out<<(data.front()-0.5)*2<<'\t'; data.pop_front();}
	out<<'\n';
	}
out.close();
}

void MainWindow::toLog(QString message)
{
	QTextCursor cursor(ui->plainTextEdit->textCursor());
	cursor.movePosition(QTextCursor::End);
	cursor.insertText(message);
}




void makeInputFile(long n)
{
	long long int k=0;
	std::ofstream out;
	out.open("inputs.dat",std::ios::out);
	std::list<int> data;

	for(long long k=0;k<(1<<n);k++)
	{
		for (unsigned i=0;i<n;i++)
			data.push_front((k&(1<<i))>>i);
		for (unsigned i=0;i<n;i++)
		{
			out<<(data.front()-0.5)*2;
			if (i<(n-1)) out<<'\t';
			data.pop_front();
		}
	if (k<(1<<n)-1) out<<'\n';
	}
out.close();
}

QString toBinaryString(long n, int digitNum)
{
  QString res;
  int i=0;
  while (n>>i)
  {
          if ((n&(1<<i))>>i)
                  res+="1";
          else res+="0";
          i++;
  }
  QString result;
  for(unsigned i=0;i<res.size();i++)
          result+=res[res.size()-i-1];
  while (result.length()<digitNum)
          result="0"+result;
  result.replace("1", "1\t");
  result.replace("0","-1\t");
  return result;


}

QString toBinary(long long n, int digitNum)
{
	QString res;
	int i=0;
	while (n>>i)
	{
		if ((n&(1<<i))>>i)
			res+="1";
		else res+="0";
		i++;
	}
	QString result;
	for(unsigned i=0;i<res.size();i++)
		result+=res[res.size()-i-1];
	while (result.length()<digitNum)
		result="0"+result;
	return result;
}

bool mysort (const QPointF & d1, const QPointF & d2)
{
	return (d1.y()>d2.y());
}
bool mysort1 (const QPoint & d1, const QPoint & d2)
{
	return (d1.y()<d2.y());
}
