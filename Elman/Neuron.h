#pragma once
#include <vector>
using std::vector;

class Net;
class Edge;

class Neuron {
    Net *net;
    int id;
    double value;

    vector<Edge*> in;

    Neuron(int id);

public:

    void addEdge(Edge *edge);
    double activate();

    static double sigmoid(double x);

    friend class Net;
};