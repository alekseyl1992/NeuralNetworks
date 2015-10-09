#pragma once

class Neuron;
struct Edge {
    double w;
    int id;
    Neuron *from;
    Neuron *to;

    Edge(Neuron *from, Neuron *to, int id);

    friend class Neuron;
    friend class Net;

public:
};