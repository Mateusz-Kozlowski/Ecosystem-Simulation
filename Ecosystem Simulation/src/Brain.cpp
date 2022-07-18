#include "bbpch.h"
#include "Brain.h"

Blueberry::Brain::Brain(const unsigned inputSize, const unsigned outputSize)
	: m_inputSize(inputSize)
	, m_outputSize(outputSize)
	, m_neurons(inputSize + outputSize)
	, m_output(outputSize)
{
	
}

Blueberry::Brain::Brain(const char* filePath)
{
	if (!loadFromFile(filePath))
	{
		std::cerr
			<< "Error::Blueberry::Brain::Brain(const char* filePath):\n"
			<< "could not open: " << filePath << '\n';
		assert(false);
	}
}

Blueberry::Brain::Brain(const Brain& rhs)
	: m_inputSize(rhs.m_inputSize)
	, m_outputSize(rhs.m_outputSize)
	, m_neurons(rhs.m_neurons)
	, m_output(rhs.m_output)
	, m_synapses(rhs.m_synapses)
{
	
}

Blueberry::Brain& Blueberry::Brain::operator=(const Brain& rhs)
{
	if (this == &rhs)
		return *this;

	m_inputSize = rhs.m_inputSize;
	m_outputSize = rhs.m_outputSize;
	m_neurons = rhs.m_neurons;
	m_output = rhs.m_output;
	m_synapses = rhs.m_synapses;

	return *this;
}

bool Blueberry::Brain::saveToFile(const char* filePath) const
{
	std::filesystem::path dirPath = std::filesystem::path(filePath).remove_filename();
	
	if (dirPath != "")
	{
		std::filesystem::create_directories(dirPath);
	}

	std::ofstream ofs(filePath);

	if (!ofs.is_open()) return false;

	ofs << m_inputSize << '\n';
	ofs << m_outputSize << '\n';
	ofs << m_neurons.size() << '\n';

	for (const auto& neuron : m_neurons)
	{
		ofs << neuron.toStr() << '\n';
	}

	ofs << m_synapses.size() << '\n';

	for (const auto& synapse : m_synapses)
	{
		ofs << synapse.toStr() << '\n';
	}

	ofs.close();
	
	return true;
}

bool Blueberry::Brain::loadFromFile(const char* filePath)
{
	std::ifstream ifs(filePath);

	if (!ifs.is_open()) return false;
	
	unsigned neuronsCount = 0U;
	unsigned synapsesCount = 0U;

	ifs >> m_inputSize;
	ifs >> m_outputSize;
	ifs >> neuronsCount;

	m_neurons.clear();
	m_neurons.reserve(neuronsCount);

	while (neuronsCount--)
	{
		bool disabled;
		Scalar val;
		Scalar bias;
		Scalar actVal;
		std::string actFunc;

		ifs >> disabled;
		ifs >> val;
		ifs >> bias;
		ifs >> actVal;
		std::getline(ifs, actFunc);
		std::getline(ifs, actFunc);

		m_neurons.emplace_back(disabled, val, bias, actVal, actFunc.c_str());
	}

	m_output.resize(m_outputSize);

	ifs >> synapsesCount;

	m_synapses.clear();
	m_synapses.reserve(synapsesCount);

	while (synapsesCount--)
	{
		bool disabled;
		unsigned src;
		unsigned dst;
		Scalar weight;

		ifs >> disabled;
		ifs >> src;
		ifs >> dst;
		ifs >> weight;

		m_synapses.emplace_back(disabled, src, dst, weight);
	}

	ifs.close();

	return true;
}

void Blueberry::Brain::propagateForward()
{
	resetNeuronsVals();
	propagateForwardActVals();
	activateHiddenAndOutputNeurons();
}

// accessors:

unsigned Blueberry::Brain::getEnabledNeuronsCount() const
{
	return m_neurons.size() - getDisabledNeuronsCount();
}

