#pragma once
#include <vector>
using std::vector;

class Net;
class Edge;

class Neuron {
    int id = 0;
    bool isInput = false;
    double value = 0;

    vector<Edge*> in;

    Neuron(int id, bool isInput);

public:
    
    void addEdge(Edge *edge);
    double activate();
    double getValue();

    static double sigmoid(double x);

    friend class Net;
};