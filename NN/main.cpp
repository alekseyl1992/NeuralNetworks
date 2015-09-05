#include <iostream>
#include "MLP.h"

int main() {
    MLP mlp({ 2, 3, 1 });
    TrainingSet trainingSet =
    {
        { { 0, 0 }, { 0 } },
        { { 0, 1 }, { 1 } },
        { { 1, 0 }, { 1 } },
        { { 1, 1 }, { 0 } }
    };

    mlp.train(trainingSet, 1000, 0.002, 0.1);

    for (auto &pair : trainingSet) {
        auto result = mlp.activate(pair.first);
        std::cout << pair.first[0] << ", " << pair.first[1] << " : " << result[0] << std::endl;
    }
    std::cin.get();
}