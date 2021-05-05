#pragma once

#include "Config.h"

namespace CrappyNeuralNets
{
	class Neuron
	{
	public:
		// constructor:
		Neuron(const std::string& activation_function = "linear");

		// mutators:
		void feed(const std::vector<Neuron*>& previous_layer_neurons, const std::vector<Scalar>& weights);

		void feed(const std::vector<Scalar>& previous_layer_outputs, const std::vector<Scalar>& weights);

		void setBias(const Scalar& new_bias_value);

		void setActivationFunction(const std::string& activation_function);

		void activate();

		void setActivatedValue(const Scalar& new_activated_value);

		void derivate();

		void setDerivedValue(const Scalar& new_derived_value);

		void calcLossDerivativeWithRespectToActivationFunc(
			const Scalar& desired_output,
			const std::string& loss_function
		);
		void calcLossDerivativeWithRespectToActivationFunc(
			const std::vector<Neuron*>& next_layer_neurons,
			const std::vector<Scalar>& transposed_weights
		);

		void setLossDerivativeWithRespectToActivationFunc(const Scalar& new_value);

		void dropOut(bool dropped_out);

		void setLeakyReLUalfa(const Scalar& leaky_ReLU_alfa);

		// accessors:
		const Scalar& getValue() const;

		const Scalar& getBias() const;

		const std::string& getActivationFunction() const;

		const Scalar& getActivatedValue() const;

		const Scalar& getDerivatedValue() const;

		const Scalar& getLossDerivativeWithRespectToActivationFunc() const;

		bool isDroppedOut() const;

		const Scalar& getLeakyReLUalfa() const;

	private:
		Scalar value;

		Scalar bias;

		std::string activationFunction;

		Scalar leakyReLUalfa;

		Scalar activatedValue;

		Scalar derivedValue;

		Scalar lossDerivativeWithRespectToActivationFunc;

		bool droppedOut;

		// activation functions:
		Scalar ReLU(const Scalar& x) const;
		Scalar derivativeReLU(const Scalar& x) const;

		Scalar leakyReLU(const Scalar& x, const Scalar& alfa) const;
		Scalar derivativeLeakyReLU(const Scalar& x, const Scalar& alfa) const;

		Scalar sigmoid(const Scalar& x) const;
		Scalar derivativeSigmoid(const Scalar& sigmoid) const;

		Scalar fastSigmoid(const Scalar& x) const;
		Scalar derivativeFastSigmoid(const Scalar& x) const;

		Scalar tanh(const Scalar& x) const;
		Scalar derivativetanh(const Scalar& tanh) const;
	};
}