unsigned Blueberry::Brain::getDisabledNeuronsCount() const
{
	unsigned disabledNeuronsCount = 0U;

	for (const auto& neuron : m_neurons)
	{
		if (neuron.disabled()) disabledNeuronsCount++;
	}

	return disabledNeuronsCount;
}

const Blueberry::Scalar& Blueberry::Brain::getSpecificOutput(unsigned outputNeuronIndex) const
{
	return m_neurons[m_inputSize + outputNeuronIndex].getActVal();
}

const std::vector<Blueberry::Scalar>& Blueberry::Brain::getOutput()
{
	calcOutputVector();
	return m_output;
}

unsigned Blueberry::Brain::getEnabledSynapsesCount() const
{
	return m_synapses.size() - getDisabledSynapsesCount();
}

unsigned Blueberry::Brain::getDisabledSynapsesCount() const
{
	unsigned disabledSynapsesCount = 0U;

	for (const auto& synapse : m_synapses)
	{
		if (synapse.disabled()) disabledSynapsesCount++;
	}

	return disabledSynapsesCount;
}

// mutators:

void Blueberry::Brain::mutate(unsigned changesCount)
{
	while (changesCount--)
	{
		const unsigned randomNumber = RandomEngine::getIntInRange(0, 6);

		switch (randomNumber)
		{
		case 0U:
			addRandomNeuron();
			break;
		case 1U:
			mutateRandomNeuronBias();
			break;
		case 2U:
			mutateRandomNeuronActFunc();
			break;
		case 3U:
			disbaleRandomNeuron();
			break;
		case 4U:
			addRandomSynapse();
			break;
		case 5U:
			mutateRandomSynapseWeight();
			break;
		case 6U:
			disableRandomSynapse();
			break;
		default:
			std::cerr
				<< "Error::Blueberry::Brain::mutate(unsigned changesCount):\n"
				<< "a random number is out of range\n";
			assert(false);
		}
	}
}

bool Blueberry::Brain::addRandomNeuron()
{
	int randomSynapseIndex = getRandomSynapseIndex();

	// we can't add a neuron if all synapses are disabled:
	if (randomSynapseIndex == -1) return false;

	assert(!m_synapses[randomSynapseIndex].disabled());
	assert(!m_neurons[m_synapses[randomSynapseIndex].getSrc()].disabled());
	assert(!m_neurons[m_synapses[randomSynapseIndex].getDst()].disabled());

	// add a random neuron:
	m_neurons.emplace_back();
	m_neurons.back().mutateBias(1.0);
	m_neurons.back().setRandomActFunc();

	// split that synapse:
	m_synapses[randomSynapseIndex].disable();

	m_synapses.emplace_back(
		false,
		m_synapses[randomSynapseIndex].getSrc(),
		m_neurons.size() - 1,
		1.0
	);
	
	m_synapses.emplace_back(
		false,
		m_neurons.size() - 1,
		m_synapses[randomSynapseIndex].getDst(),
		m_synapses[randomSynapseIndex].getWeight()
	);

	return true;
}

void Blueberry::Brain::mutateRandomNeuronBias()
{
	unsigned randomHiddenOrOutputNeuronIndex = getRandomHiddenOrOutputNeuronIndex();

	assert(!m_neurons[randomHiddenOrOutputNeuronIndex].disabled());

	// mutate a neuron bias:
	// TODO: get rid of that hard-coded argument:
	m_neurons[randomHiddenOrOutputNeuronIndex].mutateBias(0.5);
}

void Blueberry::Brain::mutateRandomNeuronActFunc()
{
	unsigned randomHiddenOrOutputNeuronIndex = getRandomHiddenOrOutputNeuronIndex();

	assert(!m_neurons[randomHiddenOrOutputNeuronIndex].disabled());

	// mutate a neuron act func:
	m_neurons[randomHiddenOrOutputNeuronIndex].setRandomActFunc();
}

