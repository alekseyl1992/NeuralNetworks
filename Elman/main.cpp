#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Net.h"

using std::vector;

struct Chromosome {
    Net *net;
    double fitness = 0;

    Chromosome(Net *net)
        : net(net), fitness(0) { }
};

using Population = vector<Chromosome>;


double calcError(Net *net, vector<double> data, double expectedResult) {
    double result = 0;
    for (double value : data) {
        result = net->activate({ value });
    }

    double e =  std::abs(result - expectedResult);

    return e;
}

struct Sample {
    vector<double> stream;
    double result;
};

void calcFitness(Chromosome &ch, const vector<Sample> &trainSet) {
    double f = 0;
    for (const Sample &sample : trainSet) {
        f += calcError(ch.net, sample.stream, sample.result);
        ch.net->reset();
    }

    ch.fitness = f / trainSet.size();
}

Net *crossover(Net *a, Net *b) {
    Net *result = new Net(*a);

    for (int i = 0; i < result->edges.size(); ++i) {
        Edge *edge = result->edges[i];
        
        edge->w = rand() % 2 ?
            a->edges[i]->w : b->edges[i]->w;
    }

    return result;
}

void mutate(Net *net) {
    for (int i = 0; i < net->edges.size(); ++i) {
        Edge *edge = net->edges[i];

        edge->w = Net::getRandomWeight();
    }
}

int main() {
    // prepare data
    vector<Sample> trainSet = {
        Sample{ { 0, 1, 1, 0 }, 1 },
        Sample{ { 0, 1, 1, 0 }, 1 },
        Sample{ { 0, 1, 0, 0 }, 0 },
        Sample{ { 0, 0, 0, 0 }, 0 },
        Sample{ { 0, 1, 1, 1 }, 1 },
        Sample{ { 1, 1, 1, 1 }, 1 },
    };

    Population population;
    int populationSize = 20;
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(Chromosome(new Net()));
    }

    // train
    double targetFitness = 0.1;
    double mutationProb = 0.1;

    while (true) {
        for (Chromosome &ch : population) {
            calcFitness(ch, trainSet);
        }

        std::sort(population.begin(), population.end(), [](Chromosome &ch1, Chromosome &ch2) {
            return ch1.fitness < ch2.fitness;
        });

        if (population[0].fitness <= targetFitness)
            break;

        // crossover
        for (int i = 0; i < population.size() / 2; ++i) {
            Net *a = population[rand() % (population.size() / 2)].net;
            Net *b = population[rand() % (population.size() / 2)].net;
            Net *result = crossover(a, b);

            if (rand() % 100 < mutationProb * 100)
                mutate(result);

            int pos = population.size() - rand() % (population.size() / 2) - 1;
            population[pos] = Chromosome(result);
        }
    }

    // output
    Net *net = population[0].net;
    for (const Sample &sample : trainSet) {
        for (double v : sample.stream) {
            net->activate({ v });
        }

        std::cout << (net->output > 0) << std::endl;
    }

    std::cin.get();
}