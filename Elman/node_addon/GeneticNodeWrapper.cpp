#include "GeneticNodeWrapper.h"
#include <thread>
#include <iostream>

using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;
using v8::Exception;
using v8::Array;

Persistent<Function> GeneticNodeWrapper::constructor;

GeneticNodeWrapper::GeneticNodeWrapper(Isolate *isolate, Local<Object> gConfig, Local<Object> nConfig, Local<Array> trainSet) {
    Genetic::Config _gConfig;

    _gConfig.populationSize = gConfig->Get(String::NewFromUtf8(isolate, "populationSize"))->IntegerValue();
    _gConfig.mutationProb = gConfig->Get(String::NewFromUtf8(isolate, "mutationProb"))->NumberValue();
    _gConfig.segmentDivider = gConfig->Get(String::NewFromUtf8(isolate, "segmentDivider"))->NumberValue();
    _gConfig.targetFitness = gConfig->Get(String::NewFromUtf8(isolate, "targetFitness"))->NumberValue();

    Net::Config _nConfig;
    _nConfig.hiddenCount = nConfig->Get(String::NewFromUtf8(isolate, "hiddenCount"))->IntegerValue();
    _nConfig.inputsCount = nConfig->Get(String::NewFromUtf8(isolate, "inputsCount"))->IntegerValue();

    Genetic::TrainSet _trainSet;
    // for each "video" pair
    for (int i = 0; i < trainSet->Length(); ++i) {
        Genetic::Sample _sample;
        Local<Array> sample = trainSet->Get(i).As<Array>();

        Genetic::Stream _stream;
        Local<Array> stream = sample->Get(0).As<Array>();
        double _label = sample->Get(1)->NumberValue();
        std::cout << _label << std::endl;

        // for each "frame"
        for (int j = 0; j < stream->Length(); ++j) {
            Genetic::Input _input;
            Local<Array> input = stream->Get(j).As<Array>();

            for (int k = 0; k < input->Length(); ++k) {
                _input.push_back(input->Get(k)->NumberValue());
            }

            _stream.push_back(_input);
        }

        _sample.stream = _stream;
        _sample.result = _label;
    }


    genetic = new Genetic(_gConfig, _nConfig, _trainSet);
}

GeneticNodeWrapper::~GeneticNodeWrapper() {
    delete genetic;
    genetic = nullptr;
}

void GeneticNodeWrapper::Init(Local<Object> exports) {
    Isolate* isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "GeneticNodeWrapper"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "start", Start);
    NODE_SET_PROTOTYPE_METHOD(tpl, "recognize", Recognize);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "GeneticNodeWrapper"),
        tpl->GetFunction());
}

void GeneticNodeWrapper::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new GeneticNodeWrapper(...)`
        if (args.Length() != 3) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "Wrong arguments")));
            return;
        }

        Local<Object> gConfig = args[0].As<Object>();
        Local<Object> nConfig = args[1].As<Object>();
        Local<Array> trainSet = args[2].As<Array>();

        GeneticNodeWrapper* obj = new GeneticNodeWrapper(isolate, gConfig, nConfig, trainSet);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
    else {
        // Invoked as plain function `GeneticNodeWrapper(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        args.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

void GeneticNodeWrapper::Start(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Function> cb = Local<Function>::Cast(args[0]);

    std::thread th([=]() {
        GeneticNodeWrapper* obj = ObjectWrap::Unwrap<GeneticNodeWrapper>(args.Holder());
        long result = obj->genetic->start();

        const unsigned argc = 1;
        Local<Value> argv[argc] = { Number::New(isolate, result) };
        cb->Call(Null(isolate), argc, argv);
    });
}

void GeneticNodeWrapper::Recognize(const FunctionCallbackInfo<Value>& args) {
    // TODO: parse params and call
    // genetic-recognize();
}