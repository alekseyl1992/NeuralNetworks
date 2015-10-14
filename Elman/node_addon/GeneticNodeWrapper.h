#pragma once

#include <node.h>
#include <node_object_wrap.h>
#include <memory>
#include "../Genetic.h"

class GeneticNodeWrapper : public node::ObjectWrap {
public:
    static void Init(v8::Local<v8::Object> exports);

private:
    explicit GeneticNodeWrapper(v8::Local<v8::Object> gConfig, v8::Local<v8::Object> nConfig, v8::Local<v8::Object> trainSet);
    ~GeneticNodeWrapper();

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Start(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Recognize(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Persistent<v8::Function> constructor;
    
    Genetic *genetic = nullptr;
};