/*
 * neuron.cpp
 *
 *  Created on: 04.11.2010
 *      Author: serj129
 */

#include "neuron.h"
static double LIMIT = 1e-6;
static double ALFA = 1;
static double EDUCATION_SPEED = 0.2;

using namespace std;
void setLimit (double limit)
{if (limit>0) LIMIT = limit; return;}

void setAlfa (double alfa)
{ALFA = alfa; return;}


		neuron::~neuron() {}
		neuron::neuron():ILF(0), axon(0), status(OK){}
double 	neuron::Axon() {return axon;}
neuron::sigmoid_type neuron::TypeOfSigmoid() {return typeOfSigmoid;}
double neuron::Ilf(){return ILF;}



neuron_f::neuron_f(unsigned numberOfInputs, neuron::sigmoid_type type) {
	_allocate(numberOfInputs, type);
}

neuron_f::~neuron_f() {	_deallocate();}

void neuron_f::Propagate() {
	ILF = 0;
	for (unsigned i = 0; i < synapses.size(); i++)
		ILF += synapses[i] * *(inputs[i]);
	Sigmoid();
}

void neuron_f::Sigmoid() {

	switch (typeOfSigmoid) {
	case logistic:	axon = 1 / (1 + exp(-ALFA * this->ILF)); break;
	case hypertan:	axon = tanh(ILF); break;
	case custom: 	axon = 0; break;
	case threshold: {
		if (ILF < 0) axon = -1;	else axon = 1;	break;}
	default:
		axon=0; break;
	}
}
void neuron_f::RandomizeSynapses(int range) {
	for (unsigned i = 0; i < synapses.size(); i++)
		synapses[i] = range * (static_cast<double> (rand()) / RAND_MAX - 0.5);
}

void neuron_f::RandomizeAxon(int range) {
	axon = range * (static_cast<double> (rand()) / RAND_MAX - 0.5);
}

void neuron_f::Init(unsigned numberOfInputs, neuron::sigmoid_type type) {

	_allocate(numberOfInputs, type);
}

unsigned neuron_f::numberOfInputs(){
	return synapses.size();
}
void neuron_f::_allocate(unsigned numberOfInputs, neuron::sigmoid_type type) {
	synapses.clear();
	inputs.clear();
	status = OK;
	typeOfSigmoid = type;
	if (numberOfInputs == 0)return;

	synapses.resize(numberOfInputs, 0.0);
	inputs.resize(numberOfInputs, NULL);
}
void neuron_f::_deallocate() {
	if (status == OK && synapses.size() != 0) {
		synapses.clear();
		inputs.clear();
	}
}
void neuron_f::SetInputs(double * in) {

	//Обработка события совпадения размеров in и inputs
	for (unsigned i = 0; i < synapses.size(); i++)
		inputs[i] = &in[i];
}

void neuron_f::Print(ostream &out) {
	ios_base::fmtflags flags = out.flags();
	out<<scientific<<noshowpos;
	for (unsigned i = 0; i < synapses.size(); i++)
		out << i << ":" << "in: "<<*(inputs[i])<<"\tsyn: "<<synapses[i]<<endl;
	out << " ILF: " << ILF;
	out << "\nAxon: " << axon<<endl;
	out.flags(flags);
}
neuron_f::neuron_f() {
	synapses.clear();
	inputs.clear();
	status = OK;
	typeOfSigmoid = threshold;
}

double neuron_b::SigmoidDerivative() {
//	std::cout<<"type of sigmoid"<<typeOfSigmoid<<endl;
	switch (typeOfSigmoid) {
	case logistic:
		return ALFA * axon * (1 - axon); break;
	case hypertan:
		return 1 - axon * axon;
		break;
	case threshold:
		return 0;
		break;
	case custom:
			return 0;
			break;
	default:
		return 0;
		break;
	};
}

