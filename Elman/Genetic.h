#pragma once
#include <vector>
#include "Net.h"

class Genetic {
public:
    struct Config {
        int populationSize = 20;
        double targetFitness = 0.1;
        double mutationProb = 0.1;
        double segmentDivider = 10;
    };

    struct Chromosome {
        Net *net;
        double fitness = 0;

        Chromosome(Net *net)
            : net(net), fitness(0) { }

        ~Chromosome() {
            delete net;
            net = nullptr;
        }
    };

    using Population = std::vector<Chromosome>;
    using Input = std::vector<double>;

    struct Sample {
        std::vector<Input> stream;
        double result;
    };

    Genetic(Genetic::Config gConfig, Net::Config nConfig, vector<Sample> trainSet);
    virtual ~Genetic();

    double calcError(Net * net, vector<Input> stream, double expectedResult);
    void calcFitness(Chromosome & ch, const vector<Sample>& trainSet);
    Net * crossover(Net * a, Net * b);
    void mutate(Net * net);
    long start();

private:
    Genetic::Config gConfig;
    Net::Config nConfig;
    vector<Sample> trainSet;
};

