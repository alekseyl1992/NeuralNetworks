#pragma once

#include <v8.h>
#include <node.h>
#include <uv.h>
#include <node_object_wrap.h>
#include <memory>
#include "../Genetic.h"

class GeneticNodeWrapper : public node::ObjectWrap {
public:
    static void Init(v8::Local<v8::Object> exports);

    struct TrainWork {
        uv_work_t request;
        v8::Persistent<v8::Function> callback;

        Genetic *genetic;
        double result;
    };

private:
    explicit GeneticNodeWrapper(v8::Isolate *isolate, v8::Local<v8::Object> gConfig, v8::Local<v8::Object> nConfig, v8::Local<v8::Array> trainSet);
    ~GeneticNodeWrapper();

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Start(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void TrainAsync(uv_work_t * req);
    static void TrainAsyncComplete(uv_work_t * req, int status);
    static void Recognize(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Persistent<v8::Function> constructor;
    
    Genetic *genetic = nullptr;
};