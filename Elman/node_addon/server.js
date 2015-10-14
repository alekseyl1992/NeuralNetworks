var addon = require('./build/Release/addon');

var nConfig = {

};

var gConfig = {

};

var trainSet = {

};

var obj = new addon.GeneticNodeWrapper(nConfig, gConfig, trainSet);
obj.start((iterationsCount) => console.log("Finished after: " + iterationsCount));
obj.recognize({});

setTimeout(() => console.log('Timed out'), 5000);