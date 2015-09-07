#pragma once
#include <vector>
#include <random>
#include <iostream>
#include "Layer.h"

using TrainPair = std::pair<arr, arr>;
using TrainingSet = std::vector<TrainPair>;

class MLP {
private:
    std::vector<Layer> layers;

public:
    MLP(std::vector<double> layerSizes, bool bias) {
        for (size_t i = 1; i < layerSizes.size(); ++i) {
            layers.emplace_back(Layer(layerSizes[i], layerSizes[i - 1], doubleRand, bias));
        }
    }

    arr activate(arr inputs) {
        for (Layer &l : layers) {
            inputs = l.activate(inputs);
        }

        return inputs;
    }

    void trainUntilSteps(const TrainingSet &trainingSet, size_t maxSteps, double speed, double smoothing) {
        for (size_t i = 0; i < maxSteps; ++i) {
            for (const TrainPair &pair : trainingSet) {
                activate(pair.first);
                trainIteration(pair, speed, smoothing);
            }
        }
    }

    size_t trainUntilError(const TrainingSet &trainingSet, double maxError, double speed, double smoothing) {
        double error = 0;
        size_t steps = 0;

        do {
            error = 0;

            for (auto &pair : trainingSet) {
                const arr &output = activate(pair.first);
                error += getError(pair.second, output);

                trainIteration(pair, speed, smoothing);
            }
            ++steps;
        } while (error / trainingSet.size() > maxError);

        return steps;
    }

    void trainIteration(const TrainPair &pair, double speed, double smoothing) {
        int lastLId = layers.size() - 1;
        layers[lastLId].trainLast(pair.second);

        for (int lId = lastLId - 1; lId >= 0; --lId) {
            layers[lId].train(layers[lId + 1]);
        }

        layers[0].updateWeights(pair.first, speed, smoothing);

        for (int lId = 1; lId <= lastLId; ++lId) {
            layers[lId].updateWeights(layers[lId - 1], speed, smoothing);
        }
    }

    double getError(const arr &expected, const arr &actual) {
        double sum = 0;
        for (size_t i = 0; i < expected.size(); ++i) {
            sum += (expected[i] - actual[i]) * (expected[i] - actual[i]);
        }

        return sum;
    }

    static double doubleRand() {
        static std::random_device rd;
        static std::mt19937 engine(rd());
        static std::uniform_real_distribution<> dist(0, 1);

        return dist(engine);
    }
};