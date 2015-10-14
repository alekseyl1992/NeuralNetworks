#pragma once

#include <node.h>
#include <node_object_wrap.h>


class GeneticNodeWrapper : public node::ObjectWrap {
public:
    static void Init(v8::Local<v8::Object> exports);

private:
    explicit GeneticNodeWrapper(double value = 0);
    ~GeneticNodeWrapper();

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Persistent<v8::Function> constructor;
    double value_;
};