void neuron_b::Print(ostream &out) {
	ios_base::fmtflags flags = out.flags();
	out<<scientific<<noshowpos;
	for (unsigned i = 0; i < synapses.size(); i++)
		out << i << ":" << "in: "<<*(inputs[i])<<"\tsyn: "
		<<synapses[i]<<"\tdelta: "<<deltas[i]<<endl;
	out << " ILF: " << ILF;
	out << "\nAxon: " << axon<<endl;
	out.flags(flags);
}
void neuron_b::_allocate(unsigned numberOfInputs, neuron::sigmoid_type type) {
	deltas.clear();
//	error = 0;
	LocalGradient=0;
	if (numberOfInputs == 0)
		return;
	deltas.resize(numberOfInputs, 0);
}

void neuron_b::Correction() {
	for (unsigned i = 0; i < deltas.size(); i++)
		synapses[i]+=deltas[i];
}
void neuron_b::Init(unsigned numberOfInputs, neuron::sigmoid_type type) {
	neuron_f::Init(numberOfInputs, type);
	deltas.clear();
	_allocate(numberOfInputs, type);

}
neuron_b::neuron_b(unsigned numberOfInputs, neuron::sigmoid_type type) :
	neuron_f(numberOfInputs, type) {
	_allocate(numberOfInputs, type);
//	std::cout<<"neuron_b";

}
neuron_b::neuron_b() {
	deltas.clear();
	LocalGradient = 0;
//	error = 0;
}
bool neuron_b::IsConverged() {
	for (unsigned i = 0; i < deltas.size(); i++)
		if (fabs(deltas[i]) > LIMIT)	return false;
	return true;
}
void neuron_b::_deallocate() {	deltas.clear(); }
neuron_b::~neuron_b() {	_deallocate();}

///////////////////////////////////////////////////////////////////////
layer_f::~layer_f() {neurons.clear();}

layer_f::layer_f(unsigned numberOfNeurons,unsigned numberOfInputs, neuron::sigmoid_type type) {
	neurons.clear();
	for(unsigned i=0; i<numberOfNeurons; i++)
		neurons.push_back(neuron_f(numberOfInputs, type));
}
void layer_f::SetInputs(double *in){
	for (unsigned i=0;i<neurons.size();i++)
			neurons[i].SetInputs(in);
}
void layer_f::Print(std::ostream & out){
	for (unsigned i=0;i<neurons.size();i++)
			{	out<<"\nState of neuron: "<<i<<endl;	neurons[i].Print(out);}
}

void layer_f::Propagate(){
	for (unsigned i=0;i<neurons.size();i++)
			neurons[i].Propagate();
}
void layer_f::RandomizeAxons(int range){
	for (unsigned i=0;i<neurons.size();i++)
			neurons[i].RandomizeAxon(range);
}
void layer_f::RandomizeSynapses(int range){
	for (unsigned i=0;i<neurons.size();i++)
			neurons[i].RandomizeSynapses(range);
}

layer_b::~layer_b() {neurons.clear();}

layer_b::layer_b(unsigned numberOfNeurons,unsigned numberOfInputs, neuron::sigmoid_type type) {
	neurons.clear();
	for(unsigned i=0; i<numberOfNeurons; i++)
		neurons.push_back(neuron_b(numberOfInputs, type));
}

void layer_b::Correction() {
	for (unsigned i=0;i<neurons.size();i++)
	neurons[i].Correction();
}
void layer_b::SetInputs(double *in){
	for (unsigned i=0;i<neurons.size();i++)
			neurons[i].SetInputs(in);
}
void layer_b::Print(std::ostream & out){
	for (unsigned i=0;i<neurons.size();i++)
			{	out<<"\nState of neuron: "<<i<<endl;	neurons[i].Print(out);}
}

void layer_b::Propagate(){
	for (unsigned i=0;i<neurons.size();i++)
			neurons[i].Propagate();
}
void layer_b::RandomizeAxons(int range){
	for (unsigned i=0;i<neurons.size();i++)
			neurons[i].RandomizeAxon(range);
}
void layer_b::RandomizeSynapses(int range){
	for (unsigned i=0;i<neurons.size();i++)
			neurons[i].RandomizeSynapses(range);
}

