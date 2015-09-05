#pragma once
#include <vector>
#include <random>
#include "Layer.h"

using TrainingSet = std::vector<std::pair<arr, arr>>;

class MLP {
private:
    std::vector<Layer> layers;

public:
    MLP(std::vector<double> layerSizes) {
        for (size_t i = 1; i < layerSizes.size(); ++i) {
            layers.emplace_back(Layer(layerSizes[i], layerSizes[i - 1], doubleRand));
        }
    }

    arr activate(arr inputs) {
        for (Layer &l : layers) {
            inputs = l.activate(inputs);
        }

        return inputs;
    }

    void train(TrainingSet trainingSet, size_t maxSteps, double speed, double smoothing) {
        for (size_t i = 0; i < maxSteps; ++i) {
            for (auto &pair : trainingSet) {
                activate(pair.first);

                int lastLId = layers.size() - 1;
                layers[lastLId].trainLast(pair.second);

                for (int lId = lastLId - 1; lId >= 0; --lId) {
                    layers[lId].train(layers[lId + 1]);
                }

                layers[0].updateWeights(pair.first, speed, smoothing);
                for (int lId = 1; i <= lastLId; ++i) {
                    layers[lId].updateWeights(layers[lId - 1], speed, smoothing);
                }
            }
        }
    }

    static double doubleRand() {
        static std::random_device rd;
        static std::mt19937 engine(rd());
        static std::uniform_real_distribution<> dist(0, 1);

        return dist(engine);
    }
};