// Bootstrap 3 doesn't support UMD properly (BS4 will do though)
global.$ = global.jQuery = require('jquery');
require('bootstrap');
require('flot');

import _ from 'lodash';
import vis from 'vis';
import alertify from 'alertifyjs';
import handlebars from 'handlebars';

import Perceptron from './perceptron';
import apiTest from './apiTest';


export default class UI {
    constructor() {
        this.templates = {
            inputImage: handlebars.compile($('#input-image-template').html()),
            trainingSetLane: handlebars.compile($('#training-set-image-template').html()),
            config: handlebars.compile($('#config-template').html())
        };

        this.$config = $('#config');
        this.$inputLane = $('.input-lane');
        this.$inputLaneAddImage = $('.input-lane-add-image');

        this.$trainingSet = $('#training-set');
        this.$displayValue = $('.display__value');

        $('#apply').click(this.apply.bind(this));
        $('#train').click(this.train.bind(this));
        $('#reset').click(this.reset.bind(this));

        this.$inputLaneAddImage.click(() => {
            this.addInputImage();
        });

        $('.add-image').click((e) => {
            var label = $(e.target).data('value');
            this.addLane(label);
        });
        $('.clear').click(this.clearInputLane.bind(this));
        $('#recognize').click(this.recognize.bind(this));
        $('#run-tests').click(apiTest);


        $('.nav-tabs a').click(function (e) {
            e.preventDefault();
            $(this).tab('show');
        });

        this.config = {
            gConfig: {
                populationSize: 2,
                targetFitness: 0.1,
                segmentDivider: 10,
                mutationProb: 0.1
            },
            nConfig: {
                inputsCount: 9,
                hiddenCount: 9
            }
        };

        this.renderConfig(this.config);
        this.addInputImage();
    }

    addInputImage() {
        var image = [];
        var imageSize = Math.sqrt(this.config.nConfig.inputsCount);

        for (let i = 0; i < imageSize; ++i) {
            let row = [];
            for (let j = 0; j < imageSize; ++j) {
                row.push(0);
            }
            image.push(row);
        }

        var html = this.templates.inputImage(image);
        var $image = $(html);
        $image.appendTo(this.$inputLane);

        this.$inputLane.find('.cell').each((id, cell) => {
            var $cell = $(cell);
            $cell.click(this.flipCell.bind(this, $cell));
        });
    }

    clearInputLane() {
        this.$inputLane.empty();
    }

    flipCell($cell) {
        var value = $cell.data('value');
        $cell.removeClass('cell' + value);
        value = +!value;
        $cell.data('value', value);
        $cell.addClass('cell' + value);
    }

    getInputLane($el) {
        var cells = $el.find('.cell');
        return _.map(cells, (cell) => $(cell).data('value'));
    }

    addLane(label) {
        var imageSize = Math.sqrt(this.config.nConfig.inputsCount);

        var lane = {
            label: label,
            image: _.chunk(this.getInputLane(this.$inputLane), imageSize)
        };

        var $lane = $(this.templates.trainingSetLane(lane));


        $lane.appendTo(this.$trainingSet);

        $lane.find('.remove').click(() => $image.remove());

        // clear input
        this.clearInputLane();
    }

    apply() {
        this.config = this.getConfig();

        this.$trainingSet.empty();
        this.clearInputLane();
    }

    train() {
        // collect training set
        var trainingSet = {
            0: [],
            1: []
        };

        this.$trainingSet.find('.sample').each((id, sample) => {
            var $sample = $(sample);
            var label = $sample.data('label');
            var image = this.getInputImage($sample.find('.image-table'));

            trainingSet[label].push(image);
        });

        // get config
        this.config = this.getConfig();

        // train
        this.perceptron = new Perceptron(
            this.config.imageSize * this.config.imageSize,
            this.config.threshold);

        var result = this.perceptron.train(trainingSet, this.config.speed);

        alertify.success('Training finished after ' + result.stepsCount + ' steps');
        this.plot(result.history);
    }

    reset() {
        this.perceptron.reset();
    }

    recognize() {
        var image = this.getInputImage(this.$inputImage);
        var value = this.perceptron.activate(image);
        this.$displayValue.text(value.value + ' (' + value.raw.toFixed(2) + ')');
    }

    renderConfig(config) {
        this.config = config;
        var html = this.templates.config({
            config: config
        });
        this.$config.html(html);
    }

    getConfig() {
        var config = {
            gConfig: {},
            nConfig: {}
        };

        this.$config.find('input').each((id, input) => {
            var $input = $(input);
            var val = parseFloat($input.val());
            if (_.isNaN(val))
                val = $input.val();

            var subConfig = $input.data('config');
            var key = $input.data('key');
            config[subConfig][key] = val;
        });

        return config;
    }
}