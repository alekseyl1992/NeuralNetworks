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
import Client from './Client';

export default class UI {
    constructor() {
        this.templates = {
            inputImage: handlebars.compile($('#input-image-template').html()),
            trainingSetLane: handlebars.compile($('#training-set-lane-template').html()),
            config: handlebars.compile($('#config-template').html())
        };

        this.$config = $('#config');
        this.$inputLane = $('.input-lane');
        this.$inputLaneAddImage = $('.input-lane-add-image');
        this.$trainingSet = $('#training-set');

        $('#apply').click(this.apply.bind(this));
        $('#train').click(this.train.bind(this));

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
                row.push(-1);
            }
            image.push(row);
        }

        var html = this.templates.inputImage(image);
        var $image = $(html);
        $image.appendTo(this.$inputLane);

        this.$inputLane.find('.cell').each((id, cell) => {
            var $cell = $(cell);
            $cell.off('click');
            $cell.click(this.flipCell.bind(this, $cell));
        });
    }

    clearInputLane() {
        this.$inputLane.empty();
        this.addInputImage();
    }

    flipCell($cell) {
        var value = $cell.data('value');
        $cell.removeClass('cell' + value);
        value = -value;
        $cell.data('value', value);

        //for .clone() to work
        $cell.attr('data-value', value);
        $cell.addClass('cell' + value);
    }

    getInputLaneData($el) {
        var images = [];

        var $images = $el.find('.input-lane__image');
        $images.each((i, image) => {
            var $image = $(image);
            var $cells = $image.find('.cell');

            var data = _.map($cells, (cell) => $(cell).data('value'));
            images.push(data);
        });

        return images;
    }

    addLane(label) {
        var $sample = this.$inputLane.clone();

        var $lane = $(this.templates.trainingSetLane({
            label: label
        }));

        $sample.appendTo($lane.find('.lane-wrapper'));

        $lane.appendTo(this.$trainingSet);

        $lane.find('.remove').click(() => $lane.remove());

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
        var trainingSet = [];

        this.$trainingSet.find('.sample').each((id, sample) => {
            var $sample = $(sample);
            var label = $sample.data('label');
            var data = this.getInputLaneData($sample.find('.input-lane'));

            trainingSet.push([data, label]);
        });

        // get config
        this.config = this.getConfig();

        // train
        Client.init(this.config, trainingSet, (err, data) => {
            if (err) {
                alertify.error('Init error');
                return;
            }

            alertify.success('Initialized');
            Client.start((err, data) => {
                if (err) {
                    alertify.error('Train error');
                    return;
                }

                alertify.success('Trained with ' + data.iterationsCount + ' iterations');
            });
        });
    }

    recognize() {
        var lane = this.getInputLaneData(this.$inputLane);
        Client.recognize(lane, (err, data) => {
            if (err) {
                alertify.error('Recognize error');
                return;
            }

            alertify.success('Recognized: ' + data.result);
        });
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