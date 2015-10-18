import $ from 'jquery';
import _ from 'lodash';
import RequestError from './RequestError';

export default class Client {
    static init(config, trainingSet, cb) {
        cb = cb || _.noop();
        Client._post('/api/init', {
            config: config,
            trainingSet: trainingSet
        }, {
            onError: (res) => cb(new RequestError(res)),
            onComplete: (res) => {
                if (res.status == "ok") {
                    cb(null, res.data);
                } else {
                    cb(new RequestError(res, true), null);
                }
            }
        });
    }

    static start(cb) {
        cb = cb || _.noop();
        Client._post('/api/start', {}, {
            onError: (res) => cb(new RequestError(res)),
            onComplete: (res) => {
                if (res.status == "ok") {
                    cb(null, res.data);
                } else {
                    cb(new RequestError(res, true), null);
                }
            }
        });
    }

    static recognize(stream, cb) {
        cb = cb || _.noop();
        Client._post('/api/recognize', {
            stream: stream
        }, {
            onError: (res) => cb(new RequestError(res)),
            onComplete: (res) => {
                if (res.status == "ok") {
                    cb(null, res.data);
                } else {
                    cb(new RequestError(res, true), null);
                }
            }
        });
    }

    static _get(url, data, callbacks) {
        $.ajax({
            type: "GET",
            contentType: "application/json",
            url: url,
            data: data
        })
            .done(function (msg) {
                Client.logData(url, msg);
                callbacks.onComplete(msg);
            })
            .fail(function (error) {
                callbacks.onError(error);
            });
    }

    static _post(url, data, callbacks) {
        $.ajax({
            type: "POST",
            contentType: "application/json",
            url: url,
            dataType: "json",
            data: JSON.stringify(data)
        })
            .done(function (msg) {
                Client.logData(url, msg);
                callbacks.onComplete(msg);
            })
            .fail(function (error) {
                callbacks.onError(error);
            });
    }

    static _postForm(url, data, callbacks) {
        $.ajax({
            type: "POST",
            url: url,
            dataType: "json",
            data: data
        })
            .done(function (msg) {
                Client.logData(url, msg);
                callbacks.onComplete(msg);
            })
            .fail(function (error) {
                callbacks.onError(error);
            });
    }

    static logData(url, res) {
        console.log("[" + url + "] Message received: ", res);
    }
}