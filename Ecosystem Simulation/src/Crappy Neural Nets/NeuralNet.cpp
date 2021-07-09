#include "../stdafx.h"
#include "NeuralNet.h"

using namespace CrappyNeuralNets;

// constructor/destructor:
NeuralNet::NeuralNet()
{
	this->inputLayer = nullptr;
	this->outputLayer = nullptr;
	this->randomWeightsRange = { -1.0, 1.0 };
	this->string = "";
	this->isCompiled = false;
}

NeuralNet::~NeuralNet()
{
	delete this->inputLayer;
	
	for (auto& hiddenLayer : this->hiddenLayers) delete hiddenLayer;
	
	delete this->outputLayer;

	this->deallocateAllMemoryAssociatedWithWeights();
}

void CrappyNeuralNets::NeuralNet::copyConstructor(const CrappyNeuralNets::NeuralNet& neural_net)
{
	*this->inputLayer = *neural_net.inputLayer;
	
	this->hiddenLayers.resize(neural_net.getHiddenLayers().size());
	for (int i = 0; i < neural_net.getHiddenLayers().size(); i++)
	{
		this->hiddenLayers[i]->copyConstructor(*neural_net.getHiddenLayers()[i]);
	}
	
	this->outputLayer->copyConstructor(*neural_net.getOutputLayer());

	this->weights.resize(neural_net.getWeights().size());
	for (int i = 0; i < neural_net.getWeights().size(); i++) 
		*this->weights[i] = *neural_net.getWeights()[i];

	this->transposedWeights.resize(neural_net.getTransposedWeights().size());
	for (int i = 0; i < neural_net.getTransposedWeights().size(); i++) 
		*this->transposedWeights[i] = *neural_net.getTransposedWeights()[i];

	this->weightsGradient.resize(neural_net.getWeightsGradient().size());
	for (int i = 0; i < neural_net.getWeightsGradient().size(); i++)
		*this->getWeightsGradient()[i] = *neural_net.getWeightsGradient()[i];
	
	this->biasesGradient = neural_net.biasesGradient;
	this->randomWeightsRange = neural_net.randomWeightsRange;
	this->string = neural_net.string;
	this->isCompiled = neural_net.isCompiled;
}

// public methods:
void NeuralNet::initInputLayer(InputLayer* input_layer)
{
	assert(input_layer != nullptr);

	// TODO: add info about deallocation

	if (this->inputLayer) delete this->inputLayer;

	this->inputLayer = input_layer;

	this->isCompiled = false;
}

void NeuralNet::addHiddenLayer(HiddenLayer* hidden_layer)
{
	assert(hidden_layer != nullptr);

	this->hiddenLayers.push_back(hidden_layer);

	this->isCompiled = false;
}

void NeuralNet::initOutputLayer(OutputLayer* output_layer)
{
	assert(output_layer != nullptr);

	// TODO: add info about deallocation
	
	delete this->outputLayer;

	this->outputLayer = output_layer;

	this->isCompiled = false;
}

void NeuralNet::compile()
{
	// sets all weights to random values resets gradients
	
	assert(this->inputLayer != nullptr && this->outputLayer != nullptr);

	this->initRandomWeights();
	
	this->initBiasesGradient();

	this->isCompiled = true;
}

