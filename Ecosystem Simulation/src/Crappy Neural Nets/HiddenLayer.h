#pragma once

#include "Neuron.h"
#include "Matrix.h"

namespace CrappyNeuralNets
{
    class HiddenLayer
    {
    public:
        // constructor:
        HiddenLayer(
            unsigned neurons_count,
            const std::string& activation_function = "linear",
            const Scalar& dropout_rate = 1.0
        );

        // mutators:
        void setDropoutRate(const Scalar& dropout_rate);

        void input(const std::vector<Neuron*>& previous_layer_neurons, const Matrix* weights);
        void input(const std::vector<Scalar>& previous_layer_outputs, const Matrix* weights);

        void setActivationFunction(const std::string& activation_function);

        void backpropErrors(const std::vector<Neuron*>& next_layer_neurons, const Matrix* transposed_weights);

        void setBias(unsigned neuron_index, const Scalar& new_bias_value);

        //void setLeakyReLUalfa(const Scalar& leaky_ReLU_alfa);

        void setRandomNumbersGenerator(RandomNumbersGenerator& generator);

        // accessors:
        const Scalar& getDropoutRate() const;

        const std::string& getActivationFunction() const;

        unsigned getNeuronsCount() const;

        const std::vector<Neuron*>& getNeurons() const;

        const Scalar& getLeakyReLUalfa() const;

    private:
        Scalar dropoutRate;

        std::string activationFunction;

        std::vector<Neuron*> neurons;

        Scalar leakyReLUalfa;

        RandomNumbersGenerator* randomNumbersGenerator;

        // private functions:
        void setNewDropout();
    };
}
