#pragma once

class Neuron;
struct Edge {
    double w = 0;
    int id = 0;
    Neuron *from = nullptr;
    Neuron *to = nullptr;

    Edge(Neuron *from, Neuron *to, int id);

    friend class Neuron;
    friend class Net;

public:
};