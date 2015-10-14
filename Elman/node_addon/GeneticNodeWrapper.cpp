#include "GeneticNodeWrapper.h"

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

Persistent<Function> GeneticNodeWrapper::constructor;

GeneticNodeWrapper::GeneticNodeWrapper(double value) : value_(value) {
}

GeneticNodeWrapper::~GeneticNodeWrapper() {
}

void GeneticNodeWrapper::Init(Local<Object> exports) {
    Isolate* isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "GeneticNodeWrapper"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "GeneticNodeWrapper"),
        tpl->GetFunction());
}

void GeneticNodeWrapper::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new GeneticNodeWrapper(...)`
        double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        GeneticNodeWrapper* obj = new GeneticNodeWrapper(value);
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

void GeneticNodeWrapper::PlusOne(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    GeneticNodeWrapper* obj = ObjectWrap::Unwrap<GeneticNodeWrapper>(args.Holder());
    obj->value_ += 1;

    args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}