void BaseNet::SetInputs(std::vector<double> & in) {
	if (inputs.size()==in.size())
		for (unsigned i=0;i<in.size();i++)
			inputs[i]=in[i];
	else std::cerr<<"\nThe sizes are not equal!\n";
}
//////////////////////////////////////////////////////
net_hop::net_hop (std::vector<net_init> initializer) {
	layers.clear();
	for (unsigned i = 0; i<initializer.size(); i++)
		layers.push_back(layer_f(initializer[i].numberOfNeurons,
				initializer[i].numberOfInputs,initializer[i].typeOfSigmoid));
	inputs.clear();
	outputs.clear();
	if (!initializer.empty()) {
			inputs.resize(initializer.front().numberOfNeurons,0);
			outputs.resize(initializer.back().numberOfNeurons,0);
			SetConnections();
	}
}
net_hop::~net_hop() {layers.clear(); inputs.clear(); outputs.clear();}

void net_hop::SetConnections() {
	static std::vector <double *> in;
	in.clear();
//	in.resize(inputs.size(),&inputs[0]);
//	std::vector<double>::iterator t = inputs.begin();
//	for (unsigned i=0;i<inputs.size();i++) std::cout<<inputs[i]<<endl;
	for (unsigned i=0;i<inputs.size();i++) in.push_back( & inputs[i]);
	for (unsigned i = 0; i < layers.size(); i++)
		for (unsigned j=0; j<layers[i].neurons.size(); j++)
		{
//			std::cout<<*in[j]<<endl;
			if (i==0)
				layers[i].neurons[j].axon = *(in[j]);
			for (unsigned k=0;k<layers[i].neurons[j].synapses.size(); k++)
				if (j!=k) layers[i].neurons[j].inputs[k] = & layers[i].neurons[k].axon;
				else layers[i].neurons[j].inputs[k] = in[j];
		}
}
void net_hop::Randomize(int range){
	for (unsigned i = 0; i < layers.size(); i++)
	{
//		layers[i].RandomizeAxons(range);
		layers[i].RandomizeSynapses(range);
	}
}
void net_hop::SetSynapses(std::vector<std::vector<double> > & syn) {
	if (syn.size()==layers[0].neurons.size())
		for (unsigned i=0;i<layers[0].neurons.size();i++)
			for (unsigned j=0;j<layers[0].neurons[i].synapses.size();j++)
				layers[0].neurons[i].synapses[j]=syn[i][j];
	else
		std::cerr<<"\nArray with synapses is not compatible with this network\n";
}

/*
vector <vector<float> > net_hop::GiveSynapses() {
	vector <vector<float> > result;
	vector<float> line;
	for (unsigned i = 0; i < layers[1]->neuron_number; i++)
	{
		for (unsigned j = 0; j < layers[1]->neurons[i]->sin_number; j++)
			line.push_back(layers[1]->neurons[i]->sinapses[j]);
	result.push_back(line);
	line.clear();
	}
return result;
}
 */

std::vector<std::vector<double> > net_hop::GiveSynapses() {
	std::vector <std::vector<double> > result;
	std::vector<double> line;
	for (unsigned i = 0; i < layers[0].neurons.size(); i++)
	{
		for (unsigned j=0;j<layers[0].neurons[i].synapses.size();j++)
			line.push_back(layers[0].neurons[i].synapses[j]);
		result.push_back(line);
		line.clear();
	}
	return result;
}

/*double hamilton (std::vector <float> & in, std::vector <std::vector <float> > & syn) {
	double result=0;
	for (unsigned i=0;i<syn.size();i++)
		for (unsigned j=0;j<syn[0].size();j++)
			if (i!=j)
			result+=in[i]*in[j]*syn[i][j];
	return result;
}*/

double net_hop::GiveHamiltonian() {
	double result=0;
	for (unsigned i=0;i<layers[0].neurons.size();i++)
		for(unsigned j=0;j<layers[0].neurons[i].synapses.size();j++)
			if (i!=j)
				result-=inputs[i]*inputs[j]*layers[0].neurons[i].synapses[j];
	return result;
}