bool NeuralNet::loadFromFile(const std::string& file_path)
{
	std::ifstream file(file_path);

	if (!file.good())
	{
		std::stringstream ss;

		ss << "\nERROR: void NeuralNet::loadFromFile(const char* path) const: CANNOT OPEN: " << file_path << "\n\n";

		ss << "If u want to continue program anyway please press any key...";

		std::cerr << ss.str();

		std::cin.get();
		std::cout << '\n';

		return false;
	}

	// topology:
	unsigned layers_count;
	file >> layers_count;

	// deallocate layers memory:
	//delete this->inputLayer; // initInputLayer function will deallocate memory

	for (auto& hiddenLayer : this->hiddenLayers) if (hiddenLayer) delete hiddenLayer;

	this->hiddenLayers.clear();

	//delete this->outputLayer; // initOutputLayer function will deallocate memory
	
	// create new architecture:
	for (int i = 0; i < layers_count; i++)
	{
		if (!i)
		{
			unsigned size;
			double dropoutRate;

			file >> size;
			file >> dropoutRate;

			this->initInputLayer(new InputLayer(size, dropoutRate));

			continue;
		}

		if (i == layers_count - 1U)
		{
			unsigned size;
			std::string activationFunction;

			file >> size;
			getline(file, activationFunction); // avoid weird "bug"/"issue"? while reading from file
			getline(file, activationFunction);

			this->initOutputLayer(new OutputLayer(size, activationFunction));

			continue;
		}

		unsigned size;
		double dropoutRate;
		std::string activationFunction;

		file >> size;
		file >> dropoutRate;
		getline(file, activationFunction); // avoid weird "bug"/"issue"? while reading from file
		getline(file, activationFunction);

		this->addHiddenLayer(new HiddenLayer(size, activationFunction, dropoutRate));
	}
	
	// weights:
	this->deallocateAllMemoryAssociatedWithWeights();

	std::vector<unsigned> topology = this->getCurrentTopology();

	this->weights.clear();
	this->weights.reserve(topology.size() - 1);

	this->transposedWeights.clear();
	this->transposedWeights.reserve(topology.size() - 1);

	this->weightsGradient.clear();
	this->weightsGradient.reserve(topology.size() - 1);

	for (size_t i = 0; i < topology.size() - 1; i++)
	{
		this->weights.emplace_back(new Matrix({ topology[i + 1], topology[i] }));

		this->weightsGradient.emplace_back(new Matrix({ topology[i + 1], topology[i] }));

		for (int j = 0; j < topology[i + 1]; j++)
		{
			for (int k = 0; k < topology[i]; k++)
			{
				Scalar weight;
				file >> weight;
				this->weights[i]->setValue({ j, k }, weight);
				this->weightsGradient[i]->setValue({ j, k }, 0.0);
			}
		}

		Matrix* T = new Matrix(*this->weights[i]);
		T->transpose();
		this->transposedWeights.emplace_back(T);
	}

	file.close();

	this->initBiasesGradient();
	
	this->isCompiled = true;

	return true;
}

const std::string& NeuralNet::toString()
{
	std::stringstream ss;

	// input layer:
	if (this->inputLayer == nullptr) ss << "input layer: not initialized (nullptr)\n\n";

	else
	{
		ss << "input layer:\n\t";
		ss << std::to_string(this->inputLayer->getSize()) << " neuron";

		if (this->inputLayer->getSize() > 1U) ss << 's';

		ss << "\n\tdropout rate: " << this->inputLayer->getDropoutRate() << '\n';
	}

	// hidden layers:
	for (int i = 0; i < this->hiddenLayers.size(); i++)
	{
		if (!i) ss << "1st hidden layer:\n\t";

		else if (i == 1) ss << "2nd hidden layer:\n\t";

		else if (i == 2) ss << "3rd hidden layer:\n\t";

		else ss << std::to_string(i) << "st hidden layer:\n\t";

		ss << this->hiddenLayers[i]->getNeuronsCount() << " neuron";

		if (this->hiddenLayers[i]->getNeuronsCount() > 1U) ss << 's';

		ss << "\n\tdropout rate: " << this->hiddenLayers[i]->getDropoutRate() << '\n';

		ss << "\tactivation function: " << this->hiddenLayers[i]->getActivationFunction() << '\n';
	}

	// output layer:
	if (this->outputLayer == nullptr) ss << "output layer: not initialized (nullptr)\n\n";

	else
	{
		ss << "output layer:\n\t";
		ss << std::to_string(this->outputLayer->getNeuronsCount()) << " neuron";

		if (this->outputLayer->getNeuronsCount() > 1U) ss << 's';

		ss << "\n\tactivation function: " << this->outputLayer->getActivationFunction() << '\n';
	}

	this->string = ss.str();

	return this->string;
}

