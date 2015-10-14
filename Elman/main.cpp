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

    // recognize
    std::cout << genetic.recognize(trainSet[0].stream) << std::endl;
    std::cout << genetic.recognize(trainSet[1].stream) << std::endl;

    std::cin.get();
}