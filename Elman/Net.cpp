#include <vector>
#include <random>
#include "Net.h"
#include "Neuron.h"
using std::vector;

Net::Net() {
    vector<Neuron *> hidden;
    int hiddenCount = 9;
    for (int i = 0; i < hiddenCount; ++i) {
        Neuron *h = createNeuron();
        hidden.push_back(h);
    }
        
    Neuron *o = createNeuron();
    
    int inputsCount = 9;
    for (int i = 0; i < inputsCount; ++i) {
        Neuron *n = createNeuron(true);
        for (int j = 0; j < hiddenCount; ++j) {
            createEdge(n, hidden[j]);
        }
    }

    for (int i = 0; i < hiddenCount; ++i) {
        createEdge(hidden[i], o);
        for (int j = 0; j < hiddenCount; ++j) {
            createEdge(hidden[i], hidden[j]);
        }
    }

    output = o;
}

Net::Net(const Net &net) {
    for (Neuron *n : net.inputs) {
        Neuron *copy = createNeuron(true);
    }

    for (Neuron *n : net.neurons) {
        Neuron *copy = createNeuron();
    }

    for (Edge *e : net.edges) {
        Neuron *from;
        if (!e->from->isInput) {
            from = neurons[e->from->id];
        }
        else {
            from = inputs[e->from->id];
        }

        Neuron *to = neurons[e->to->id];

        Edge *copy = createEdge(from, to);
        copy->w = e->w;
    }

    output = net.neurons[net.output->id];
}

Neuron *Net::createNeuron(bool isInput) {
    Neuron *neuron;

    if (!isInput) {
        neuron = new Neuron(neurons.size(), isInput);
        neurons.push_back(neuron);
    }
    else {
        neuron = new Neuron(inputs.size(), isInput);
        inputs.push_back(neuron);
    }

    return neuron;
}

Edge *Net::createEdge(Neuron *from, Neuron *to) {
    Edge *edge = new Edge(from, to, edges.size());
    edge->w = getRandomWeight();

    to->addEdge(edge);
    edges.push_back(edge);

    return edge;
}

double Net::activate(const vector<double> &value) {
    for (int i = 0; i < value.size(); ++i) {
        inputs[i]->value = value[i];
    }
    
    for (int i = 0; i < neurons.size(); ++i) {
        neurons[i]->activate();
    }

    return output->value;
}

void Net::reset() {
    for (Neuron *n: neurons) {
        n->value = 0;
    }
}

double Net::getRandomWeight(double oldWeight) {
    double rnd = getRandomWeight();
    double newWeight = oldWeight * 0.7 + rnd * 0.3;
    return newWeight;
}

double Net::getRandomWeight() {
    const double lower_bound = -1;
    const double upper_bound = 1;
    static std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    static std::random_device rand_dev;
    static std::mt19937 rand_engine(rand_dev());

    double x = unif(rand_engine);
    return x;
}
