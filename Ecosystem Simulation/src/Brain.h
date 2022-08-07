#pragma once

#include "Neuron.h"
#include "Synapse.h"

namespace Blueberry
{
	class Brain
	{
	public:
		Brain(
			const unsigned inputSize,
			const unsigned outputSize,
			const std::vector<std::string>& neuronsAdditionalInfo = {}
		);
		Brain(const std::string& filePath);
		Brain(const Brain& rhs);
		Brain& operator=(const Brain& rhs);

		bool saveToFile(const char* filePath) const;
		bool loadFromFile(const char* filePath);

		template<typename T>
		void setInput(const std::vector<T>& input);
		
		template<typename T>
		void setInput(std::vector<T>&& input);

		template<typename T>
		void propagateForward(const std::vector<T>& input);
		void propagateForward();

		// accessors:

		inline unsigned getInputSize() const { return m_inputSize; }
		inline unsigned getOutputSize() const { return m_outputSize; }
		
		unsigned getEnabledNeuronsCount() const;
		unsigned getDisabledNeuronsCount() const;

		inline const std::vector<Neuron>& getNeurons() const { return m_neurons; }

		const Scalar& getSpecificOutput(unsigned outputNeuronIndex) const;
		const std::vector<Scalar>& getOutput();
		
		unsigned getEnabledSynapsesCount() const;
		unsigned getDisabledSynapsesCount() const;

		inline const std::vector<Synapse>& getSynapses() const { return m_synapses; }
		
		// mutators:

		void mutate(unsigned changesCount);

		bool addRandomNeuron();
		void mutateRandomNeuronBias();
		void mutateRandomNeuronActFunc();
		bool disbaleRandomNeuron();
		void addRandomSynapse();
		bool mutateRandomSynapseWeight();
		bool disableRandomSynapse();

	private:
		void setNeuronsAdditionalInfo(
			const std::vector<std::string>& neuronsAdditionalInfo
		);

		void resetNeuronsVals();
		void propagateForwardActVals();
		void activateHiddenAndOutputNeurons();

		void calcOutputVector();

		int getRandomHiddenNeuronIndex() const;
		unsigned getRandomHiddenOrOutputNeuronIndex() const;
		unsigned getRandomNeuronIndex() const;

		inline bool isInput(unsigned neuronIndex) const { return neuronIndex < m_inputSize; }
		bool isOutput(unsigned neuronIndex) const;
		
		int getRandomSynapseIndex() const;

		void disableInputSynapses(unsigned neuronIndex);
		void disableOutputSynapses(unsigned neuronIndex);

		bool hasZeroInputs(unsigned neuronIndex) const;

		void recurrentGarbageCollector(unsigned neuronIndex);

	private:
		unsigned m_inputSize;
		unsigned m_outputSize;

		std::vector<Neuron> m_neurons;
		std::vector<Scalar> m_output;
		std::vector<Synapse> m_synapses;
	};

	template<typename T>
	void Brain::setInput(const std::vector<T>& input)
	{
		if (input.size() != m_inputSize)
		{
			std::cerr
				<< "Error::void Brain::setInput(const std::vector<T>& input):\n"
				<< "input size should be equal to " << m_inputSize
				<< " and not to " << input.size() << '\n';
			assert(false);
		}

		for (int i = 0; i < input.size(); i++) 
		{
			m_neurons[i].setActVal(input[i]);
		}
	}

	template<typename T>
	inline void Brain::setInput(std::vector<T>&& input)
	{
		if (input.size() != m_inputSize)
		{
			std::cerr
				<< "Error::void Brain::setInput(const std::vector<T>& input):\n"
				<< "input size should be equal to " << m_inputSize
				<< " and not to " << input.size() << '\n';
			assert(false);
		}

		for (int i = 0; i < input.size(); i++)
		{
			m_neurons[i].setActVal(input[i]);
		}
	}

	template<typename T>
	void Brain::propagateForward(const std::vector<T>& input)
	{
		setInput(input);
		propagateForward();
	}
}
