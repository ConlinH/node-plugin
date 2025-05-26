#ifndef FAKER_TOOLS_H
#define FAKER_TOOLS_H

#include <node.h>


using namespace v8;
using v8::FunctionCallbackInfo;


namespace faker {

void Print(const FunctionCallbackInfo<Value>& info);
void GetArg(const FunctionCallbackInfo<Value>& info);
void SetArg(const FunctionCallbackInfo<Value>& info);

}

#endif