#pragma once
#include "Neuron.h"
#include "Edge.h"

struct Net {
    struct Config {
        int inputsCount = 9;
        int hiddenCount = 9;
    };
    Config config;

    vector<Neuron *> neurons;
    vector<Edge *> edges;

    vector<Neuron *> inputs;
    Neuron *output;

    Net(const Config &config);
    Net(const Net &net);
    ~Net();

    Neuron *createNeuron(bool isInput=false);
    Edge *createEdge(Neuron *from, Neuron *to);

    double activate(const vector<double> &value);
    void reset();

    static double getRandomWeight(double oldWeight);
    static double getRandomWeight();
};
