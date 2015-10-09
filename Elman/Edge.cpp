#include "Edge.h"
#include "Neuron.h"

Edge::Edge(Neuron *from, Neuron *to, int id)
    : from(from), to(to), id(id) {

}