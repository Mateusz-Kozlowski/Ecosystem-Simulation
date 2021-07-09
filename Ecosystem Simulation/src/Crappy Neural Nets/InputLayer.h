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

        // accessors:
        const Scalar& getDropoutRate() const;

        unsigned getSize() const;

        const std::vector<Scalar>& output() const;

    private:
        Scalar dropoutRate;

        std::vector<Scalar> v;

        // private methods:
        void dropout();
    };
}
