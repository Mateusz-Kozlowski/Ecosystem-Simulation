#pragma once

#include "Neuron.h"
#include "Matrix.h"

namespace CrappyNeuralNets
{
    class OutputLayer
    {
    public:
        // constructor:
        OutputLayer(
            unsigned neurons_count,
            const std::string& activation_function = "linear"
        );

        void copyConstructor(const CrappyNeuralNets::OutputLayer& output_layer);

        // mutators:
        void input(const std::vector<Scalar>& previous_layer_outputs, const Matrix* weights);
        void input(const std::vector<Neuron*>& previous_layer_neurons, const Matrix* weights);

        void setActivationFunction(const std::string& activation_function);

        void backpropErrors(const std::vector<Scalar>& desired_values, const std::string& loss_function);

        void setBias(unsigned neuron_index, const Scalar& new_bias_value);

        // accessors:
        const std::string& getActivationFunction() const;

        const std::vector<Scalar>& getOutputs();

        unsigned getNeuronsCount() const;

        const std::vector<Neuron*>& getNeurons() const;

        Scalar calcCurrentLoss(const std::vector<Scalar>& desired_values, const std::string& loss_function) const;

    private:
        std::string activationFunction;

        std::vector<Scalar> outputs;

        std::vector<Neuron*> neurons;

        // private functions:
        void softmax();

        void calcSoftmaxDerivatives();
    };
}