void net_hop::OpenPatternFiles(std::ifstream & in, std::ifstream & out){
	if(in)
		{
			in.seekg(0,ios::beg);
			double temp;
			long count = 0;
			while (in)	{in>>temp; count++;}
			if ((count-1)%inputs.size())
			{
				std::cerr<<"\nFile with patterns is not comply with this neural network\n";
				in.clear(ios::failbit);
				exit(1);
				return;
			}
			in.clear();
			in.seekg(0,ios::beg);
//			std::cout<<in.eof()<<in.fail()<<in.bad()<<in.good();
		}
	else {
		std::cerr<<"\nFailed to read file with patterns\n";
		exit(1);
	}

}
void net_hop::UpdateInputs() {
	if (!layers.empty())
		for (unsigned i=0;i<layers.front().neurons.size();i++)
			layers.front().neurons[i].axon=inputs[i];
}
void net_hop::LoadNextPatterns(std::ifstream & in, std::ifstream & out) {
	if (in)
		{
		for(unsigned i=0; i<inputs.size();i++)

			{
				if (in.eof()) {in.clear(); in.seekg(0,ios::beg);} in>>inputs[i];
//				std::cout<<inputs[i];
			}
		UpdateInputs();
		}
	else { std::cerr<<"\nFailed to read inputs from file with patterns\n"; exit(1);}
}


void net_hop::Propagate() {
	for (unsigned i = 0; i < layers.size(); i++)
		layers[i].Propagate();
}

bool net_hop::IsConverged() {
	static vector <double> previousAxons;
	if (previousAxons.empty())
	{
			for (unsigned i = 0; i < layers.back().neurons.size(); i++)
				previousAxons.push_back(layers.back().neurons[i].axon);
			Propagate();
	}
	for (unsigned i = 0; i < layers.back().neurons.size(); i++)
		if (previousAxons[i]!=layers.back().neurons[i].axon)
		{
			for (unsigned j = 0; j < layers.back().neurons.size(); j++)
			{
				previousAxons[j]=layers.back().neurons[j].axon;
//				std::cout<<previousAxons[j]<<"  ";
			}
			return false;
		}
//	previousAxons.clear();
	return true;
}
neuron_f * net_hop::GiveNeuron(unsigned layerNumber, unsigned neuronNumber) {
	if (layerNumber<layers.size())
		if (neuronNumber<layers[layerNumber].neurons.size()) return & layers[layerNumber].neurons[neuronNumber];
	std::cerr<<"\nCouldn't locate "<<neuronNumber<<"-th neuron in "<<layerNumber<<"-th layer\n";
	exit(1);
}

void net_hop::Recognize() {
//	std::cout<<"recognize";
//	for (unsigned i=0;i<inputs.size();i++)
//		std::cout<<layers[0].neurons[i].axon<<endl;
	while (!IsConverged())
		{
		Propagate();
//		Print(std::cout);
//		std::cout<<"recognize";
		}
}

void net_hop::Print (std::ostream & out) {
	for (unsigned i = 0; i < layers.size(); i++)
	{ out<<"State of layer: "<<i; layers[i].Print(out); }
}