void NeuralNet::train(
	std::vector<std::vector<Scalar>>& inputs, 
	std::vector<std::vector<Scalar>>& desired_outputs, 
	unsigned epochs_count,
	const std::string& loss_function,
	unsigned mini_batch_size,
	const Scalar& learning_rate,
	bool logs)
{
	assert(this->isCompiled);
	assert(inputs.size() == desired_outputs.size());
	assert(epochs_count >= 1U);
	assert(mini_batch_size <= inputs.size());
	assert(learning_rate > 0.0);
	
	if (!mini_batch_size) mini_batch_size = inputs.size();

	for (int i = 0; i < epochs_count; i++)
	{
		if (logs) std::cout << "Epoch " << i + 1 << "/" << epochs_count << "\n\n";
		
		this->learningEpoch(inputs, desired_outputs, loss_function, mini_batch_size, learning_rate, logs);
	}
}

void NeuralNet::train(
	std::vector<std::vector<Scalar>>& inputs, 
	std::vector<std::vector<Scalar>>& desired_outputs, 
	const std::vector<std::vector<Scalar>>& validation_data_inputs, 
	const std::vector<std::vector<Scalar>>& validation_data_outputs, 
	unsigned epochs_count,
	const std::string& loss_function,
	unsigned mini_batch_size,
	const Scalar& learning_rate,
	bool logs)
{
	// TODO: add info, that validation works only for classification problems, 
	//		 when the result is an index of the neuron with the maximum value

	assert(this->isCompiled);
	assert(inputs.size() == desired_outputs.size());
	assert(validation_data_inputs.size() == validation_data_outputs.size());
	assert(epochs_count >= 1U);
	assert(mini_batch_size <= inputs.size());
	assert(learning_rate > 0.0);

	if (!mini_batch_size) mini_batch_size = inputs.size();

	for (int i = 0; i < epochs_count; i++)
	{
		if (logs) std::cout << "Epoch " << i + 1 << "/" << epochs_count << ":\n";

		this->learningEpoch(inputs, desired_outputs, loss_function, mini_batch_size, learning_rate, logs);

		Scalar accuracy = this->validateClassification(validation_data_inputs, validation_data_outputs);

		std::cout << "accuracy (on validation data): " << accuracy << "\n\n";
	}
}

const std::vector<Scalar>& NeuralNet::predict(const std::vector<Scalar>& inputs)
{
	// set all dropout rates to 1.0 (only for a while):
	std::vector<double> dropoutRates = this->getDropoutRates();
	this->setAllDropoutRatesToOne();

	// prediction:
	this->feedForward(inputs);

	// set dropout rates to the previous state:
	this->setDropouts(dropoutRates);

	return this->outputLayer->getOutputs();
}

bool NeuralNet::saveCurrentOutputsToFile(const std::string& file_path, const std::string& delimiter) const
{
	// if file_path contains a directory (or directories) (for example "dir1//file.ini")
	// then we have to initialize it/them:
	std::string directoryPath = this->extractDirectoryPath(file_path);

	if (directoryPath != "") std::filesystem::create_directories(directoryPath);
	
	// if it/they had already existed then nothing bad will happen

	std::ofstream file(file_path);
		
	if (!file.good())
	{
		std::stringstream ss;

		ss << "\nERROR: void NeuralNet::saveCurrentOutputsToFile(const char* path, const char* delimiter) const: ";
		ss << "THERE IS STH WRONG WITH THIS FILE : " << file_path << "\n\n";

		ss << "If u want to continue program anyway please press any key...";
		
		std::cerr << ss.str();
		
		std::cin.get();
		std::cout << '\n';

		return false;
	}

	for (auto& output : this->outputLayer->getOutputs()) file << output << delimiter;

	file.close();

	return true;
}

