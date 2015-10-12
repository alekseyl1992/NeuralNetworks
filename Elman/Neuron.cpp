#include "Neuron.h"
#include "Net.h"

Neuron::Neuron(int id, bool isInput)
    : id(id), isInput(isInput) {

}

void Neuron::addEdge(Edge *edge) {
    in.push_back(edge);
}


double Neuron::activate() {
    double sum = 0;
    for (int i = 0; i < in.size(); ++i) {
        sum += in[i]->w * in[i]->from->value;
    }

    value = sigmoid(sum);
    return value;
}

double Neuron::getValue() {
    return value;
}

double Neuron::sigmoid(double x) {
    return x > 0 ? 1 : -1;
    //return 2.0 / (1 + std::exp(-x * 20)) - 1;
}