void net_hop::SetSynapsesFromPatterns (std::ifstream & in) {
	if(in)
			{
				in.seekg(0,ios::beg);
				double temp;
				std::vector <double> t;
				std::vector <vector <double> > patterns;
				long count = 0;
				while (in)	{in>>temp; count++;}
				if ((count-1)%inputs.size())
				{
					std::cerr<<"\nFile with patterns is not comply with this neural network\n";
					in.clear(ios::failbit);
					exit(1);
					return;
				}
				in.clear();
				in.seekg(0,ios::beg);
				for (unsigned j=0;j<(count-1)/inputs.size(); j++)
					{
					for (unsigned i=0;i<layers.back().neurons.size();i++)
						{
						in>>temp;
						t.push_back(temp);
//						std::cout<<temp;
						}
					patterns.push_back(t);
					t.clear();
					}

				for (unsigned i=0;i<layers.back().neurons.size();i++)
					for (unsigned j=0;j<layers.back().neurons.size();j++)
					{
						double sum=0;
						if (i!=j)
						{
						for (unsigned k=0;k<patterns.size();k++)
							sum+=patterns[k][i]*patterns[k][j];
						layers.back().neurons[i].synapses[j]=sum;
						}
						else layers.back().neurons[i].synapses[j]=1;
					}

//				for (unsigned i=0;i<patterns.size();i++)
//					for (unsigned j=0;j<patterns[i].size();j++)
//						std::cout<<patterns[i][j];

//				std::cout<<in.eof()<<in.fail()<<in.bad()<<in.good();
			}
		else {
			std::cerr<<"\nFailed to read file with patterns\n";
			exit(1);
		}
}
void net_hop::OpenNet(std::istream & in) {
	if (!layers.empty()) { layers.clear(); inputs.clear(); outputs.clear();}
	if (in)
	{
		unsigned numberOfLayers;
		std::string str;
		in>>numberOfLayers; getline(in, str, '\n');

		for (unsigned i = 0; i<numberOfLayers;i++)
		{
			layer_f Layer;
			unsigned numberOfNeurons;
			in>>numberOfNeurons; getline(in, str, '\n');
			for (unsigned j=0; j<numberOfNeurons;j++)
			{
				unsigned numberOfSynapses;
				double Axon, Ilf;
				in>>numberOfSynapses; getline(in, str, '\n');
				neuron_f Neuron(numberOfSynapses, neuron::threshold);
				for (unsigned k=0; k<numberOfSynapses;k++)
				{
					double Synapse;
					in>>Synapse; getline(in, str, '\n');Neuron.synapses[k]=Synapse;
				}
				in>>Ilf; getline(in, str, '\n');Neuron.ILF=Ilf;
				in>>Axon; getline(in, str, '\n');Neuron.axon=Axon;
				Layer.neurons.push_back(Neuron);
			}
			layers.push_back(Layer);
		}
			unsigned numberOfInputs;
			in>>numberOfInputs; getline(in, str, '\n');
			for (unsigned i = 0; i<numberOfInputs;i++)
			{
				double Input;
				in>>Input; getline(in, str, '\n'); inputs.push_back(Input);
			}
			unsigned numberOfOutputs;
			in>>numberOfOutputs; getline(in, str, '\n');
			for (unsigned i = 0; i<numberOfOutputs;i++)
			{
				double Output;
				in>>Output; getline(in, str, '\n'); outputs.push_back(Output);
			}
			SetConnections();

	}
	else { std::cerr<<"\nFailed to read network configuration from file \n"; exit(1);}
}

void net_hop::SaveNet (std::ostream & out) {
	if (!layers.empty())
		{
		if (out)
		{
			ios_base::fmtflags flags = out.flags();
			out<<scientific<<noshowpos;
			out<<layers.size()<<"\t - number of layers\n";
			for(unsigned i=0;i<layers.size();i++)
				{
					out<<layers[i].neurons.size()<<"\t - number of neurons in "<<i<<"-th layer\n";
					for (unsigned j=0;j<layers[i].neurons.size();j++)
					{
						out<<layers[i].neurons[j].synapses.size()<<"\t - number of synapses in "<<j<<"-th neuron\n";
						for (unsigned k=0;k<layers[i].neurons[j].synapses.size();k++)
							out<<layers[i].neurons[j].synapses[k]<<"\t - value of "<<k<<"-th synapse\n";
						out<<layers[i].neurons[j].ILF<<"\t - value of ILF\n";
						out<<layers[i].neurons[j].axon<<"\t - value of axon\n";
					}
				}
		out<<inputs.size()<<"\t - number of inputs\n";
		for(unsigned i=0;i<inputs.size();i++)
			out<<inputs[i]<<"\t - value of "<<i<<"-th input\n";
		out<<outputs.size()<<"\t - number of outputs\n";
		for(unsigned i=0;i<outputs.size();i++)
				out<<outputs[i]<<"\t - value of "<<i<<"-th output\n";
		out.flags(flags);
		}
		else { std::cerr<<"\nFailed to write network configuration to file \n"; exit(1);}
		}
}

