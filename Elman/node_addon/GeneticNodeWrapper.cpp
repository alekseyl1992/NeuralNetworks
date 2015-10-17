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
using v8::Handle;

Persistent<Function> GeneticNodeWrapper::constructor;

GeneticNodeWrapper::GeneticNodeWrapper(Isolate *isolate, Local<Object> gConfig, Local<Object> nConfig, Local<Array> trainSet) {
    Genetic::Config _gConfig;

    std::cout << (_gConfig.populationSize = gConfig->Get(String::NewFromUtf8(isolate, "populationSize"))->IntegerValue()) << std::endl;
    std::cout << (_gConfig.mutationProb = gConfig->Get(String::NewFromUtf8(isolate, "mutationProb"))->NumberValue()) << std::endl;
    std::cout << (_gConfig.segmentDivider = gConfig->Get(String::NewFromUtf8(isolate, "segmentDivider"))->NumberValue()) << std::endl;
    std::cout << (_gConfig.targetFitness = gConfig->Get(String::NewFromUtf8(isolate, "targetFitness"))->NumberValue()) << std::endl;

    Net::Config _nConfig;
    std::cout << (_nConfig.hiddenCount = nConfig->Get(String::NewFromUtf8(isolate, "hiddenCount"))->IntegerValue()) << std::endl;
    std::cout << (_nConfig.inputsCount = nConfig->Get(String::NewFromUtf8(isolate, "inputsCount"))->IntegerValue()) << std::endl;

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
                double val = input->Get(k)->NumberValue();
                std::cout << val << " ";
                _input.push_back(val);
            }
            std::cout << std::endl;

            _stream.push_back(_input);
        }

        _sample.stream = _stream;
        _sample.result = _label;

        _trainSet.push_back(_sample);
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

    TrainWork *work = new TrainWork();
    work->request.data = work;
    
    GeneticNodeWrapper* obj = ObjectWrap::Unwrap<GeneticNodeWrapper>(args.Holder());
    work->genetic = obj->genetic;
    
    Local<Function> callback = Local<Function>::Cast(args[0]);
    work->callback.Reset(isolate, callback);
 
    // kick of the worker thread
    uv_queue_work(uv_default_loop(), &work->request, TrainAsync, TrainAsyncComplete);
}

void GeneticNodeWrapper::TrainAsync(uv_work_t *req) {
    TrainWork *work = static_cast<TrainWork *>(req->data);
    work->result = work->genetic->start();
}

void GeneticNodeWrapper::TrainAsyncComplete(uv_work_t *req, int status) {
    Isolate * isolate = Isolate::GetCurrent();

    v8::HandleScope handle_scope(isolate);

    TrainWork *work = static_cast<TrainWork *>(req->data);

    const unsigned argc = 1;
    Local<Value> argv[argc] = { Number::New(isolate, work->result) };

    Local<Function>::New(isolate, work->callback)->Call(Null(isolate), argc, argv);

    delete work;
}

void GeneticNodeWrapper::Recognize(const FunctionCallbackInfo<Value>& args) {
    Genetic::Stream _stream;
    Local<Array> stream = args[0].As<Array>();

    // for each "frame"
    for (int j = 0; j < stream->Length(); ++j) {
        Genetic::Input _input;
        Local<Array> input = stream->Get(j).As<Array>();

        for (int k = 0; k < input->Length(); ++k) {
            double val = input->Get(k)->NumberValue();
            std::cout << val << " ";
            _input.push_back(val);
        }
        std::cout << std::endl;

        _stream.push_back(_input);
    }

    GeneticNodeWrapper* obj = ObjectWrap::Unwrap<GeneticNodeWrapper>(args.Holder());
    long result = obj->genetic->recognize(_stream);

    args.GetReturnValue().Set(result);
}