bool NeuralNet::saveToFile(const std::string& file_path) const
{
	// if file_path contains a directory (or directories) (for example "dir1//file.ini")
	// then we have to initialize it/them:
	std::string directoryPath = this->extractDirectoryPath(file_path);

	if (directoryPath != "") std::filesystem::create_directories(directoryPath);
	
	// if it/they had already existed then nothing bad will happen

	std::ofstream file(file_path);

	if (!file.good())
	{
		std::stringstream ss;
		
		ss << "\nERROR: void NeuralNet::saveToFile(const char* path) const: CANNOT OPEN: " << file_path << "\n\n";

		ss << "If u want to continue program anyway please press any key...";

		std::cerr << ss.str();

		std::cin.get();
		std::cout << '\n';

		return false;
	}

	std::stringstream ss;

	// topology:
	ss << this->hiddenLayers.size() + 2 << '\n';

	// input layer:
	ss << this->inputLayer->getSize() << '\n';

	ss << this->inputLayer->getDropoutRate() << '\n';

	// hidden layers:
	for (auto& hiddenLayer : this->hiddenLayers)
	{
		ss << hiddenLayer->getNeuronsCount() << '\n';
		ss << hiddenLayer->getDropoutRate() << '\n';
		ss << hiddenLayer->getActivationFunction() << '\n';
	}

	// output layer:
	ss << this->outputLayer->getNeuronsCount() << '\n';

	ss << this->outputLayer->getActivationFunction();

	// weights:
	for (auto& matrix : this->weights)
		for (auto& v : matrix->getValues())
			for (auto& c : v) ss << '\n' << c;

	file << ss.str();

	file.close();

	return true;
}

void NeuralNet::randomMutate(const Scalar& mutation_percentage)
{
	// weights:
	for (int i = 0; i < this->weights.size(); i++)
	{
		for (int j = 0; j < this->weights[i]->getDimensions().first; j++)
		{
			for (int k = 0; k < this->weights[i]->getDimensions().second; k++)
			{
				// to get value in range <-mutation_percentage; mutation_percentage>:
				std::pair<Scalar, Scalar> range = { -1.0, 1.0 };

				Scalar mutation = mutation_percentage * RandomNumbersGenerator::getRandomNumber(range);

				this->weights[i]->setValue(
					{ j, k },
					this->weights[i]->getValue({ j, k }) + this->weights[i]->getValue({ j, k }) * mutation / 100.0
				);

				this->transposedWeights[i]->setValue(
					{ k, j },
					this->transposedWeights[i]->getValue({ k, j }) + 
					this->transposedWeights[i]->getValue({ k, j }) * mutation / 100.0
				);
			}
		}
	}

	// biases:

	// hidden layers:
	for (auto& hiddenLayer : this->hiddenLayers)
	{
		for (int i = 0; i < hiddenLayer->getNeuronsCount(); i++)
		{
			// to get value in range <-mutation_percentage; mutation_percentage>:
			std::pair<Scalar, Scalar> range = { -1.0, 1.0 };

			Scalar mutation = mutation_percentage * RandomNumbersGenerator::getRandomNumber(range);

			hiddenLayer->setBias(
				i,
				hiddenLayer->getNeurons()[i]->getBias() +
				hiddenLayer->getNeurons()[i]->getBias() * mutation / 100.0
			);
		}
	}

	// output layer:
	for (int i = 0; i < this->outputLayer->getNeuronsCount(); i++)
	{
		// to get value in range <-mutation_percentage; mutation_percentage>:
		std::pair<Scalar, Scalar> range = { -1.0, 1.0 };

		Scalar mutation = mutation_percentage * RandomNumbersGenerator::getRandomNumber(range);

		this->outputLayer->setBias(
			i,
			this->outputLayer->getNeurons()[i]->getBias() +
			this->outputLayer->getNeurons()[i]->getBias() * mutation / 100.0
		);
	}
}

// architecture accessors:
const InputLayer* NeuralNet::getInputLayer() const
{
	return this->inputLayer;
}