void net_hop::OpenInputsFile (std::ifstream & in){
	if(in)
			{
				in.seekg(0,ios::beg);
				double temp;
				long count = 0;
				while (in)	{in>>temp; count++;}
				if ((count-1)%inputs.size())
				{
					std::cerr<<"\nFile with inputs is not comply with this neural network\n";
					in.clear(ios::failbit);
					exit(1);
					return;
				}
				in.clear();
				in.seekg(0,ios::beg);
//				std::cout<<in.eof()<<in.fail()<<in.bad()<<in.good();
//				std::cout<<in.tellg()<<endl;
			}
		else {
			std::cerr<<"\nFailed to read file with inputs\n";
			exit(1);
			}
		in.close();

}

void net_hop::LoadNextInput (std::ifstream & in) {
	if (in)
	{
		for(unsigned i=0; i<inputs.size();i++)
			in>>inputs[i];
		UpdateInputs();
	}
	else { std::cerr<<"\nFailed to read inputs from file with inputs\n"; exit(1);}
}

std::vector<double> net_hop::GiveOutputs () {
	std::vector<double> out;
	for (unsigned i=0;i<layers.back().neurons.size();i++)
		out.push_back(layers.back().neurons[i].axon);
	return out;
}
std::vector<double> net_hop::giveInputs () {
		return inputs;
}
//////////////////////////////////////////////////////////////////////////////
net_bp::net_bp (std::vector<net_init> initializer) {
	layers.clear();
	for (unsigned i = 0; i<initializer.size(); i++)
		layers.push_back(layer_b(initializer[i].numberOfNeurons,
				initializer[i].numberOfInputs,initializer[i].typeOfSigmoid));
	inputs.clear();
	outputs.clear();
	if (!initializer.empty()) {
			inputs.resize(initializer.front().numberOfNeurons,0);
			outputs.resize(initializer.back().numberOfNeurons,0);
			SetConnections();
	}
}
net_bp::~net_bp() {layers.clear(); inputs.clear(); outputs.clear();}

void net_bp::Print (std::ostream & out) {
	for (unsigned i = 0; i < layers.size(); i++)
	{ out<<"State of layer: "<<i; layers[i].Print(out); }
}
void net_bp::UpdateInputs() {
	if (!layers.empty())
		for (unsigned i=0;i<layers.front().neurons.size();i++)
			layers.front().neurons[i].axon=inputs[i];
}

void net_bp::SetConnections() {
	static std::vector <double *> in;
	in.clear();
	for (unsigned i=0;i<inputs.size();i++) in.push_back( & inputs[i]);
	for (unsigned i = 0; i < layers.size(); i++)
		for (unsigned j=0; j<layers[i].neurons.size(); j++)
		{
			if (i==0)
				layers[i].neurons[j].axon = *(in[j]);
			else
			for (unsigned k=0;k<layers[i].neurons[j].synapses.size(); k++)
				layers[i].neurons[j].inputs[k] = & layers[i-1].neurons[k].axon;
		}
}

void net_bp::Randomize(int range){
	for (unsigned i = 0; i < layers.size(); i++)
	{
		layers[i].RandomizeAxons(range);
		layers[i].RandomizeSynapses(range);
	}
}
void net_bp::Propagate() {
	if (layers.size()>1)
	for (unsigned i = 1; i < layers.size(); i++)
		layers[i].Propagate();
	else {std::cerr<<"\nNeural net hasn't enough number of layers to calculate!\n"; exit(1);}
}

