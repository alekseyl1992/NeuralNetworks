#include "Genetic.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Net.h"

using std::vector;

Genetic::Genetic(Genetic::Config gConfig, Net::Config nConfig, TrainSet trainSet)
    : gConfig(gConfig), nConfig(nConfig), trainSet(trainSet) {

}

Genetic::~Genetic() {

}

double Genetic::calcError(Net *net, vector<Input> stream, double expectedResult) {
    for (const Input &value : stream) {
        net->activate(value);
    }

    double result = net->output->getValue();
    double e = std::abs(result - expectedResult);

    return e;
}

void Genetic::calcFitness(Chromosome &ch, const vector<Sample> &trainSet) {
    double f = 0;
    for (const Sample &sample : trainSet) {
        ch.net->reset();
        f += calcError(ch.net, sample.stream, sample.result);
    }

    ch.fitness = f / trainSet.size();
}

Net *Genetic::crossover(Net *a, Net *b) {
    Net *result = new Net(nConfig);

    Net* sources[] = { a, b };
    Net *source = sources[rand() % 2];

    for (int i = 0; i < result->edges.size(); ++i) {
        int segmentSize = rand() % result->edges.size() / gConfig.segmentDivider;

        for (int j = 0; j < segmentSize; ++j) {
            if (i + j >= result->edges.size())
                break;

            Edge *edge = result->edges[i + j];
            edge->w = source->edges[i + j]->w;
        }

        i += segmentSize;
        source = sources[rand() % 2];
    }

    return result;
}

void Genetic::mutate(Net *net) {
    for (int i = 0; i < net->edges.size(); ++i) {
        Edge *edge = net->edges[i];

        edge->w = Net::getRandomWeight(edge->w);
    }
}

long Genetic::start() {
    // create initial population
    Population population;
    for (int i = 0; i < gConfig.populationSize; ++i) {
        population.push_back(Chromosome(new Net(nConfig)));
    }

    // train
    long iterationCount = 0;
    while (true) {
        //std::cout << iterationCount << ") ";
        for (Chromosome &ch : population) {
            calcFitness(ch, trainSet);
        }

        std::sort(population.begin(), population.end(), [](Chromosome &ch1, Chromosome &ch2) {
            return ch1.fitness < ch2.fitness;
        });

        //std::cout << population[0].fitness << std::endl;
        if (population[0].fitness <= gConfig.targetFitness)
            break;


        // crossover
        for (int i = 0; i < population.size() / 2; ++i) {
            // selection
            Net *a = population[rand() % (population.size())].net;
            Net *b = population[rand() % (population.size()) / 2].net;
            Net *result = crossover(a, b);

            // mutation
            if (rand() % 100 < gConfig.mutationProb * 100)
                mutate(result);

            // reduction
            int pos = population.size() - i - 1;
            population[pos] = std::move(Chromosome(result));
        }


        ++iterationCount;
    }

    // save best network in result
    result = std::move(population[0]);

    return iterationCount;
}

double Genetic::recognize(vector<Input> stream) {
    Net *net = result.net;    
    net->reset();

    for (const Input &v : stream) {
        net->activate(v);
    }

    double result = net->output->getValue();
    if (std::abs(result - 1) < std::abs(result + 1))
        return 1;
    else
        return -1;
}