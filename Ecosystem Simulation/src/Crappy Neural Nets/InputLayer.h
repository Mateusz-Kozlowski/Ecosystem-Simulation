#pragma once

#include "RandomNumbersGenerator.h"

namespace CrappyNeuralNets
{
    class InputLayer
    {
    public:
        // constructor:
        InputLayer(unsigned inputs_count, const Scalar& dropout_rate = 1.0);

        // mutators:
        void setDropoutRate(const Scalar& dropout_rate);

        void input(const std::vector<Scalar>& inputs);

        void setRandomNumbersGenerator(RandomNumbersGenerator& generator);

        // accessors:
        const Scalar& getDropoutRate() const;

        unsigned getSize() const;

        const std::vector<Scalar>& output() const;

    private:
        Scalar dropoutRate;

        std::vector<Scalar> v;

        RandomNumbersGenerator* randomNumbersGenerator;

        // private methods:
        void dropout();
    };
}