bool Blueberry::Brain::disbaleRandomNeuron()
{
	int randomHiddenNeuronIndex = getRandomHiddenNeuronIndex();

	// we can't disable a hidden neuron if all hidden neurons are disabled:
	if (randomHiddenNeuronIndex == -1) return false;

	assert(!m_neurons[randomHiddenNeuronIndex].disabled());
	
	recurrentGarbageCollector(randomHiddenNeuronIndex);

	return true;
}

void Blueberry::Brain::addRandomSynapse()
{
	unsigned randomNeuronIndex1 = getRandomNeuronIndex();
	unsigned randomNeuronIndex2 = getRandomNeuronIndex();

	bool areEqual = randomNeuronIndex1 == randomNeuronIndex2;

	while (areEqual || isInput(randomNeuronIndex2) || isOutput(randomNeuronIndex1))
	{
		randomNeuronIndex1 = getRandomNeuronIndex();
		randomNeuronIndex2 = getRandomNeuronIndex();

		areEqual = randomNeuronIndex1 == randomNeuronIndex2;
	}

	assert(!m_neurons[randomNeuronIndex1].disabled());
	assert(!m_neurons[randomNeuronIndex2].disabled());

	m_synapses.emplace_back(
		false,
		randomNeuronIndex1,
		randomNeuronIndex2,
		RandomEngine::getScalarInRange(-1.0, 1.0)
	);
}

bool Blueberry::Brain::mutateRandomSynapseWeight()
{
	int randomSynapseIndex = getRandomSynapseIndex();
	
	// we can't mutate a synapse index if all synapses are disabled:
	if (randomSynapseIndex == -1) return false;

	assert(!m_synapses[randomSynapseIndex].disabled());
	assert(!m_neurons[m_synapses[randomSynapseIndex].getSrc()].disabled());
	assert(!m_neurons[m_synapses[randomSynapseIndex].getDst()].disabled());

	// mutate a random synapse weight:
	m_synapses[randomSynapseIndex].mutateWeight(0.5);

	return true;
}

bool Blueberry::Brain::disableRandomSynapse()
{
	int randomSynapseIndex = getRandomSynapseIndex();

	// we can't mutate a synapse index if all synapses are disabled:
	if (randomSynapseIndex == -1) return false;

	assert(!m_synapses[randomSynapseIndex].disabled());
	assert(!m_neurons[m_synapses[randomSynapseIndex].getSrc()].disabled());
	assert(!m_neurons[m_synapses[randomSynapseIndex].getDst()].disabled());

	// disable:
	m_synapses[randomSynapseIndex].disable();
	
	// try to disable dst of this synapse:
	unsigned dstIndex = m_synapses[randomSynapseIndex].getDst();

	if (dstIndex >= m_inputSize + m_outputSize && hasZeroInputs(dstIndex))
	{
		recurrentGarbageCollector(dstIndex);
	}
	
	return true;
}

// private methods:

void Blueberry::Brain::resetNeuronsVals()
{
	for (auto& neuron : m_neurons) 
	{
		neuron.resetVal();
	}
}

void Blueberry::Brain::propagateForwardActVals()
{
	for (const auto& synapse : m_synapses)
	{
		if (synapse.disabled()) continue;
		if (m_neurons[synapse.getSrc()].disabled()) continue;
		if (m_neurons[synapse.getDst()].disabled()) continue;

		m_neurons[synapse.getDst()].setVal(
			m_neurons[synapse.getDst()].getVal() +
			m_neurons[synapse.getSrc()].getActVal() * synapse.getWeight()
		);
	}
}

void Blueberry::Brain::activateHiddenAndOutputNeurons()
{
	for (int i = m_inputSize; i < m_neurons.size(); i++)
	{
		if (!m_neurons[i].disabled())
		{
			m_neurons[i].activate();
		}
	}
}

void Blueberry::Brain::calcOutputVector()
{
	for (int i=0; i<m_outputSize; i++)
	{
		m_output[i] = m_neurons[m_inputSize + i].getActVal();
	}
}

