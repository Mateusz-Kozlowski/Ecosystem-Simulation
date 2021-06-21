#pragma once

#include "InputLayer.h"
#include "HiddenLayer.h"
#include "OutputLayer.h"
#include "Matrix.h"

namespace CrappyNeuralNets
{
	class NeuralNet
	{
	public:
		// constructor/destructor:
		NeuralNet();
		~NeuralNet();

		void copyConstructor(const CrappyNeuralNets::NeuralNet& neural_net);

		// public methods:
		void initInputLayer(InputLayer* input_layer);

		void addHiddenLayer(HiddenLayer* hidden_layer);

		void initOutputLayer(OutputLayer* output_layer);

		void compile();

		bool loadFromFile(const std::string& file_path);

		const std::string& toString();

		void train(
			std::vector<std::vector<Scalar>>& inputs,
			std::vector<std::vector<Scalar>>& desired_outputs,
			unsigned epochs_count,
			const std::string& loss_function,
			unsigned mini_batch_size = 0U,
			const Scalar& learning_rate = 1.0,
			bool logs = true
		);
		void train(
			std::vector<std::vector<Scalar>>& inputs,
			std::vector<std::vector<Scalar>>& desired_outputs,
			const std::vector<std::vector<Scalar>>& validation_data_inputs,
			const std::vector<std::vector<Scalar>>& validation_data_outputs,
			unsigned epochs_count,
			const std::string& loss_function,
			unsigned mini_batch_size = 0U,
			const Scalar& learning_rate = 1.0,
			bool logs = true
		);

		const std::vector<Scalar>& predict(const std::vector<Scalar>& inputs);

		bool saveCurrentOutputsToFile(const std::string& file_path, const std::string& delimiter) const;

		bool saveToFile(const std::string& file_path) const;

		void randomMutate(const Scalar& mutation_percentage = 10.0);

		// architecture accessors:
		const InputLayer* getInputLayer() const;
		const std::vector<HiddenLayer*>& getHiddenLayers() const;
		const OutputLayer* getOutputLayer() const;

		const std::vector<Matrix*>& getWeights() const;
		const std::vector<Matrix*>& getTransposedWeights() const;
		const std::vector<Matrix*>& getWeightsGradient() const;

		const std::vector<std::vector<Scalar>>& getBiasesGradient() const;

	private:
		InputLayer* inputLayer;
		std::vector<HiddenLayer*> hiddenLayers;
		OutputLayer* outputLayer;

		std::vector<Matrix*> weights;
		std::vector<Matrix*> transposedWeights;
		std::vector<Matrix*> weightsGradient;

		std::vector<std::vector<Scalar>> biasesGradient;

		std::pair<Scalar, Scalar> randomWeightsRange;

		std::string string;

		bool isCompiled;

		// privates methods:

		// architecture:	
		void initRandomWeights();

		void initBiasesGradient();

		void deallocateAllMemoryAssociatedWithWeights();

		std::vector<unsigned> getCurrentTopology() const;

		// learning:
		void learningEpoch(
			std::vector<std::vector<Scalar>>& inputs,
			std::vector<std::vector<Scalar>>& desired_outputs,
			const std::string& loss_function,
			unsigned mini_batch_size,
			const Scalar& learning_rate,
			bool logs
		);

		void feedForward(const std::vector<Scalar>& inputs);

		void backpropagateErrors(
			const std::vector<Scalar>& desired_outputs,
			const std::string& loss_function
		);

		void updateWeightsGradient();

		void updateBiasesGradient();

		void updateWeights(const Scalar& learning_rate, const Scalar& mini_batch_size);

		void updateBiases(const Scalar& learning_rate, const Scalar& mini_batch_size);

		// other helpers:
		void randomShuffle(
			std::vector<std::vector<Scalar>>& inputs,
			std::vector<std::vector<Scalar>>& desired_outputs
		) const;

		Scalar validateClassification(
			const std::vector<std::vector<Scalar>>& validation_data_inputs,
			const std::vector<std::vector<Scalar>>& validation_data_outputs
		);

		std::vector<double> getDropoutRates() const;

		void setAllDropoutRatesToOne();

		void setDropouts(const std::vector<double>& dropout_rates);

		std::string extractDirectoryPath(const std::string& file_path) const;
	};
}
