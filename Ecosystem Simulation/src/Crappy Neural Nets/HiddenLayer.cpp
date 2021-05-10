#include "../stdafx.h"
#include "HiddenLayer.h"

using namespace CrappyNeuralNets;

// constructor:
HiddenLayer::HiddenLayer(
	unsigned neurons_count, 
	const std::string& activation_function, 
	const Scalar& dropout_rate)
{
	assert(activation_function != "softmax");
	// TODO: doesn't support softmax for hidden layers bla bla...

	assert(dropout_rate > 0.0 && dropout_rate <= 1.0);

	this->dropoutRate = dropout_rate;

	this->activationFunction = activation_function;

	for (auto& neuron : this->neurons) delete neuron;

	this->neurons.clear();

	this->neurons.reserve(neurons_count);

	for (int i = 0; i < neurons_count; i++) this->neurons.push_back(new Neuron(activation_function));
	
	/*
	this->leakyReLUalfa = leaky_ReLU_alfa;

	for (auto& neuron : this->neurons) neuron->setLeakyReLUalfa(leaky_ReLU_alfa);
	*/

	this->randomNumbersGenerator = nullptr;
}

// mutators:
void HiddenLayer::setDropoutRate(const Scalar& dropout_rate)
{
	assert(dropout_rate > 0.0 && dropout_rate <= 1.0);

	this->dropoutRate = dropout_rate;
}

void HiddenLayer::input(const std::vector<Neuron*>& previous_layer_neurons, const Matrix* weights)
{
	this->setNewDropout();

	for (int i = 0; i < this->neurons.size(); i++)
	{
		if (this->neurons[i]->isDroppedOut()) this->neurons[i]->setActivatedValue(0.0);

		else
		{
			this->neurons[i]->feed(previous_layer_neurons, weights->getValues()[i]);
			this->neurons[i]->activate();
		}
	}
}

void HiddenLayer::input(const std::vector<Scalar>& previous_layer_outputs, const Matrix* weights)
{
	this->setNewDropout();

	for (int i = 0; i < this->neurons.size(); i++)
	{
		if (this->neurons[i]->isDroppedOut()) this->neurons[i]->setActivatedValue(0.0);

		else
		{
			this->neurons[i]->feed(previous_layer_outputs, weights->getValues()[i]);
			this->neurons[i]->activate();
		}
	}
}

void HiddenLayer::setActivationFunction(const std::string& activation_function)
{
	assert(activation_function != "softmax");
	// TODO: doesn't support softmax for hidden layers bla bla...

	this->activationFunction = activation_function;

	for (auto& neuron : this->neurons) neuron->setActivationFunction(activation_function);
}

void HiddenLayer::backpropErrors(const std::vector<Neuron*>& next_layer_neurons, const Matrix* transposed_weights)
{
	// other of these 2 actions doesn't matter:

	// derivate neurons:
	for (auto& neuron : this->neurons)
	{
		if (neuron->isDroppedOut()) neuron->setDerivedValue(0.0);

		else neuron->derivate();
	}

	// calculate for each neuron derivative of loss function with respect to activated value:
	for (int i = 0; i < this->neurons.size(); i++)
	{
		if (this->neurons[i]->isDroppedOut()) this->neurons[i]->setLossDerivativeWithRespectToActivationFunc(0.0);

		else
		{
			this->neurons[i]->calcLossDerivativeWithRespectToActivationFunc(
				next_layer_neurons,
				transposed_weights->getValues()[i]
			);
		}
	}
}

void HiddenLayer::setBias(unsigned neuron_index, const Scalar& new_bias_value)
{
	this->neurons[neuron_index]->setBias(new_bias_value);
}

/*
void HiddenLayer::setLeakyReLUalfa(const Scalar& leaky_ReLU_alfa)
{
	this->leakyReLUalfa = leaky_ReLU_alfa;
	
	for (auto& neuron : this->neurons) neuron->setLeakyReLUalfa(leaky_ReLU_alfa);
}
*/

void HiddenLayer::setRandomNumbersGenerator(RandomNumbersGenerator& generator)
{
	this->randomNumbersGenerator = &generator;
}

// accessors:
const Scalar& HiddenLayer::getDropoutRate() const
{
	return this->dropoutRate;
}

const std::string& HiddenLayer::getActivationFunction() const
{
	return this->activationFunction;
}

unsigned HiddenLayer::getNeuronsCount() const
{
	return this->neurons.size();
}

const std::vector<Neuron*>& HiddenLayer::getNeurons() const
{
	return this->neurons;
}

const Scalar& HiddenLayer::getLeakyReLUalfa() const
{
	return this->leakyReLUalfa;
}

// private functions:
void HiddenLayer::setNewDropout()
{
	for (auto& neuron : this->neurons) neuron->dropOut(false);

	unsigned alreadyDroppedOut = 0;

	while (alreadyDroppedOut < this->neurons.size() * (1.0 - this->dropoutRate))
	{
		std::pair<unsigned, unsigned> range = { 0U, this->neurons.size() - 1 };

		unsigned randomIndex = this->randomNumbersGenerator->getRandomNumber(range);

		if (this->neurons[randomIndex]->isDroppedOut()) continue;

		this->neurons[randomIndex]->dropOut(true);
		alreadyDroppedOut++;
	}
}