const std::vector<HiddenLayer*>& NeuralNet::getHiddenLayers() const
{
	return this->hiddenLayers;
}

const OutputLayer* NeuralNet::getOutputLayer() const
{
	return this->outputLayer;
}

const std::vector<Matrix*>& NeuralNet::getWeights() const
{
	return this->weights;
}

const std::vector<Matrix*>& NeuralNet::getTransposedWeights() const
{
	return this->transposedWeights;
}

const std::vector<Matrix*>& NeuralNet::getWeightsGradient() const
{
	return this->weightsGradient;
}

const std::vector<std::vector<Scalar>>& NeuralNet::getBiasesGradient() const
{
	return this->biasesGradient;
}

// private methods:

// architecture:
void NeuralNet::initRandomWeights()
{
	// init weights and weights gradient

	this->deallocateAllMemoryAssociatedWithWeights();

	std::vector<unsigned> topology = this->getCurrentTopology();

	this->weights.clear();
	this->weights.reserve(topology.size() - 1);

	this->transposedWeights.clear();
	this->transposedWeights.reserve(topology.size() - 1);

	this->weightsGradient.clear();
	this->weightsGradient.reserve(topology.size() - 1);

	for (size_t i = 0; i < topology.size() - 1; i++)
	{
		this->weights.push_back(new Matrix({ topology[i + 1], topology[i] }));
		this->weights[i]->setRandomValues(this->randomWeightsRange);

		Matrix* T = new Matrix(*this->weights[i]);
		T->transpose();
		this->transposedWeights.push_back(T);

		this->weightsGradient.emplace_back(new Matrix({ topology[i + 1], topology[i] }));
		this->weightsGradient[i]->setValues(0.0);
	}
}

void NeuralNet::initBiasesGradient()
{
	std::vector<unsigned> topology = this->getCurrentTopology();

	this->biasesGradient.resize(topology.size() - 1);

	for (size_t i = 1; i < topology.size(); i++) this->biasesGradient[i - 1].resize(topology[i]);
}

void NeuralNet::deallocateAllMemoryAssociatedWithWeights()
{
	for (auto& matrix : this->weights) delete matrix;

	for (auto& matrix : this->transposedWeights) delete matrix;

	for (auto& matrix : this->weightsGradient) delete matrix;
}

std::vector<unsigned> NeuralNet::getCurrentTopology() const
{
	std::vector<unsigned> topology;

	topology.push_back(this->inputLayer->getSize());

	for (auto& hiddenLayer : this->hiddenLayers) topology.push_back(hiddenLayer->getNeuronsCount());

	topology.push_back(this->outputLayer->getNeuronsCount());

	return topology;
}

// learning:
void NeuralNet::learningEpoch(
	std::vector<std::vector<Scalar>>& inputs, 
	std::vector<std::vector<Scalar>>& desired_outputs,
	const std::string& loss_function,
	unsigned mini_batch_size,
	const Scalar& learning_rate,
	bool logs)
{
	// TODO: add info that the function will make random shuffling
	// TODO: add the info above to train function also

	this->randomShuffle(inputs, desired_outputs);

	Scalar averageLoss = 0.0;

	for (int i = 0; i < inputs.size(); i++)
	{
		this->feedForward(inputs[i]);

		this->backpropagateErrors(desired_outputs[i], loss_function);

		this->updateWeightsGradient();

		this->updateBiasesGradient();

		if (!((i + 1) % mini_batch_size) || i == inputs.size() - 1)
		{
			this->updateWeights(learning_rate, mini_batch_size);
			this->updateBiases(learning_rate, mini_batch_size);
		}

		if (logs) averageLoss += this->outputLayer->calcCurrentLoss(desired_outputs[i], loss_function);
	}

	if (logs) std::cout << "average loss: " << averageLoss / inputs.size() << '\n';
}

