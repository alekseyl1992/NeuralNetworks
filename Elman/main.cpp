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

struct Sample {
    vector<double> stream;
    double result;
};


double calcError(Net *net, vector<double> stream, double expectedResult) {
    for (double value : stream) {
        net->activate({ value });
    }

    double result = net->output->getValue();
    double e = std::abs(result - expectedResult);

    return e;
}

void calcFitness(Chromosome &ch, const vector<Sample> &trainSet) {
    double f = 0;
    for (const Sample &sample : trainSet) {
        ch.net->reset();
        f += calcError(ch.net, sample.stream, sample.result);
    }

    ch.fitness = f / trainSet.size();
}

Net *crossover(Net *a, Net *b) {
    Net *result = new Net(*a);

    for (int i = 0; i < result->edges.size(); ++i) {
        Edge *edge = result->edges[i];
        
        edge->w = (rand() % 2) ? a->edges[i]->w : b->edges[i]->w;
    }

    return result;
}

void mutate(Net *net) {
    for (int i = 0; i < net->edges.size(); ++i) {
        Edge *edge = net->edges[i];

        edge->w = Net::getRandomWeight(edge->w);
    }
}

int main() {
    // prepare data
    vector<Sample> trainSet = {
        Sample{ { -1,  1,  1, -1 },  1 },
        Sample{ { -1,  1,  1, -1 },  1 },
        Sample{ { -1,  1, -1, -1 }, -1 },
        Sample{ { -1, -1, -1, -1 }, -1 },
        Sample{ { -1,  1,  1,  1 },  1 },
        Sample{ {  1,  1,  1,  1 },  1 },
    };

    Population population;
    int populationSize = 20;
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(Chromosome(new Net()));
    }

    // train
    double targetFitness = 0.35;
    double mutationProb = 0.1;
    long iterationCount = 0;

    while (true) {
        for (Chromosome &ch : population) {
            calcFitness(ch, trainSet);
        }

        std::sort(population.begin(), population.end(), [](Chromosome &ch1, Chromosome &ch2) {
            return ch1.fitness < ch2.fitness;
        });

        std::cout << population[0].fitness << std::endl;
        if (population[0].fitness <= targetFitness)
            break;


        // crossover
        for (int i = 0; i < population.size() / 2; ++i) {
            // selection
            net *a = population[rand() % (population.size())].net;
            net *b = population[rand() % (population.size()) / 2].net;
            net *result = crossover(a, b);

            // mutation
            if (rand() % 100 < mutationprob * 100)
                mutate(result);

            // reduction
            int pos = rand() % population.size();
            population[pos] = chromosome(result);
        }


        ++iterationCount;
    }

    // finished learning
    std::cout << "Finished with: " << iterationCount << " iterations" << std::endl;

    // output recognition results
    Net *net = population[0].net;
    for (const Sample &sample : trainSet) {
        net->reset();

        for (double v : sample.stream) {
            net->activate({ v });
        }

        double result = net->output->getValue();
        if (std::abs(result - 1) < std::abs(result + 1))
            std::cout << 1;
        else
            std::cout << -1;

        std::cout << ": " << result << std::endl;
    }

    std::cin.get();
}