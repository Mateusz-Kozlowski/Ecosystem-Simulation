#include "../stdafx.h"
#include "OutputLayer.h"

using namespace CrappyNeuralNets;

// constructor:
OutputLayer::OutputLayer(unsigned neurons_count, const std::string& activation_function)
{
	assert(activation_function != "leaky relu");

	this->activationFunction = activation_function;

	this->outputs.resize(neurons_count);

	for (auto& neuron : this->neurons) delete neuron;

	this->neurons.clear();

	this->neurons.reserve(neurons_count);

	for (int i = 0; i < neurons_count; i++) this->neurons.push_back(new Neuron(activation_function));
}

void OutputLayer::copyConstructor(const CrappyNeuralNets::OutputLayer& output_layer)
{
	this->activationFunction = output_layer.activationFunction;
	this->outputs = output_layer.outputs;
	
	this->neurons.resize(output_layer.getNeuronsCount());
	for (int i = 0; i < this->neurons.size(); i++)
	{
		*this->neurons[i] = *output_layer.getNeurons()[i];
	}
}

// mutators:
void OutputLayer::input(const std::vector<Scalar>& previous_layer_outputs, const Matrix* weights)
{
	for (int i = 0; i < this->neurons.size(); i++) this->neurons[i]->feed(previous_layer_outputs, weights->getValues()[i]);

	// activate:
	if (this->activationFunction == "softmax")
	{
		this->softmax();
		return;
	}

	for (auto& neuron : this->neurons) neuron->activate();
}

void OutputLayer::input(const std::vector<Neuron*>& previous_layer_neurons, const Matrix* weights)
{
	for (int i = 0; i < this->neurons.size(); i++) this->neurons[i]->feed(previous_layer_neurons, weights->getValues()[i]);

	// activate:
	if (this->activationFunction == "softmax")
	{
		this->softmax();
		return;
	}

	for (auto& neuron : this->neurons) neuron->activate();
}

void OutputLayer::setActivationFunction(const std::string& activation_function)
{
	this->activationFunction = activation_function;

	for (auto& neuron : this->neurons) neuron->setActivationFunction(activation_function);
}

void OutputLayer::backpropErrors(const std::vector<Scalar>& desired_values, const std::string& loss_function)
{
	// other of these 2 actions doesn't matter:

	// derivate neurons:
	if (this->activationFunction == "softmax") this->calcSoftmaxDerivatives();

	else for (auto& neuron : this->neurons) neuron->derivate();

	// calculate for each neuron derivative of loss function with respect to activated value:
	for (int i = 0; i < this->neurons.size(); i++)
	{
		this->neurons[i]->calcLossDerivativeWithRespectToActivationFunc(
			desired_values[i],
			loss_function
		);
	}
}

void OutputLayer::setBias(unsigned neuron_index, const Scalar& new_bias_value)
{
	this->neurons[neuron_index]->setBias(new_bias_value);
}

// accessors:
const std::string& OutputLayer::getActivationFunction() const
{
	return this->activationFunction;
}

const std::vector<Scalar>& OutputLayer::getOutputs()
{
	for (int i = 0; i < this->neurons.size(); i++) this->outputs[i] = this->neurons[i]->getActivatedValue();

	return this->outputs;
}

unsigned OutputLayer::getNeuronsCount() const
{
	return this->neurons.size();
}

const std::vector<Neuron*>& OutputLayer::getNeurons() const
{
	return this->neurons;
}

Scalar OutputLayer::calcCurrentLoss(const std::vector<Scalar>& desired_values, const std::string& loss_function) const
{
	double lossValue = 0;

	if (loss_function == "mse")
	{
		for (int i = 0; i < this->neurons.size(); i++)
			lossValue += pow(this->neurons[i]->getActivatedValue() - desired_values[i], 2.0);

		return lossValue;
	}

	if (loss_function == "cross-entropy")
	{
		Scalar epsilon = 10e-32;

		for (int i = 0; i < this->neurons.size(); i++)
			lossValue -= desired_values[i] * log(this->neurons[i]->getActivatedValue() + epsilon);

		return lossValue;
	}

	std::stringstream ss;

	ss << "\nTHERE IS NO LOSS FUNCTION LIKE: '" << loss_function << "'\n\n";

	ss << "If u want to continue program anyway please press any key...";

	std::cerr << ss.str();

	std::cin.get();
	std::cout << '\n';
}

// private functions:
void OutputLayer::softmax()
{
	// find max to avoid softmax underflow:
	Scalar maks = this->neurons[0]->getValue();

	for (int i = 1; i < this->neurons.size(); i++) maks = std::max(maks, neurons[i]->getValue());

	// even subtracting maks wouldn't avoid softmax underflow if even maks value starts to be smaller than 1000 
	if (maks <= -1024.0)
	{
		// find how many times maks occurs (necessairly for summing all activated value to 1.0):
		unsigned maksOccurrences = 0U;

		for (auto& neuron : this->neurons) if (neuron->getValue() == maks) maksOccurrences += 1U;

		// set arbitrary values because of softmax underflow:
		Scalar maksOccurrencesScalar = static_cast<Scalar>(maksOccurrences);

		for (auto& neuron : neurons)
		{
			if (neuron->getValue() == maks) neuron->setActivatedValue(1.0 / maksOccurrencesScalar);
			
			else neuron->setActivatedValue(0.0);
		}
	}
	else  // avoid softmax underflow:
	{
		// softmax(z) = softmax(z - c) where c is a constant scalar

		// calc exp(z - maks) for all neurons:
		std::vector<Scalar> exponentials;
		exponentials.reserve(this->neurons.size());

		for (auto& neuron : this->neurons) exponentials.push_back(exp(neuron->getValue() - maks));

		// calc sigma:
		Scalar sigma = 0.0;

		for (auto& it : exponentials) sigma += it;

		// calc softmaxes:
		for (int i = 0; i < this->neurons.size(); i++) this->neurons[i]->setActivatedValue(exponentials[i] / sigma);
	}
}

void OutputLayer::calcSoftmaxDerivatives()
{
	for (auto& neuron : this->neurons)
	{
		Scalar activatedValue = neuron->getActivatedValue();
		neuron->setDerivedValue(activatedValue * (1.0 - activatedValue));
	}
}