int Blueberry::Brain::getRandomHiddenNeuronIndex() const
{
	// there are no hidden neurons:
	if (m_neurons.size() == m_inputSize + m_outputSize) return -1;

	bool allHiddenAreRemoved = true;

	for (int i = m_inputSize + m_outputSize; i < m_neurons.size(); i++)
	{
		if (!m_neurons[i].disabled())
		{
			allHiddenAreRemoved = false;
			break;
		}
	}
	
	if (allHiddenAreRemoved) return -1;

	unsigned randomHiddenNeuronIndex = RandomEngine::getIntInRange(
		m_inputSize + m_outputSize,
		m_neurons.size() - 1
	);

	while (m_neurons[randomHiddenNeuronIndex].disabled())
	{
		randomHiddenNeuronIndex = RandomEngine::getIntInRange(
			m_inputSize + m_outputSize,
			m_neurons.size() - 1
		);
	}

	assert(!m_neurons[randomHiddenNeuronIndex].disabled());

	return randomHiddenNeuronIndex;
}

unsigned Blueberry::Brain::getRandomHiddenOrOutputNeuronIndex() const
{
	unsigned randomNeuronIndex = RandomEngine::getIntInRange(m_inputSize, m_neurons.size() - 1);

	while (m_neurons[randomNeuronIndex].disabled())
	{
		randomNeuronIndex = RandomEngine::getIntInRange(m_inputSize, m_neurons.size() - 1);
	}

	assert(!m_neurons[randomNeuronIndex].disabled());

	return randomNeuronIndex;
}

unsigned Blueberry::Brain::getRandomNeuronIndex() const
{
	unsigned randomNeuronIndex = RandomEngine::getIntInRange(0, m_neurons.size() - 1);

	while (m_neurons[randomNeuronIndex].disabled())
	{
		randomNeuronIndex = RandomEngine::getIntInRange(0, m_neurons.size() - 1);
	}

	assert(!m_neurons[randomNeuronIndex].disabled());

	return randomNeuronIndex;
}

bool Blueberry::Brain::isOutput(unsigned neuronIndex) const
{
	return neuronIndex >= m_inputSize && neuronIndex < m_inputSize + m_outputSize;
}

int Blueberry::Brain::getRandomSynapseIndex() const
{
	if (getDisabledSynapsesCount() == m_synapses.size()) return -1;

	unsigned randomSynapseIndex = RandomEngine::getIntInRange(0, m_synapses.size() - 1);

	while (m_synapses[randomSynapseIndex].disabled())
	{
		randomSynapseIndex = RandomEngine::getIntInRange(0, m_synapses.size() - 1);
	}

	assert(!m_synapses[randomSynapseIndex].disabled());

	return randomSynapseIndex;
}

void Blueberry::Brain::disableInputSynapses(unsigned neuronIndex)
{
	for (auto& synapse : m_synapses)
	{
		if (synapse.getDst() == neuronIndex)
		{
			synapse.disable();
		}
	}
}

void Blueberry::Brain::disableOutputSynapses(unsigned neuronIndex)
{
	for (auto& synapse : m_synapses)
	{
		if (synapse.getSrc() == neuronIndex) 
		{
			synapse.disable();
		}
	}
}

bool Blueberry::Brain::hasZeroInputs(unsigned neuronIndex) const
{
	for (const auto& synapse : m_synapses)
	{
		if (synapse.disabled()) continue;

		if (synapse.getDst() == neuronIndex) return false;
	}

	return true;
}

void Blueberry::Brain::recurrentGarbageCollector(unsigned neuronIndex)
{
	disableInputSynapses(neuronIndex);
	disableOutputSynapses(neuronIndex);
	m_neurons[neuronIndex].disable();

	for (int i = m_inputSize + m_outputSize; i < m_neurons.size(); i++)
	{
		if (m_neurons[i].disabled()) continue;

		if (hasZeroInputs(i))
		{
			recurrentGarbageCollector(i);
		}
	}
}