void NeuralNet::feedForward(const std::vector<Scalar>& inputs)
{
	// input layer:
	this->inputLayer->input(inputs);

	// hidden layers:
	for (size_t i = 0; i < this->hiddenLayers.size(); i++)
	{
		if (!i) this->hiddenLayers[i]->input(this->inputLayer->output(), this->weights[0]);

		else this->hiddenLayers[i]->input(this->hiddenLayers[i - 1]->getNeurons(), this->weights[i]);
	}

	// output layer:
	if (this->hiddenLayers.size() > 0)
	{
		this->outputLayer->input(
			this->hiddenLayers[this->hiddenLayers.size() - 1]->getNeurons(),
			this->weights[this->weights.size() - 1]
		);
	}
	else
	{
		this->outputLayer->input(this->inputLayer->output(), this->weights[0]);
	}
}

void NeuralNet::backpropagateErrors(
	const std::vector<Scalar>& desired_outputs, 
	const std::string& loss_function)
{
	// output layer:
	this->outputLayer->backpropErrors(desired_outputs, loss_function);

	// hidden layers:
	for (int i = this->hiddenLayers.size() - 1; i >= 0; i--)
	{
		if (i == this->hiddenLayers.size() - 1)
		{
			this->hiddenLayers[this->hiddenLayers.size() - 1]->backpropErrors(
				this->outputLayer->getNeurons(),
				this->transposedWeights[transposedWeights.size() - 1]
			);
		}
		else
		{
			this->hiddenLayers[i]->backpropErrors(
				this->hiddenLayers[i + 1]->getNeurons(),
				this->transposedWeights[i + 1]
			);
		}
	}
}

void NeuralNet::updateWeightsGradient()
{
	Scalar deltaWeight;

	// if the net doesn't contain any hidden layer then update weights between input and output layer:
	if (!this->hiddenLayers.size())
	{
		unsigned inputLayerSize = this->inputLayer->getSize();
		unsigned outputLayerNeuronsCount = this->outputLayer->getNeuronsCount();

		for (int i = 0; i < inputLayerSize; i++)
		{
			Scalar input = this->inputLayer->output()[i];

			for (int j = 0; j < outputLayerNeuronsCount; j++)
			{
				deltaWeight =
					input *
					this->outputLayer->getNeurons()[j]->getDerivatedValue() *
					this->outputLayer->getNeurons()[j]->getLossDerivativeWithRespectToActivationFunc();

				this->weightsGradient[0]->setValue(
					{ j, i },
					this->weightsGradient[0]->getValue({ j, i }) + deltaWeight
				);
			}
		}

		return;
	}

	// update weights between last hidden layer and output layer:
	unsigned lastHiddenLayerNeuronsCount = this->hiddenLayers[this->hiddenLayers.size() - 1]->getNeuronsCount();
	unsigned outputLayerNeuronsCount = this->outputLayer->getNeuronsCount();

	for (int i = 0; i < lastHiddenLayerNeuronsCount; i++)
	{
		Scalar activated = this->hiddenLayers[this->hiddenLayers.size() - 1]->getNeurons()[i]->getActivatedValue();

		for (int j = 0; j < outputLayerNeuronsCount; j++)
		{
			deltaWeight =
				activated *
				this->outputLayer->getNeurons()[j]->getDerivatedValue() *
				this->outputLayer->getNeurons()[j]->getLossDerivativeWithRespectToActivationFunc();

			this->weightsGradient[this->weights.size() - 1]->setValue(
				{ j, i },
				this->weightsGradient[this->weights.size() - 1]->getValue({ j, i }) + deltaWeight
			);
		}
	}

	// update weights between hidden layers:
	for (size_t k = this->hiddenLayers.size() - 1; k > 0; k--)
	{
		unsigned hiddenLayerNeuronsCount1 = this->hiddenLayers[k - 1]->getNeuronsCount();
		unsigned hiddenLayerNeuronsCount2 = this->hiddenLayers[k]->getNeuronsCount();

		for (int i = 0; i < hiddenLayerNeuronsCount1; i++)
		{
			Scalar activated = this->hiddenLayers[k - 1]->getNeurons()[i]->getActivatedValue();

			for (int j = 0; j < hiddenLayerNeuronsCount2; j++)
			{
				deltaWeight =
					activated *
					this->hiddenLayers[k]->getNeurons()[j]->getDerivatedValue() *
					this->hiddenLayers[k]->getNeurons()[j]->getLossDerivativeWithRespectToActivationFunc();

				this->weightsGradient[k]->setValue(
					{ j, i },
					this->weightsGradient[k]->getValue({ j, i }) + deltaWeight
				);
			}
		}
	}

	// update weights between input layer and first hidden layer:
	unsigned inputLayerSize = this->inputLayer->getSize();
	unsigned firstHiddenLayerNeuronsCount = this->hiddenLayers[0]->getNeuronsCount();

	for (unsigned i = 0; i < inputLayerSize; i++)
	{
		Scalar input = this->inputLayer->output()[i];

		for (unsigned j = 0; j < firstHiddenLayerNeuronsCount; j++)
		{
			deltaWeight =
				input *
				this->hiddenLayers[0]->getNeurons()[j]->getDerivatedValue() *
				this->hiddenLayers[0]->getNeurons()[j]->getLossDerivativeWithRespectToActivationFunc();

			this->weightsGradient[0]->setValue(
				{ j, i },
				this->weightsGradient[0]->getValue({ j, i }) + deltaWeight
			);
		}
	}
}

