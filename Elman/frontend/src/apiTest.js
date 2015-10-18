export default function runTests() {
    var p = new Perceptron(9, 0.5);
    var trainSet = {
        0: [[
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        ]],
        1: [[
            0, 0, 1,
            0, 1, 0,
            1, 0, 0
        ]]
    };

    console.log('Test started');
    var result = p.train(trainSet, 0.1);
    console.log('Steps: ' + result.stepsCount);
    console.assert(p.activate(trainSet[0][0]).value == 0);
    console.assert(p.activate(trainSet[1][0]).value == 1);
    console.log('Test finished');
}