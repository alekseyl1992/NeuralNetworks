#pragma once
#include "Neuron.h"
#include "Edge.h"

struct Net {
    vector<Neuron *> neurons;
    vector<Edge *> edges;

    vector<Neuron *> inputs;
    Neuron *output;

    Net();
    Net(const Net &net);

    Neuron *createNeuron(bool isInput=false);
    Edge *createEdge(Neuron *from, Neuron *to);

    double activate(const vector<double> &value);
    void reset();

    static double getRandomWeight();
};