void NeuralNet::updateBiasesGradient()
{
	// hidden layers:
	if (this->hiddenLayers.size() > 0)
	{
		for (size_t i = 0; i < this->hiddenLayers.size(); i++)
		{
			for (int j = 0; j < this->hiddenLayers[i]->getNeuronsCount(); j++)
			{
				this->biasesGradient[i][j] +=
					this->hiddenLayers[i]->getNeurons()[j]->getDerivatedValue() *
					this->hiddenLayers[i]->getNeurons()[j]->getLossDerivativeWithRespectToActivationFunc();
			}
		}
	}

	// output layer:
	for (int i = 0; i < this->outputLayer->getNeuronsCount(); i++)
	{
		this->biasesGradient[this->biasesGradient.size() - 1][i] +=
			this->outputLayer->getNeurons()[i]->getDerivatedValue() *
			this->outputLayer->getNeurons()[i]->getLossDerivativeWithRespectToActivationFunc();
	}
}

void NeuralNet::updateWeights(const Scalar& learning_rate, const Scalar& mini_batch_size)
{
	for (int i = 0; i < this->weights.size(); i++)
	{
		for (int j = 0; j < this->weights[i]->getDimensions().first; j++)
		{
			for (int k = 0; k < this->weights[i]->getDimensions().second; k++)
			{
				this->weights[i]->setValue(
					{ j, k },
					this->weights[i]->getValue({ j, k }) - 
					learning_rate * this->weightsGradient[i]->getValue({ j, k }) / mini_batch_size
				);
			}
		}
	}

	// update transposed weights:
	for (int i = 0; i < this->weights.size(); i++)
	{
		delete this->transposedWeights[i];

		Matrix* T = new Matrix(*this->weights[i]);
		T->transpose();
		this->transposedWeights[i] = T;
	}

	// reset weights gradient:
	for (auto& matrix : this->weightsGradient) matrix->setValues(0.0);
}

void NeuralNet::updateBiases(const Scalar& learning_rate, const Scalar& mini_batch_size)
{
	// hidden layers:
	for (int i = 0; i < this->hiddenLayers.size(); i++)
	{
		for (int j = 0; j < this->hiddenLayers[i]->getNeuronsCount(); j++)
		{
			this->hiddenLayers[i]->setBias(
				j,
				this->hiddenLayers[i]->getNeurons()[j]->getBias() - 
				learning_rate * this->biasesGradient[i][j] / mini_batch_size
			);
		}
	}

	// output layer:
	for (int i = 0; i < this->outputLayer->getNeuronsCount(); i++)
	{
		this->outputLayer->setBias(
			i,
			this->outputLayer->getNeurons()[i]->getBias() - 
			learning_rate * this->biasesGradient[this->biasesGradient.size() - 1][i] / mini_batch_size
		);
	}

	// reset biases gradient:
	for (auto& v : this->biasesGradient) for (auto& biasGradient : v) biasGradient = 0.0;
}

