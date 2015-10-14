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

        Chromosome(const Chromosome &ch) = delete;
        Chromosome(Chromosome &&ch) {
            this->net = ch.net;
            this->fitness = ch.fitness;

            ch.net = nullptr;
        }

        Chromosome &operator=(const Chromosome &ch) = delete;
        Chromosome &operator=(Chromosome &&ch) {
            this->net = ch.net;
            this->fitness = ch.fitness;

            ch.net = nullptr;

            return *this;
        }

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

    using TrainSet = std::vector<Sample>;

    Genetic(Genetic::Config gConfig, Net::Config nConfig, TrainSet trainSet);
    virtual ~Genetic();

    double calcError(Net * net, vector<Input> stream, double expectedResult);
    void calcFitness(Chromosome & ch, const vector<Sample>& trainSet);
    Net * crossover(Net * a, Net * b);
    void mutate(Net * net);
    
    long start();
    double recognize(vector<Input> stream);

private:
    Genetic::Config gConfig;
    Net::Config nConfig;
    vector<Sample> trainSet;
    Chromosome result = Chromosome(nullptr);
};