void net_bp::SaveNet (std::ostream & out) {
	if (!layers.empty())
		{
		if (out)
		{
			ios_base::fmtflags flags = out.flags();
			out<<scientific<<noshowpos;
			out<<layers.size()<<"\t - number of layers\n";
			for(unsigned i=0;i<layers.size();i++)
				{
					out<<layers[i].neurons.size()<<"\t - number of neurons in "<<i<<"-th layer\n";
					for (unsigned j=0;j<layers[i].neurons.size();j++)
					{
						out<<layers[i].neurons[j].synapses.size()<<"\t - number of synapses in "<<j<<"-th neuron\n";
						for (unsigned k=0;k<layers[i].neurons[j].synapses.size();k++)
							{
							out<<layers[i].neurons[j].synapses[k]<<"\t - value of "<<k<<"-th synapse\n";
//							out<<layers[i].neurons[j].deltas[k]<<"\t - value of "<<k<<"-th delta\n";
							}
						out<<layers[i].neurons[j].ILF<<"\t - value of ILF\n";
						out<<layers[i].neurons[j].axon<<"\t - value of axon\n";
					}
				}
		out<<inputs.size()<<"\t - number of inputs\n";
		for(unsigned i=0;i<inputs.size();i++)
			out<<inputs[i]<<"\t - value of "<<i<<"-th input\n";
		out<<outputs.size()<<"\t - number of outputs\n";
		for(unsigned i=0;i<outputs.size();i++)
				out<<outputs[i]<<"\t - value of "<<i<<"-th output\n";
		out.flags(flags);
		}
		else { std::cerr<<"\nFailed to write network configuration to file \n"; exit(1);}
		}
}

void net_bp::OpenNet(std::istream & in) {
	if (!layers.empty()) { layers.clear(); inputs.clear(); outputs.clear();}
	if (in)
	{
		unsigned numberOfLayers;
		std::string str;
		in>>numberOfLayers; getline(in, str, '\n');

		for (unsigned i = 0; i<numberOfLayers;i++)
		{
			layer_b Layer;
			unsigned numberOfNeurons;
			in>>numberOfNeurons; getline(in, str, '\n');
			for (unsigned j=0; j<numberOfNeurons;j++)
			{
				unsigned numberOfSynapses;
				double Axon, Ilf;
				in>>numberOfSynapses; getline(in, str, '\n');
				neuron_b Neuron(numberOfSynapses, neuron::hypertan);
//				if (numberOfSynapses)
				for (unsigned k=0; k<numberOfSynapses;k++)
				{
					double Synapse;
					in>>Synapse; getline(in, str, '\n');Neuron.synapses[k]=Synapse;
				}
				in>>Ilf; getline(in, str, '\n');Neuron.ILF=Ilf;
				in>>Axon; getline(in, str, '\n');Neuron.axon=Axon;
				Layer.neurons.push_back(Neuron);
			}
			layers.push_back(Layer);
		}
			unsigned numberOfInputs;
			in>>numberOfInputs; getline(in, str, '\n');
			for (unsigned i = 0; i<numberOfInputs;i++)
			{
				double Input;
				in>>Input; getline(in, str, '\n'); inputs.push_back(Input);
			}
			unsigned numberOfOutputs;
			in>>numberOfOutputs; getline(in, str, '\n');
			for (unsigned i = 0; i<numberOfOutputs;i++)
			{
				double Output;
				in>>Output; getline(in, str, '\n'); outputs.push_back(Output);
			}
			SetConnections();

	}
	else { std::cerr<<"\nFailed to read network configuration from file \n"; exit(1);}
}

void net_bp::OpenPatternFiles(std::ifstream & in, std::ifstream & out){

	if(in && out)
		{
			in.seekg(0,ios::beg);
			out.seekg(0,ios::beg);
			double temp;
			long count1 = 0;
			long count2 = 0;
			while (in)	{in>>temp; count1++;}
			while (out)	{out>>temp; count2++;}
			if ((count1-1)%inputs.size()||(count2-1)%outputs.size() ||
					(count1-1)/inputs.size()!=(count2-1)/outputs.size())
			{
				std::cerr<<"\nFiles with patterns is not comply with this neural network\n";
				in.clear(ios::failbit);
				out.clear(ios::failbit);
				exit(1);
			}
			in.clear();
			out.clear();
			in.seekg(0,ios::beg);
			out.seekg(0,ios::beg);

		}
	else {
		std::cerr<<"\nFailed to read files with patterns\n";
		exit(1);
	}
}

void net_bp::LoadNextPatterns(std::ifstream & in, std::ifstream & out) {
	if (in && out)
	{
		for(unsigned i=0; i<inputs.size();i++)
			{
				if (in.eof()) {in.clear(); in.seekg(0,ios::beg);} in>>inputs[i];
//				std::cout<<inputs[i];

			}
		UpdateInputs();
		for(unsigned i=0; i<outputs.size();i++)
			{
				if (out.eof()) {out.clear(); out.seekg(0,ios::beg);} out>>outputs[i];
//				std::cout<<outputs[i];
			}
	}
	else { std::cerr<<"\nFailed to read inputs or outputs from file with patterns\n"; exit(1);}
}