// other helpers:
void NeuralNet::randomShuffle(
	std::vector<std::vector<Scalar>>& inputs, 
	std::vector<std::vector<Scalar>>& desired_outputs) const
{
	for (int i = 0; i < inputs.size(); i++)
	{
		std::pair<unsigned, unsigned> range = { 0U, inputs.size() - 1 };

		unsigned randomIndex = RandomNumbersGenerator::getRandomNumber(range);

		std::swap(inputs[randomIndex], inputs[i]);
		std::swap(desired_outputs[randomIndex], desired_outputs[i]);
	}
}

Scalar NeuralNet::validateClassification(
	const std::vector<std::vector<Scalar>>& validation_data_inputs, 
	const std::vector<std:: vector<Scalar>>& validation_data_outputs)
{
	unsigned goodAnswers = 0U;

	for (int i = 0; i < validation_data_inputs.size(); i++)
	{
		std::vector<Scalar> predictions = this->predict(validation_data_inputs[i]);

		// find max index in predictions:
		unsigned maxIndex1 = 0U;

		for (int j = 1; j < predictions.size(); j++) if (predictions[j] > predictions[maxIndex1]) maxIndex1 = j;

		// find max index in validation outputs:
		unsigned maxIndex2 = 0U;

		for (int j = 1; j < predictions.size(); j++)
		{
			if (validation_data_outputs[i][j] > validation_data_outputs[i][maxIndex2]) maxIndex2 = j;
		}

		goodAnswers += maxIndex1 == maxIndex2;
	}

	return static_cast<Scalar>(goodAnswers) / static_cast<Scalar>(validation_data_inputs.size());
}

std::vector<double> NeuralNet::getDropoutRates() const
{
	std::vector<double> dropoutRates;

	dropoutRates.reserve(this->hiddenLayers.size() + 1);

	dropoutRates.push_back(this->inputLayer->getDropoutRate());

	for (auto& hiddenLayer : this->hiddenLayers) dropoutRates.push_back(hiddenLayer->getDropoutRate());

	return dropoutRates;
}

void NeuralNet::setAllDropoutRatesToOne()
{
	this->inputLayer->setDropoutRate(1.0);

	for (auto& hiddenLayer : this->hiddenLayers) hiddenLayer->setDropoutRate(1.0);
}

void NeuralNet::setDropouts(const std::vector<double>& dropout_rates)
{
	this->inputLayer->setDropoutRate(dropout_rates[0]);

	for (int i = 0; i < this->hiddenLayers.size(); i++) this->hiddenLayers[i]->setDropoutRate(dropout_rates[i + 1]);
}

std::string NeuralNet::extractDirectoryPath(const std::string& file_path) const
{
	// TODO: explain actually why this function is neccessairly
	// TODO: add also info about / (\ does not work)
	// TODO: add info that it returns an empty string ("") when file_path doesn;t contain any directory

	// find the last '/' char:
	int lastSlachIndex = -1;

	for (int i = file_path.size() - 1; i >= 0; i--)
	{
		if (file_path[i] == '/')
		{
			lastSlachIndex = i;
			break;
		}
	}

	// there is no diretory in file_path:
	if (lastSlachIndex == -1) return "";

	// '/' can be Scalard or even appear a few times (for example "dir1/dir2///file.txt"):
	while (file_path[lastSlachIndex - 1] == '/') lastSlachIndex--;

	// extract the directory path from file_path:
	return file_path.substr(0, lastSlachIndex);
}
