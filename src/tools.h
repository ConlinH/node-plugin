#ifndef FAKER_TOOLS_H
#define FAKER_TOOLS_H

#include <node.h>


using namespace v8;
using v8::FunctionCallbackInfo;


namespace faker {

void Print(const FunctionCallbackInfo<Value>& info);
void GetData(const FunctionCallbackInfo<Value>& info);
void SetData(const FunctionCallbackInfo<Value>& info);
void DelData(const FunctionCallbackInfo<Value>& info);

}

#endif