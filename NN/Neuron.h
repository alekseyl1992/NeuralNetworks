#pragma once
#include <cmath>
#include <functional>
#include <vector>

using arr = std::vector<double>;
class Neuron {
private:
    struct Synaps {
        double w, dw;
    };

    std::vector<Synaps> synapses;
    double output = 0;

public:
    Neuron(size_t inputsCount, std::function<double()> &doubleSupplier) {
        synapses.resize(inputsCount + 1);  // +bias
        for (Synaps &s : synapses) {
            s.w = doubleSupplier();
            s.dw = 0;
        }
    }

    double activate(arr &inputs) {
        double sum = 0;
        for (size_t i = 0; i < inputs.size(); ++i) {
            sum += inputs[i] * synapses[i].w;
        }

        return output = sigmoid(sum);
    }

    double getOutput() {
        return output;
    }

    inline double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    friend class Layer;
};