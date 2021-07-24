#include "../pch.h"
#include "Neuron.h"

using namespace CrappyNeuralNets;

// constructor:
Neuron::Neuron(const std::string& activation_function)
{
	this->value = NULL;
	
	this->bias = 0.0;
	
	this->activationFunction = activation_function;
	
	this->leakyReLUalfa = 0.25;

	this->activatedValue = NULL;
	
	this->derivedValue = NULL;
	
	this->lossDerivativeWithRespectToActivationFunc = NULL;

	this->droppedOut = false;
}

// mutators:
void Neuron::feed(const std::vector<Neuron*>& previous_layer_neurons, const std::vector<Scalar>& weights)
{
	this->value = 0.0;
	
	for (int i = 0; i < previous_layer_neurons.size(); i++)
		this->value += previous_layer_neurons[i]->getActivatedValue() * weights[i];

	this->value += this->bias;
}

void Neuron::feed(const std::vector<Scalar>& previous_layer_outputs, const std::vector<Scalar>& weights)
{
	this->value = 0.0;

	for (int i = 0; i < previous_layer_outputs.size(); i++) this->value += previous_layer_outputs[i] * weights[i];

	this->value += this->bias;
}

void Neuron::setBias(const Scalar& new_bias_value)
{
	this->bias = new_bias_value;
}

void Neuron::setActivationFunction(const std::string& activation_function)
{
	this->activationFunction = activation_function;
}

void Neuron::activate()
{
	if (this->activationFunction == "linear")
	{
		this->activatedValue = this->value;
		return;
	}

	if (this->activationFunction == "relu")
	{
		this->activatedValue = this->ReLU(this->value);
		return;
	}

	if (this->activationFunction == "leaky relu")
	{
		this->activatedValue = this->leakyReLU(this->value, this->leakyReLUalfa);
		return;
	}

	if (this->activationFunction == "sigmoid")
	{
		this->activatedValue = this->sigmoid(this->value);
		return;
	}

	if (this->activationFunction == "fast sigmoid")
	{
		this->activatedValue = this->fastSigmoid(this->value);
		return;
	}

	if (this->activationFunction == "tanh")
	{
		this->activatedValue = this->tanh(this->value);
		return;
	}

	std::stringstream ss;

	ss << "\nTHERE IS NO ACTIVATION FUNC LIKE '" << this->activationFunction << "' (U PROBABLY MADE A TYPO)\n";
	ss << "USE NeuralNet::getAvailableActivationFunctions() TO CHECK AVAILABLE ACTIVATION FUNCTIONS\n\n";

	ss << "If u want to continue program anyway please press any key...";

	std::cerr << ss.str();

	std::cin.get();
	std::cout << '\n';
}

void Neuron::setActivatedValue(const Scalar& new_activated_value)
{
	this->activatedValue = new_activated_value;
}

void Neuron::derivate()
{
	if (this->activationFunction == "linear")
	{
		this->derivedValue = 1.0;
		return;
	}

	if (this->activationFunction == "relu")
	{
		this->derivedValue = this->derivativeReLU(this->value);
		return;
	}

	if (this->activationFunction == "leaky relu")
	{
		this->derivedValue = this->derivativeLeakyReLU(this->value, this->leakyReLUalfa);
		return;
	}

	if (this->activationFunction == "sigmoid")
	{
		this->derivedValue = this->derivativeSigmoid(this->activatedValue);
		return;
	}

	if (this->activationFunction == "fast sigmoid")
	{
		this->derivedValue = this->derivativeFastSigmoid(this->value);
		return;
	}

	if (this->activationFunction == "tanh")
	{
		this->derivedValue = this->derivativetanh(this->activatedValue);
		return;
	}

	std::stringstream ss;

	ss << "\nTHERE IS NO ACTIVATION FUNC LIKE '" << this->activationFunction << "' (U PROBABLY MADE A TYPO)\n";
	ss << "USE NeuralNet::getAvailableActivationFunctions() TO CHECK AVAILABLE ACTIVATION FUNCTIONS\n\n";
	
	ss << "If u want to continue program anyway please press any key...";

	std::cerr << ss.str();

	std::cin.get();
	std::cout << '\n';
}

void Neuron::setDerivedValue(const Scalar& new_derived_value)
{
	this->derivedValue = new_derived_value;
}

