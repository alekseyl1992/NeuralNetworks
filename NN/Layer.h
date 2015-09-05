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

    arr getDeltas(arr expectedValues) {
        arr deltas;
        for (size_t i = 0; i < neurons.size(); ++i) {
            Neuron &neuron = neurons[i];

            double expected = expectedValues[i];
            double actual = neuron.getOutput();

            double delta = -actual * (1 - actual) * (expected - actual);
            deltas.push_back(delta);
        }

        return deltas;
    }

    arr train(arr nextDeltas, Layer &nextLayer, double speed, double smoothing) {
        arr deltas;
        for (size_t i = 0; i < neurons.size(); ++i) {
            Neuron &neuron = neurons[i];

            double actual = neuron.getOutput();

            double sum = 0;
            for (size_t j = 0; j < nextLayer.neurons.size(); ++j) {
                Neuron &nextNeuron = nextLayer.neurons[j];

                Neuron::Synaps &s = nextNeuron.synapses[i];
                sum += s.w * nextDeltas[j];
            }

            double delta = actual * (1 - actual) * sum;

            // change weights
            for (size_t j = 0; j < nextLayer.neurons.size(); ++j) {
                Neuron &nextNeuron = nextLayer.neurons[j];

                Neuron::Synaps &s = nextNeuron.synapses[i];
                s.dw = smoothing * s.dw + (1 - smoothing) * speed * nextDeltas[j] * actual;
                s.w += s.dw;
            }

            deltas.push_back(delta);
        }

        return deltas;
    }
};