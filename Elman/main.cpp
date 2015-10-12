#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Net.h"
#include "Genetic.h"

using std::vector;

int main() {
    // prepare data
    vector<Genetic::Sample> trainSet = {
        Genetic::Sample{
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
        Genetic::Sample{
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

    Genetic::Config gConfig;
    gConfig.populationSize = 2;
    gConfig.targetFitness = 0.1;
    gConfig.mutationProb = 0.1;

    Net::Config nConfig;
    nConfig.inputsCount = 9;
    nConfig.hiddenCount = 9;

    Genetic genetic(gConfig, nConfig, trainSet);

    
    // train
    long iterationCount = genetic.start();
    std::cout << "Finished with: " << iterationCount << " iterations" << std::endl;

    // output recognition results
    /*Net *net = population[0].net;
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
    }*/

    std::cin.get();
}