/*
 * neuron.h
 *
 *  Created on: 04.11.2010
 *      Author: serj129
 */
#ifndef NEURON_H_
#define NEURON_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <iomanip>



void setLimit (double limit);
void setAlfa (double alfa);

class layer_b;
class net_hop;
class layer_f;
class net_bp;

class neuron {
friend class net_hop;
friend class net_bp;
public:
	enum Status {ERROR, OK};
	enum sigmoid_type {logistic, hypertan, threshold, custom};
	neuron();
	virtual ~neuron();
	double Axon();
	sigmoid_type TypeOfSigmoid();
	double Ilf();

protected:
	sigmoid_type typeOfSigmoid;
	Status status; //состояние нейрона
	double axon; //выход нейрона
	double ILF; //индуцированное локальное поле нейрона

};

class neuron_f: public neuron {
	friend class net_hop;
protected:
	std::vector <double> synapses;
	std::vector <double *> inputs;
	void _allocate(unsigned numberOfInputs, neuron::sigmoid_type type); //выделение памяти для нейрона
	void _deallocate(); //уничтожение нейрона
public:
	virtual ~neuron_f();
	neuron_f(unsigned numberOfInputs, neuron::sigmoid_type type);
	neuron_f();
	unsigned numberOfInputs();
	void Propagate(); //расчет ILF
	void SetInputs(double * in); //загрузка входов нейрона
	void RandomizeSynapses(int range); //загрузка весов синапсов случайными значениями
	void RandomizeAxon(int range);
	void Sigmoid(); //расчет выхода нейрона axon
	void Init(unsigned numberOfInputs, neuron::sigmoid_type type);
	void Print(std::ostream &out);
};

class neuron_b: public neuron_f {
//friend class layer_b;
friend class net_bp;
protected:
	std::vector <double> deltas; //изменения весов синапсов
	double LocalGradient;	//локальный градиент
//	double error;
	void _deallocate ();
	void _allocate(unsigned numberOfInputs, neuron::sigmoid_type type);
public:
	neuron_b(unsigned numberOfInputs, neuron::sigmoid_type type);
	neuron_b();
	virtual ~neuron_b();
	void Init(unsigned numberOfInputs, neuron::sigmoid_type type);
	void Print(std::ostream &out);
	double SigmoidDerivative ();
	void Correction();
	bool IsConverged ();

};


class layer_f {
friend class net_hop;
private:
	std::vector <neuron_f> neurons;
public:
	layer_f() {neurons.clear();};
	layer_f(unsigned numberOfNeurons,unsigned numberOfInputs, neuron::sigmoid_type type);
	virtual ~layer_f();
	unsigned NumberOfNeurons() {return neurons.size();};
	void RandomizeAxons(int range);
	void RandomizeSynapses(int range);
	void Propagate();
	void SetInputs(double* in);
	void Print(std::ostream & out);
};

class layer_b  {
friend class net_bp;
private:
	std::vector <neuron_b> neurons;
public:
	layer_b() {neurons.clear();};
	layer_b(unsigned numberOfNeurons,unsigned numberOfInputs, neuron::sigmoid_type type);
	virtual ~layer_b();
	void Correction();
	unsigned NumberOfNeurons() {return neurons.size();};
	void RandomizeAxons(int range);
	void RandomizeSynapses(int range);
	void Propagate();
	void SetInputs(double* in);
	void Print(std::ostream & out);
};



class BaseNet {
protected:
	std::vector <double> inputs;
	std::vector <double> outputs;
public:
BaseNet() {};
virtual ~BaseNet() {};
void SetInputs(std::vector<double> & in);
virtual	void OpenNet (std::istream & in){};
virtual	void SaveNet (std::ostream & out){};
virtual void OpenPatternFiles(std::ifstream & in, std::ifstream & out) {};
virtual void LoadNextPatterns (std::ifstream & in, std::ifstream & out) {};
//virtual void OpenFile
//virtual void SetConnections () {};
virtual void Randomize(int range) {};
virtual void Recognize() {};
virtual void CalculateLG() {};
virtual void CorrectSynapses() {};
virtual void OpenInputsFile (std::ifstream & in){};
virtual void LoadNextInput (std::ifstream & in){};
virtual bool IsConverged(){};
virtual void Print(std::ostream & out) {};
virtual void Propagate() =0;
virtual void Learn(std::ifstream &in, std::ifstream & out){};
virtual neuron_f * GiveNeuron(unsigned layerNumber, unsigned neuronNumber) {return NULL;};
//virtual void SetSynapsesFromPatterns (std::ifstream & in){};
virtual std::vector<double> GiveOutputs () {};
virtual std::vector<double> giveInputs () {};
};

struct net_init {
	unsigned numberOfInputs;
	unsigned numberOfNeurons;
	neuron::sigmoid_type typeOfSigmoid;
	net_init( unsigned neurons,	unsigned inputs, neuron::sigmoid_type type):
		numberOfInputs(inputs), numberOfNeurons(neurons), typeOfSigmoid(type){};
	net_init(): numberOfInputs(0), numberOfNeurons(0), typeOfSigmoid(neuron::hypertan){};
};
class net_hop : public BaseNet {
private:
	std::vector<layer_f> layers;

	void SetConnections();
public:
	void UpdateInputs();
	net_hop (std::vector<net_init> initializer);
	virtual ~net_hop();
	void OpenNet (std::istream & in);
	void SaveNet (std::ostream & out);
	void OpenPatternFiles(std::ifstream & in, std::ifstream & out);
	void LoadNextPatterns (std::ifstream & in, std::ifstream & out);
	void OpenInputsFile (std::ifstream & in);
	void LoadNextInput (std::ifstream & in);
	void Randomize(int range);
	void Propagate();
	void Recognize();
	bool IsConverged();
	void Print (std::ostream & out);
	void Learn(std::ifstream &in, std::ifstream & out) {SetSynapsesFromPatterns (in);};
	void SetSynapsesFromPatterns (std::ifstream & in);
	neuron_f * GiveNeuron(unsigned layerNumber, unsigned neuronNumber);
	std::vector<double> GiveOutputs ();
	void SetSynapses(std::vector<std::vector<double> > & syn);
	std::vector<std::vector<double> > GiveSynapses();
	double GiveHamiltonian();
	std::vector<double> giveInputs ();

};

class net_bp : public BaseNet {
private:
	std::vector<layer_b> layers;
	void UpdateInputs();
	void SetConnections();
public:
	net_bp (std::vector<net_init> initializer);
	virtual ~net_bp();
	void OpenNet (std::istream & in);
	void SaveNet (std::ostream & out);
	void OpenPatternFiles(std::ifstream & in, std::ifstream & out);
	void LoadNextPatterns (std::ifstream & in, std::ifstream & out);
	void OpenInputsFile (std::ifstream & in);
	void LoadNextInput (std::ifstream & in);
	void Randomize(int range);
	void Propagate();
	void CorrectSynapses();
	void CalculateLG();
	void Recognize(){Propagate();};
	bool IsConverged();
	void Print (std::ostream & out);
	neuron_f * GiveNeuron(unsigned layerNumber, unsigned neuronNumber){};
	void Learn(std::ifstream &in, std::ifstream & out);
	std::vector<double> GiveOutputs ();


};

#endif /* NEURON_H_ */
