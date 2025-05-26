#ifndef FAKER_NATIVE_H
#define FAKER_NATIVE_H

#include <node.h>


using namespace v8;
using v8::FunctionCallbackInfo;


namespace faker {

void NativeWrapper(const FunctionCallbackInfo<Value>& info);

}

#endif

