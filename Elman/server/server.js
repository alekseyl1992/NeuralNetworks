var addon = require('../node_addon/build/Release/addon');
var express = require('express');
var bodyParser = require('body-parser');
var session = require('express-session');

var app = express();
app.use(express.static('../frontend'));
app.use(bodyParser.json());
app.use(session({
    genid: function(req) {
        return genuuid() // use UUIDs for session IDs
    },
    secret: 'keyboard cat'
}));


app.get('/api/init', function (req, res) {
    res.send('Hello World!');
});

app.get('/api/start', function (req, res) {
    res.send('Hello World!');
});

app.get('/api/recognize', function (req, res) {
    res.send('Hello World!');
});


var server = app.listen(3000, function () {
    var host = server.address().address;
    var port = server.address().port;

    console.log('Example app listening at http://%s:%s', host, port);
});


//var trainSet = [sample1, sample2];

//var obj = new addon.GeneticNodeWrapper(gConfig, nConfig, trainSet);
//console.log(obj);

//obj.start((iterationsCount) => {
//    console.log("Finished after: " + iterationsCount);

//    console.log(obj.recognize(sample1[0]));
//    console.log(obj.recognize(sample2[0]));
//});

//setTimeout(() => console.log('Timed out'), 5000);