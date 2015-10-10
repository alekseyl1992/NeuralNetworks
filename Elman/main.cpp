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
using Input = vector<double>;

struct Sample {
    vector<Input> stream;
    double result;
};


double calcError(Net *net, vector<Input> stream, double expectedResult) {
    for (const Input &value : stream) {
        net->activate(value);
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

    Net* sources[] = { a, b };
    Net *source = sources[rand() % 2];

    for (int i = 0; i < result->edges.size(); ++i) {
        int segmentSize = rand() % result->edges.size() / 10;

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

void mutate(Net *net) {
    for (int i = 0; i < net->edges.size(); ++i) {
        Edge *edge = net->edges[i];

        edge->w = Net::getRandomWeight(edge->w);
    }
}

int main() {
    // prepare data
    vector<Sample> trainSet = {
        Sample{
            {
                {
                    -1, -1, -1,
                     1, -1, -1,
                    -1, -1, -1
                },
                {
                    -1, -1, -1,
                    -1,  1, -1,
                    -1, -1, -1
                },
                {
                    -1, -1, -1,
                    -1, -1,  1,
                    -1, -1, -1
                },
                {
                    -1, -1, -1,
                    -1, -1, -1,
                    -1, -1, -1
                },
            }, 1 },
        Sample{
            {
                {
                    -1, -1, -1,
                    -1, -1,  1,
                    -1, -1, -1
                },
                {
                    -1, -1, -1,
                    -1,  1, -1,
                    -1, -1, -1
                },
                {
                    -1, -1, -1,
                     1, -1, -1,
                    -1, -1, -1
                },
                {
                    -1, -1, -1,
                    -1, -1, -1,
                    -1, -1, -1
                },
            }, -1 },
    };

    Population population;
    int populationSize = 20;
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(Chromosome(new Net()));
    }

    // train
    double targetFitness = 0.1;
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
            Net *a = population[rand() % (population.size())].net;
            Net *b = population[rand() % (population.size()) / 2].net;
            Net *result = crossover(a, b);

            // mutation
            if (rand() % 100 < mutationProb * 100)
                mutate(result);

            // reduction
            int pos = population.size() - i - 1;
            population[pos] = Chromosome(result);
        }


        ++iterationCount;
    }

    // finished learning
    std::cout << "Finished with: " << iterationCount << " iterations" << std::endl;

    // output recognition results
    Net *net = population[0].net;
    for (const Sample &sample : trainSet) {
        net->reset();

        for (const Input &v : sample.stream) {
            net->activate(v);
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