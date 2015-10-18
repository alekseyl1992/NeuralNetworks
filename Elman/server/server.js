var addon = require('../node_addon/build/Release/addon');
var express = require('express');
var bodyParser = require('body-parser');
var session = require('express-session');
var cookieParser = require('cookie-parser');

var app = express();
app.use(express.static('../frontend'));
app.use(bodyParser.json());
app.use(cookieParser());
app.use(session({
    secret: 'keyboard cat',
    resave: false,
    saveUninitialized: true
}));

var genetics = [];

app.post('/api/init', function (req, res) {
    console.log('init');
    console.log(req.sessionID);
    
    genetics[req.sessionID] = new addon.GeneticNodeWrapper(
        req.body.config.gConfig,
        req.body.config.nConfig,
        req.body.trainingSet);
   
    res.json({
        status: 'ok'
    });
});

app.post('/api/start', function (req, res) {
    console.log('start');
    console.log(req.sessionID);
    
    var genetic = genetics[req.sessionID];
    console.log(genetic);
    
    genetic.start((iterationsCount) => {
        console.log("Finished after: " + iterationsCount);

        res.json({
            status: 'ok',
            data: {
                iterationsCount: iterationsCount
            }
        });
    });
});

app.post('/api/recognize', function (req, res) {
    console.log('recognize');
    var genetic = genetics[req.sessionID];
    var result = genetic.recognize(req.body.stream);
    
    res.json({
        status: 'ok',
        data: {
            result: result
        }
    });
});


var server = app.listen(3000, function () {
    var host = server.address().address;
    var port = server.address().port;

    console.log('Example app listening at http://%s:%s', host, port);
});