void Neuron::calcLossDerivativeWithRespectToActivationFunc(
	const Scalar& desired_output, 
	const std::string& loss_function)
{
	// add info that only MSE requires number_of_neurons_in_layer (cross-entropy doesn't, but
	// the function always will take that value as the argument so that the code will be clean)

	if (loss_function == "mse")
	{
		this->lossDerivativeWithRespectToActivationFunc = 2.0 * (this->activatedValue - desired_output);
		return;
	}

	if (loss_function == "cross-entropy")
	{
		Scalar epsilon = 10e-32;
		Scalar denominator = this->activatedValue + epsilon;

		this->lossDerivativeWithRespectToActivationFunc = -desired_output / denominator;
		return;
	}
	
	std::stringstream ss;

	ss << "\nTHERE IS NO LOSS FUNCTION LIKE: '" << loss_function << "\n\n";

	ss << "If u want to continue program anyway please press any key...";

	std::cerr << ss.str();

	std::cin.get();
	std::cout << '\n';
}

void Neuron::calcLossDerivativeWithRespectToActivationFunc(
	const std::vector<Neuron*>& next_layer_neurons, 
	const std::vector<Scalar>& transposed_weights)
{
	this->lossDerivativeWithRespectToActivationFunc = 0.0;

	for (int i = 0; i < next_layer_neurons.size(); i++)
	{
		this->lossDerivativeWithRespectToActivationFunc +=
			transposed_weights[i] *
			next_layer_neurons[i]->getDerivatedValue() *
			next_layer_neurons[i]->getLossDerivativeWithRespectToActivationFunc();
	}
}

void Neuron::setLossDerivativeWithRespectToActivationFunc(const Scalar& new_value)
{
	this->lossDerivativeWithRespectToActivationFunc = new_value;
}

void Neuron::dropOut(bool dropped_out)
{
	this->droppedOut = dropped_out;
}

void Neuron::setLeakyReLUalfa(const Scalar& leaky_ReLU_alfa)
{
	this->leakyReLUalfa = leaky_ReLU_alfa;
}

// accessors:
const Scalar& Neuron::getValue() const
{
	return this->value;
}

const Scalar& Neuron::getBias() const
{
	return this->bias;
}

const std::string& Neuron::getActivationFunction() const
{
	return this->activationFunction;
}

const Scalar& Neuron::getActivatedValue() const
{
	return this->activatedValue;
}

const Scalar& Neuron::getDerivatedValue() const
{
	return this->derivedValue;
}

const Scalar& Neuron::getLossDerivativeWithRespectToActivationFunc() const
{
	return this->lossDerivativeWithRespectToActivationFunc;
}

bool Neuron::isDroppedOut() const
{
	return this->droppedOut;
}

const Scalar& Neuron::getLeakyReLUalfa() const
{
	return this->leakyReLUalfa;
}

// activation functions:
Scalar Neuron::ReLU(const Scalar& x) const
{
	return std::max(0.0, x);
}

Scalar Neuron::derivativeReLU(const Scalar& x) const
{
	return x >= 0.0;
}

Scalar Neuron::leakyReLU(const Scalar& x, const Scalar& alfa) const
{
	return std::max(x, x * alfa);
}

Scalar Neuron::derivativeLeakyReLU(const Scalar& x, const Scalar& alfa) const
{
	if (x >= 0.0) return 1.0;
	
	return alfa;
}

Scalar Neuron::sigmoid(const Scalar& x) const
{
	return 1.0 / (1.0 + exp(-x));
}

Scalar Neuron::derivativeSigmoid(const Scalar& sigmoid) const
{
	return sigmoid * (1.0 - sigmoid);
}

Scalar Neuron::fastSigmoid(const Scalar& x) const
{
	return x / (1.0 + abs(x));
}

Scalar Neuron::derivativeFastSigmoid(const Scalar& x) const
{
	return pow((abs(x) + 1.0), -2.0);
}

Scalar Neuron::tanh(const Scalar& x) const
{
	Scalar temp = exp(2 * x);
	return (temp - 1) / (temp + 1);
}

Scalar Neuron::derivativetanh(const Scalar& tanh) const
{
	return 1.0 - pow(tanh, 2.0);
}
