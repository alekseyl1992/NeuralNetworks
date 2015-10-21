#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Net.h"
#include "Genetic.h"

using std::vector;

int mainOld() {
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
    gConfig.segmentDivider = 10;

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

    return 0;
}

long testIter(double value) {
    // prepare data
    vector<Genetic::Sample> trainSet = {
        Genetic::Sample{ { { 1 },{ 2 },{ 3 },{ 4 },{ 5 },{ 6 },{ 7 },{ 8 },{ 9 },{ 10 } }, 1 },
        Genetic::Sample{ { { 1 },{ 2 },{ 0 },{ 4 },{ 5 },{ 0 },{ 7 },{ 6 },{ 9 },{ 0 } }, 1 },
        Genetic::Sample{ { { 0 },{ 2 },{ 0 },{ 4 },{ 5 },{ 0 },{ 7 },{ 6 },{ 7 },{ 8 } }, 1 },
        Genetic::Sample{ { { 5 },{ 5 },{ 5 },{ 6 },{ 7 },{ 8 },{ 8 },{ 8 },{ 9 },{ 8 } }, 1 },

        Genetic::Sample{ { { 10 },{ 10 },{ 9 },{ 8 },{ 8 },{ 7 },{ 5 },{ 4 },{ 2 },{ 1 } }, -1 },
        Genetic::Sample{ { { 10 },{ 10 },{ 9 },{ 8 },{ 8 },{ 7 },{ 5 },{ 5 },{ 5 },{ 5 } }, -1 },
        Genetic::Sample{ { { 10 },{ 10 },{ 9 },{ 8 },{ 10 },{ 7 },{ 5 },{ 4 },{ 2 },{ 1 } }, -1 },
        Genetic::Sample{ { { 10 },{ 10 },{ 9 },{ 8 },{ 8 },{ 0 },{ 5 },{ 4 },{ 2 },{ 1 } }, -1 },
    };

    Genetic::Config gConfig;
    gConfig.populationSize = 50;
    gConfig.targetFitness = 0.1;
    gConfig.mutationProb = 0.1;
    gConfig.segmentDivider = 2;

    Net::Config nConfig;
    nConfig.inputsCount = 1;
    nConfig.hiddenCount = value;

    Genetic genetic(gConfig, nConfig, trainSet);


    // train
    long iterationCount = genetic.start();
    //std::cout << "Finished with: " << iterationCount << " iterations" << std::endl;

    // recognize
    //std::cout << genetic.recognize({ { 10 },{ 9 },{ 9 },{ 8 },{ 8 },{ 0 },{ 5 },{ 4 },{ 8 },{ 1 } }) << std::endl;
    //std::cout << genetic.recognize({ { 10 },{ 7 },{ 9 },{ 8 },{ 8 },{ 0 },{ 5 },{ 4 },{ 2 },{ 1 } }) << std::endl;

    return iterationCount;
}

int main() {
    for (double value = 2; value <= 10; value += 1) {
        long timeAvg = 0;
        std::vector<long> times;

        int repeatsCount = 100;

        for (int i = 0; i < repeatsCount; ++i) {
            long time = testIter(value);
            timeAvg += time;
            times.push_back(time);
        }

        std::sort(times.begin(), times.end());

        std::cout << value << " : " << double(timeAvg) / repeatsCount << " | " << times[repeatsCount / 2] << std::endl;
    }

    std::cout << "---" << std::endl;
    std::cin.get();
}