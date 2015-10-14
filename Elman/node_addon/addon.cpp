#include <node.h>
#include "GeneticNodeWrapper.h"


using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports) {
    GeneticNodeWrapper::Init(exports);
}

NODE_MODULE(addon, InitAll)