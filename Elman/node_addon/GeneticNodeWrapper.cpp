#include "GeneticNodeWrapper.h"
#include <thread>

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

Persistent<Function> GeneticNodeWrapper::constructor;

GeneticNodeWrapper::GeneticNodeWrapper(Local<Object> gConfig, Local<Object> nConfig, Local<Object> trainSet) {
    // TODO: parse params to structures
    Genetic::Config _gConfig;

    Net::Config _nConfig;

    Genetic::TrainSet _trainSet;


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
        Local<Object> trainSet = args[2].As<Object>();

        GeneticNodeWrapper* obj = new GeneticNodeWrapper(gConfig, nConfig, trainSet);
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