import Client from './Client';

export default function runTests() {
    var gConfig = {
        populationSize: 2,
        targetFitness: 0.1,
        segmentDivider: 10,
        mutationProb: 0.1
    };

    var nConfig = {
        inputsCount: 9,
        hiddenCount: 9
    };

    var config = {
        nConfig: nConfig,
        gConfig: gConfig
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

    Client.init(config, trainSet, function (err, data) {
        Client.start(function (err, data) {

            Client.recognize(sample1[0], function (err, data) {
                Client.recognize(sample2[0], function (err, data) {

                });
            });
        });
    });
}

runTests();