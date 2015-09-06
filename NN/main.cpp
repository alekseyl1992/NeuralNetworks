#include <iostream>
#include "MLP.h"

void xorTask() {
    std::cout << "XOR: " << std::endl;

    MLP mlp({ 2, 3, 1 }, true);
    TrainingSet trainingSet =
    {
        { { 0, 0 },{ 0 } },
        { { 0, 1 },{ 1 } },
        { { 1, 0 },{ 1 } },
        { { 1, 1 },{ 0 } }
    };

    mlp.train(trainingSet, 1000, 1, 0.8);

    for (auto &pair : trainingSet) {
        auto result = mlp.activate(pair.first);
        std::cout << pair.first[0] << ", " << pair.first[1] << ": " << result[0] << std::endl;
    }
}

void lectionTask() {
    std::cout << "Lection: " << std::endl;

    // works well both w/ and w/o bias
    MLP mlp({ 9, 2, 1 }, false);
    TrainingSet trainingSet =
    {
        { { 1, 0, 0,
        0, 1, 0,
        0, 0, 1 },{ 1 } },
        { { 0, 0, 1,
        0, 1, 0,
        1, 0, 0 },{ 1 } },

        { { 1, 1, 0,
        0, 1, 0,
        0, 0, 1 },{ 0 } },
        { { 0, 0, 1,
        0, 1, 0,
        0, 0, 1 },{ 0 } },
        { { 0, 1, 0,
        0, 1, 0,
        0, 1, 0 },{ 0 } },
        { { 1, 1, 1,
        1, 1, 1,
        1, 1, 1 },{ 0 } },
        { { 1, 1, 0,
        0, 1, 0,
        0, 1, 1 },{ 0 } },
        { { 1, 0, 1,
        0, 1, 0,
        1, 0, 1 },{ 0 } }
    };

    mlp.train(trainingSet, 1000, 1, 0.8);

    for (auto &pair : trainingSet) {
        auto result = mlp.activate(pair.first);
        std::cout << pair.second[0] << ": " << result[0] << std::endl;
    }
}

int main() {
    //xorTask();
    lectionTask();
    std::cin.get();
}