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
        if (err) {
            console.log(err);
            return;
        }

        console.log('Initialized');

        Client.start(function (err, data) {
            if (err) {
                console.log(err);
                return;
            }

            console.log('Trained with %d iteration', data.iterationsCount);

            Client.recognize(sample1[0], function (err, data) {
                if (err) {
                    console.log(err);
                    return;
                }

                console.log('Sample1 is ' + data.result);

                Client.recognize(sample2[0], function (err, data) {
                    if (err) {
                        console.log(err);
                        return;
                    }

                    console.log('Sample2 is ' + data.result);
                });
            });
        });
    });
}
