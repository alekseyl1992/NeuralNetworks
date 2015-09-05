#pragma once

#include <vector>
#include "Neuron.h"

class Layer {
private:
    std::vector<Neuron> neurons;

public:
    Layer(size_t neuronsCount, size_t inputsCount, std::function<double()> randomGenerator) {
        for (int i = 0; i < neuronsCount; ++i) {
            neurons.emplace_back(Neuron(inputsCount, randomGenerator));
        }
    }

    arr activate(arr inputs) {
        inputs.push_back(1);  // bias

        arr result;
        for (Neuron &n : neurons) {
            result.push_back(n.activate(inputs));
        }

        return result;
    }

    void trainLast(arr expectedValues) {
        for (size_t i = 0; i < neurons.size(); ++i) {
            Neuron &neuron = neurons[i];

            double expected = expectedValues[i];
            double actual = neuron.getOutput();

            neuron.delta = -actual * (1 - actual) * (expected - actual);
        }
    }

    void train(Layer &nextLayer) {
        for (size_t i = 0; i < neurons.size(); ++i) {
            Neuron &neuron = neurons[i];

            double actual = neuron.getOutput();

            double sum = 0;
            for (size_t j = 0; j < nextLayer.neurons.size(); ++j) {
                Neuron &nextNeuron = nextLayer.neurons[j];

                Neuron::Synaps &s = nextNeuron.synapses[i];
                sum += s.w * nextNeuron.delta;
            }

            neuron.delta = actual * (1 - actual) * sum;
        }
    }

    void updateWeights(arr inputs, double speed, double smoothing) {
        inputs.push_back(1);

        for (Neuron &neuron: neurons) {
            for (int i = 0; i < neuron.synapses.size(); ++i) {
                Neuron::Synaps &s = neuron.synapses[i];
                s.dw = smoothing * s.dw + (1 - smoothing) * speed * neuron.delta * inputs[i];
                s.w += s.dw;
            }
        }
    }

    void updateWeights(Layer &inputs, double speed, double smoothing) {
        for (Neuron &neuron : neurons) {
            for (int i = 0; i < neuron.synapses.size(); ++i) {
                Neuron::Synaps &s = neuron.synapses[i];

                double value = 1;  // bias
                if (i < inputs.neurons.size())
                    value = inputs.neurons[i].output;

                s.dw = smoothing * s.dw + (1 - smoothing) * speed * neuron.delta * value;
                s.w += s.dw;
            }
        }
    }
};