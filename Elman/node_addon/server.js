var addon = require('./build/Release/addon');

var gConfig = {
    populationSize: 2,
    targetFitness: 0.1,
    segmentDivider: 10,
    mutationProb: 0.1,
};


var nConfig = {
    inputsCount: 9,
    hiddenCount: 9,
};

var sample1 = [
    [
        [
            -1, -1, -1,
             1, -1, -1,
            -1, -1, -1
        ],
        [
            -1, -1, -1,
            -1,  1, -1,
            -1, -1, -1
        ],
        [
            -1, -1, -1,
            -1, -1,  1,
            -1, -1, -1
        ],
        [
            -1, -1, -1,
            -1, -1, -1,
            -1, -1, -1
        ],
    ],
    1
];

var sample2 = [
    [
        [
            -1, -1, -1,
            -1, -1,  1,
            -1, -1, -1
        ],
        [
            -1, -1, -1,
            -1,  1, -1,
            -1, -1, -1
        ],
        [
            -1, -1, -1,
             1, -1, -1,
            -1, -1, -1
        ],
        [
            -1, -1, -1,
            -1, -1, -1,
            -1, -1, -1
        ],
    ],
    -1
];

var trainSet = [sample1, sample2];

var obj = new addon.GeneticNodeWrapper(gConfig, nConfig, trainSet);
console.log(obj);

obj.start((iterationsCount) => {
    console.log("Finished after: " + iterationsCount);

    console.log(obj.recognize(sample1[0]));
    console.log(obj.recognize(sample2[0]));
});

setTimeout(() => console.log('Timed out'), 5000);