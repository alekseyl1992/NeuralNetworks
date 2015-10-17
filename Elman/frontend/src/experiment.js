// Bootstrap 3 doesn't support UMD properly (BS4 will do though)
global.$ = global.jQuery = require('jquery');
require('bootstrap');
require('flot');

import _ from 'lodash';

import Perceptron from './perceptron';


class Experiment {
    constructor() {
        this.iterationsPerTest = 50;

        this.trainSet = {
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

        var config = {
            imageSize: 3,
            speed: 0.1,
            threshold: 0.5
        };

        $('#start').click(() => {
            this.plot(
                this.experiment(config, 'threshold', -3, 3, 0.5),
                'threshold', 'steps');
        });
    }

    experiment(cfg, param, from, to, step) {
        var data = [];

        for (let value = from; value <= to; value += step) {
            var config = _.clone(cfg);
            config[param] = value;
            var result = this.test(config);
            result.value = value;
            data.push(result);
        }

        return data;
    }

    test(config) {
        var time = [];
        var steps = [];
        for (let i = 0; i < this.iterationsPerTest; ++i) {
            var p = new Perceptron(config.imageSize * config.imageSize, config.threshold);

            var t0 = new Date().getTime();
            var result = p.train(this.trainSet, config.speed);
            var t1 = new Date().getTime();

            time.push(t1 - t0);
            steps.push(result.stepsCount);
        }

        time = _.sortBy(time);
        steps = _.sortBy(steps);

        return {
            time: _.reduce(time, (acc, v) => acc + v) / this.iterationsPerTest,
            steps: _.reduce(steps, (acc, v) => acc + v) / this.iterationsPerTest
        };
    }

    plot(data, x, y) {
        data = [{
            label: y + ' vs ' + x,
            data: _.map(data, (obj) => {
                console.log(obj.value.toFixed(1) + '\t' + obj[y]);
                return [obj.value, obj[y]];
            })
        }];

        $.plot("#experiment", data, {
            series: {
                lines: { show: true },
                points: { show: true }
            },
            grid: {
                hoverable: true,
                clickable: true
            },
            xaxis: {
                label: x
            },
            yaxis: {
                label: y
            }
        });
    }
}

$(() => new Experiment());
