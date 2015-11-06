#ifndef ABSTRACTSTRUCTURE_H
#define ABSTRACTSTRUCTURE_H

class AbstractStructure : public QWidget
{
public:
	AbstractStructure (QWidget * parent=0): QWidget(parent) {};
	virtual std::vector<std::vector<double> > & giveSynapses() {};
	virtual unsigned giveNumber() {};

};




#endif // ABSTRACTSTRUCTURE_H
