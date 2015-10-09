#pragma once
#include <vector>
using std::vector;

class Net;
class Edge;

class Neuron {
    Net *net;
    int id;
    bool isInput;
    double value;

    vector<Edge*> in;

    Neuron(int id, bool isInput);

public:
    
    void addEdge(Edge *edge);
    double activate();
    double getValue();

    static double sigmoid(double x);

    friend class Net;
};