void net_bp::CalculateLG() {
	for (unsigned j = 0; j < layers.back().neurons.size(); j++)
	{
		layers.back().neurons[j].LocalGradient= (outputs[j]-layers.back().neurons[j].axon)
				*layers.back().neurons[j].SigmoidDerivative();
//		std::cout<<layers.back().neurons[j].LocalGradient<<endl;
//		std::cout<<outputs[j]-layers.back().neurons[j].axon<<endl;
//		std::cout<<layers.back().neurons[j].SigmoidDerivative()<<endl;
	}
	if (layers.size()>1)
	for (unsigned i = layers.size()-2; i >= 1; i--)
		for (unsigned j = 0; j < layers[i].neurons.size(); j++)
		{
			double sum = 0;
			for (unsigned k = 0; k < layers[i + 1].neurons.size(); k++)
			{
				sum += layers[i + 1].neurons[k].LocalGradient
						* layers[i + 1].neurons[k].synapses[j];
			}
			layers[i].neurons[j].LocalGradient
					= layers[i].neurons[j].SigmoidDerivative()* sum;
//			std::cout<<layers[i].neurons[j].LocalGradient<<endl;
		}

}
void net_bp::CorrectSynapses() {
	for (unsigned i = 1; i < layers.size(); i++)
		for (unsigned j = 0; j < layers[i].neurons.size(); j++)
			for (unsigned k = 0; k < layers[i].neurons[j].synapses.size(); k++)
			{
				layers[i].neurons[j].deltas[k] = EDUCATION_SPEED
						* layers[i].neurons[j].LocalGradient
						* layers[i - 1].neurons[k].axon;
				layers[i].neurons[j].synapses[k]
						+= layers[i].neurons[j].deltas[k];

			}
}
bool net_bp::IsConverged() {
	for (unsigned i = 1; i < layers.size(); i++)
		for (unsigned j = 0; j < layers[i].neurons.size(); j++)
			if (layers[i].neurons[j].IsConverged()==0)
				return false;
	return true;
}

void net_bp::Learn(ifstream &in, ifstream & out) {
	clock_t t1 = clock();
	unsigned count = 0;
	Randomize(1);
//	SetConnections();
	OpenPatternFiles(in, out);
//	LoadNextPatterns(in, out);
//	Propagate();

	do {
		LoadNextPatterns(in, out);
		Propagate();
		CalculateLG();
		CorrectSynapses();
		count++;
	} while (!IsConverged());

	std::cout << "\nTime for education:" << (clock() - t1) / CLOCKS_PER_SEC
			* 1000000 << " micros" << endl<< "Number of study operations: " << count<<endl;
}
void net_bp::OpenInputsFile (std::ifstream & in){
	if(in)
			{
				in.seekg(0,ios::beg);
				double temp;
				long count = 0;
				while (in)	{in>>temp; count++;}
				if ((count-1)%inputs.size())
				{
					std::cerr<<"\nFile with inputs is not comply with this neural network\n";
					in.clear(ios::failbit);
					exit(1);
					return;
				}
				in.clear();
				in.seekg(0,ios::beg);
				std::cout<<in.eof()<<in.fail()<<in.bad()<<in.good();
			}
		else {
			std::cerr<<"\nFailed to read file with patterns\n";
			exit(1);
			}
		in.close();

}

void net_bp::LoadNextInput (std::ifstream & in) {
	if (in)
	{
		for(unsigned i=0; i<inputs.size();i++)
			in>>inputs[i];
		UpdateInputs();
	}
		else { std::cerr<<"\nFailed to read inputs from file with inputs\n"; exit(1);}
}


std::vector<double> net_bp::GiveOutputs () {
	std::vector<double> out;
	for (unsigned i=0;i<layers.back().neurons.size();i++)
		out.push_back(layers.back().neurons[i].axon);
	return out;
}
