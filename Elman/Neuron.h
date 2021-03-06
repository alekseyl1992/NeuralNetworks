#pragma once
#include <vector>
using std::vector;

struct Net;
struct Edge;

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

    friend struct Net;
};